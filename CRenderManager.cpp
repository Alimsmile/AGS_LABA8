#include "CRenderManager.h"
#include "Omp.h"

struct PerSceneBlock
{
	glm::mat4 ProjectionMatrix;
	glm::vec4 lAmbient;
	glm::vec4 lDiffuse;
	glm::vec4 lSpecular;
	glm::vec4 lPosition;
};

struct PerObjectBlock
{
	glm::mat4 ModelViewMatrix;
	glm::vec4 mAmbient;
	glm::vec4 mDiffuse;
	glm::vec4 mSpecular;
};

void CRenderManager::setCamera(CCamera* Camera)
{
	if (this->Camera != *Camera)
	{
		this->Camera = *Camera;
		updatePerSceneBlock(PerSceneBlockBuffer);
#pragma omp parallel for num_threads(4)
		for (auto it = ObjectsState.begin(); it != ObjectsState.end(); it++)
		{
			it->second.UBOIndexUpdate = true;
		}
	}
}

void CRenderManager::setLight(CLight& Light)
{
	this->Light = Light;
}

void CRenderManager::AddToRenderQueue(CGraphicObject& GraphicObject)
{
	GLuint ObjID = GraphicObject.getID();
	if (ObjectsState[ObjID].UBOIndex == 0)
	{
		glGenBuffers(1, &ObjectsState[ObjID].UBOIndex);
		ObjectsState[ObjID].GraphicObject = GraphicObject;
	}
	if (ObjectsState[ObjID].GraphicObject != GraphicObject)
	{
		ObjectsState[ObjID].UBOIndexUpdate = true;
		ObjectsState[ObjID].GraphicObject = GraphicObject;
	}
	GraphicObjects.push_back(GraphicObject);
}

void CRenderManager::start(void)
{
	FBO_BUFFERS[CurMSAA].bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	GraphicObjects.clear();
}

void CRenderManager::SkyBoxRender(void)
{
	glDepthFunc(GL_ALWAYS);
	glCullFace(GL_FRONT);
	Shader[SKYBOX_SHADER_ID].Activate();
	CResourceManager::Instance().GetTexture(SkyBoxTextureId)->Apply(GL_TEXTURE0);
	Shader[SKYBOX_SHADER_ID].SetUniform1i("tex", 0);
	Shader[SKYBOX_SHADER_ID].SetUniformMat4("uProjectionMatrix", Camera.GetProjectionMatrix());
	Shader[SKYBOX_SHADER_ID].SetUniformMat4("uModelViewMatrix", Camera.GetViewMatrix() *
		mat4 { { 1, 0, 0, 0 },
		{ 0, 1, 0, 0 },
		{ 0, 0, 1, 0 },
		{ 0, 0, 0, 1 },
	});
	CResourceManager::Instance().GetMesh(SkyBoxMeshId)->Render();
	Shader[SKYBOX_SHADER_ID].Deactivate();
	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
}

void CRenderManager::ObjectsRender(void)
{
	// активируем шейдер
	Shader[DIFFUSE_TEXTURE_SHADER_ID].Activate();
	// матрицa наблюдения
	mat4 ViewMatrix = Camera.GetViewMatrix();

	for (auto it = GraphicObjects.begin(); it != GraphicObjects.end(); ++it) {
		int k = it->getID();
		if (ObjectsState[k].UBOIndexUpdate)
		{
			updatePerObjectBlock(ObjectsState[k].UBOIndex, *it);
		}
		else
		{
			glBindBuffer(GL_UNIFORM_BUFFER, ObjectsState[k].UBOIndex);
			glBindBufferBase(GL_UNIFORM_BUFFER, 1, ObjectsState[k].UBOIndex);
		}
		CResourceManager::Instance().GetTexture(it->getTexture())->Apply(GL_TEXTURE0);
		Shader[DIFFUSE_TEXTURE_SHADER_ID].SetUniform1i("tex", 0);
		CMesh *mesh = CResourceManager::Instance().GetMesh(it->getMesh());
		if (mesh != nullptr)
		{
			mesh->Render();
		}
	}
	Shader[DIFFUSE_TEXTURE_SHADER_ID].Deactivate();
}

void CRenderManager::finish(void)
{
	SkyBoxRender();
	ObjectsRender();
	if (AABBRenderEnable) {
		AABBRender();
	}
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	if (CurMSAA != MSAAMode::None)
	{
		FBO_BUFFERS[CurMSAA].resolveToFbo(FBO_BUFFERS[MSAAMode::None]);
	}

	CFBO::unbind();
	Shader[CUR_POST].Activate();
	FBO_BUFFERS[MSAAMode::None].bindColorTexture();
	rectangle.Render();
	Shader[CUR_POST].Deactivate();
	glutSwapBuffers();
}

void CRenderManager::initShader(void)
{
	Shader[SKYBOX_SHADER_ID].LoadFragmentShader((char*)"SHADER\\SkyBox.fsh");
	Shader[SKYBOX_SHADER_ID].LoadVertexShader((char*)"SHADER\\SkyBox.vsh");
	Shader[SKYBOX_SHADER_ID].Link();
	Shader[DIFFUSE_TEXTURE_SHADER_ID].LoadVertexShader((char*)"SHADER\\UsingShader.vsh");
	Shader[DIFFUSE_TEXTURE_SHADER_ID].LoadFragmentShader((char*)"SHADER\\UsingShader.fsh");
	Shader[DIFFUSE_TEXTURE_SHADER_ID].Link();
	ShaderProgram = Shader[DIFFUSE_TEXTURE_SHADER_ID].GetProgram();
	glGenBuffers(1, &PerSceneBlockBuffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, PerSceneBlockBuffer);
	glUniformBlockBinding(ShaderProgram, glGetUniformBlockIndex(ShaderProgram, "PerSceneBlock"), 0);
	glUniformBlockBinding(ShaderProgram, glGetUniformBlockIndex(ShaderProgram, "PerObjectBlock"), 1);
	Shader[AABB_SHADER_ID].LoadVertexShader("SHADER\\BoundingBox.vsh");
	Shader[AABB_SHADER_ID].LoadFragmentShader("SHADER\\BoundingBox.fsh");
	Shader[AABB_SHADER_ID].Link();
	Shader[POST_NORMAL_ID].LoadFragmentShader((char*)"SHADER\\SimplePostProcessing.fsh");
	Shader[POST_NORMAL_ID].LoadVertexShader((char*)"SHADER\\SimplePostProcessing.vsh");
	Shader[POST_NORMAL_ID].Link();
	Shader[POST_SEPIA_ID].LoadFragmentShader((char*)"SHADER\\SepiaPostProcessing.fsh");
	Shader[POST_SEPIA_ID].LoadVertexShader((char*)"SHADER\\SepiaPostProcessing.vsh");
	Shader[POST_SEPIA_ID].Link();
	Shader[POST_GREY_ID].LoadFragmentShader((char*)"SHADER\\GreyPostProcessing.fsh");
	Shader[POST_GREY_ID].LoadVertexShader((char*)"SHADER\\GreyPostProcessing.vsh");
	Shader[POST_GREY_ID].Link();
	CurMSAA = MSAAMode::X4;
	CUR_POST = SHADER::POST_NORMAL_ID;
	rectangle.LoadRect();
}

void CRenderManager::updatePerSceneBlock(int UBOindex)
{
	UboUpdateCount++;
	mat4 ViewMatrix = Camera.GetViewMatrix();

	PerSceneBlock temp =
	{
		Camera.GetProjectionMatrix(),
		Light.GetAmbient(),
		Light.GetDiffuse(),
		Light.GetSpecular(),
		ViewMatrix * Light.GetPosition()
	};
	glBindBuffer(GL_UNIFORM_BUFFER, UBOindex);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PerSceneBlock), &temp, GL_STATIC_DRAW);
}

void CRenderManager::updatePerObjectBlock(int UBOindex, CGraphicObject& GraphicObject)
{
	UboUpdateCount++;
	int k = GraphicObject.getID();
	mat4 ViewMatrix = Camera.GetViewMatrix();
	vec4 color(0.6, 0.6, 0.6, 1.0);
	PerObjectBlock temp =
	{
		ViewMatrix * (GraphicObject.getModelMatrix()),
		GraphicObject.getMaterial().GetAmbient()*color,
		GraphicObject.getMaterial().GetDiffuse()*color,
		GraphicObject.getMaterial().GetSpecular()*color
	};
	glBindBuffer(GL_UNIFORM_BUFFER, ObjectsState[k].UBOIndex);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, ObjectsState[k].UBOIndex);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PerObjectBlock), &temp, GL_STATIC_DRAW);
	ObjectsState[k].UBOIndexUpdate = false;
}

long int& CRenderManager::getUboUpdateCount()
{
	return UboUpdateCount;
}

void CRenderManager::ChangeAABBRenderMode()
{
	AABBRenderEnable = !AABBRenderEnable;
}

void CRenderManager::AABBRender()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	Shader[AABB_SHADER_ID].Activate();
	Shader[AABB_SHADER_ID].SetUniformMat4("ProjectionMatrix", Camera.GetProjectionMatrix());
	Shader[AABB_SHADER_ID].SetUniformVec4("Color", vec4(1, 0, 0, 1));
	for (auto it = GraphicObjects.begin(); it < GraphicObjects.end(); it++) {
		vec3 AABB = it->getAABB();
		auto scale = glm::mat4(
			glm::vec4(AABB.x, 0, 0, 0),
			glm::vec4(0, AABB.y, 0, 0),
			glm::vec4(0, 0, AABB.z, 0),
			glm::vec4(0, 0, 0, 1)
		);
		glm::mat4 ModelViewMatrix = Camera.GetViewMatrix() * it->getModelMatrix() * scale;
		Shader[AABB_SHADER_ID].SetUniformMat4("ModelViewMatrix", ModelViewMatrix);
		CMesh* mesh = CResourceManager::Instance().GetMesh(AABBMeshId);
		if (mesh != nullptr) mesh->Render();
	}
}

void CRenderManager::InitFBOBuffers(int width, int height)
{
	for (int i = 0; i < MSAAMode::mAmount; i++)
	{
		FBO_BUFFERS[i].init(width, height, std::pow(2, i));
	}
}

void CRenderManager::ScaleMSAA()
{
	CurMSAA = (MSAAMode)((CurMSAA + 1) % (int)MSAAMode::mAmount);
}

int CRenderManager::GetMSAAstate()
{
	return CurMSAA;
}

int CRenderManager::GetPost()
{
	return CUR_POST;
}

void CRenderManager::ScalePost()
{
	switch (CUR_POST)
	{
		case SHADER::POST_NORMAL_ID:
			CUR_POST = SHADER::POST_SEPIA_ID;
			break;
		case SHADER::POST_SEPIA_ID:
			CUR_POST = SHADER::POST_GREY_ID;
			break;
		case SHADER::POST_GREY_ID:
			CUR_POST = SHADER::POST_NORMAL_ID;
	}
}
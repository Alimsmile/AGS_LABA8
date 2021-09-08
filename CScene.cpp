#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "CScene.h"
#include "Omp.h"
#include "CTexture.h"
#include "iostream"
POINT CurPos, OldPos;
bool FClick;
//дистанции для объектов
std::vector<float> max_distances{
	600, // object_type = 0
	600, // object_type = 1
	600, // object_type = 2
	300, // object_type = 3
	250, // object_type = 4
	250, // object_type = 5
	150, // object_type = 6
	150, // object_type = 7
};

using namespace std;

void CScene::init(void)
{
	CUR_OPT = Optim::Frustum;
	initserver();
	pugi::xml_parse_result result = resources_description.load_file("Resources.xml");

	CRenderManager::Instance().initShader();
	//Источник света
	vec4 temp = vec4(1.0, 1.0, 1.0, 0.0);
	Light.SetAmbient(temp);
	Light.SetDiffuse(temp);
	Light.SetSpecular(temp);
	temp = vec4(-8, 9, 3, 0.0);
	Light.SetPosition(temp);
}

void CScene::initserver(void)
{
	NetProtocol.init("127.0.0.1", 27000);
	transactionID = 0;
	// запрос приветственного сообщения (код запроса - 0)
	std::string str = NetProtocol.getMessage();
	std::cout << str << std::endl;
}

void CScene::simulate(float sec)
{
	if (GetAsyncKeyState(VK_RBUTTON))
	{
		GetCursorPos(&CurPos);
		if (FClick)
		{
			OldPos = CurPos;
			FClick = false;
		}
		long HAngle = CurPos.x - OldPos.x;
		long VAngle = CurPos.y - OldPos.y;
		OldPos = CurPos;
		Camera.Rotate(HAngle, VAngle);
	}
	else FClick = true;
	if (GetAsyncKeyState(VK_UP))
		Camera.MoveOXZ(+1 * sec, 0);
	if (GetAsyncKeyState(VK_DOWN))
		Camera.MoveOXZ(-1 * sec, 0);
	if (GetAsyncKeyState(VK_RIGHT))
		Camera.MoveOXZ(0, +1 * sec);
	if (GetAsyncKeyState(VK_LEFT))
		Camera.MoveOXZ(0, -1 * sec);
	transactionID++;

	std::vector<SGameObjectDescription> description=NetProtocol.getNearbyObjects(Camera.GetPos(), 35);
	for (auto it = description.begin(); it != description.end(); it++) {
		auto object = CreateGraphicObject(it->Model);
		object.setID(it->ObjectID);
		object.setPosition(glm::vec3(it->px, it->py, it->pz));
		object.setRotation(it->Yaw);
		object.setAABB(glm::vec3(it->aabbx, it->aabby, it->aabbz));
		object.setObjType(it->ObjectType);
		GraphicObjects[it->ObjectID] = object;
	}
	GrCount = GraphicObjects.size();
}

void CScene::draw(void)
{
	CRenderManager::Instance().setLight(Light);
	CRenderManager::Instance().setCamera(&Camera);
	if (CUR_OPT!=Optim::NONE)
	{
		frustumCulling();
	}
	else
	{
		#pragma omp parallel for num_threads(4)
		for (auto it = GraphicObjects.begin(); it != GraphicObjects.end(); ++it)
		{
			CGraphicObject temp = it->second;
			CRenderManager::Instance().AddToRenderQueue(it->second);
		}
		GrSend = GraphicObjects.size();
	}
}

CCamera* CScene::getCamera()
{
	return &Camera;
}

CGraphicObject CScene::CreateGraphicObject(std::string name)
{
	pugi::xml_node models = resources_description.child("Resources").child("Models");
	pugi::xml_node model = models.find_child_by_attribute("id", name.c_str());
	CGraphicObject object;
	object.setMesh(CResourceManager::Instance().LoadMesh(model.child("Mesh").attribute("path").value()));
	CMaterial material;
	pugi::xml_node phongParameters = model.child("Material").child("PhongParameters");
	pugi::xml_node texture = model.child("Material").child("Texture");
	object.setTexture(CResourceManager::Instance().LoadTexture(texture.attribute("path").value()));
	float Diffuse[3];
	sscanf_s(phongParameters.attribute("diffuse").value(), "%f %f %f", &Diffuse[0], &Diffuse[1], &Diffuse[2]);
	vec4 tempvec = vec4(Diffuse[0], Diffuse[1], Diffuse[2], 1.0f);
	material.SetDiffuse(tempvec);

	float Ambient[3];
	sscanf_s(phongParameters.attribute("ambient").value(), "%f %f %f", &Ambient[0], &Ambient[1], &Ambient[2]);
	tempvec = vec4(Ambient[0], Ambient[1], Ambient[2], 1.0f);
	material.SetAmbient(tempvec);

	float Specular[3];
	sscanf_s(phongParameters.attribute("specular").value(), "%f %f %f", &Specular[0], &Specular[1], &Specular[2]);
	tempvec = vec4(Specular[0], Specular[1], Specular[2], 1.0f);
	material.SetSpecular(tempvec);

	material.SetShininess(std::stof(phongParameters.attribute("shininess").value()));

	object.setMaterial(material);
	return object;
}

void CScene::frustumCulling() {
	int count = 0;
	#pragma omp parallel for num_threads(4)
	for (auto it = GraphicObjects.begin(); it != GraphicObjects.end(); it++) {
		if (CUR_OPT == Optim::Frustum_LOD)
		{
			auto position = Camera.GetEyePos();
			auto distance = glm::distance(position, it->second.getPositon());
			if (distance <= max_distances[it->second.getType()])
			{
				// матрица преобразования в систему нормализованных координат устройств
				glm::mat4 PVM = Camera.GetProjectionMatrix() *
					Camera.GetViewMatrix() *
					it->second.getModelMatrix();
				auto AABB = it->second.getAABB();
				std::vector<glm::vec4> AABBVertices = {
					glm::vec4(AABB[0],  AABB[1],  AABB[2], 1.0),
					glm::vec4(AABB[0],  AABB[1], -AABB[2], 1.0),
					glm::vec4(AABB[0], -AABB[1],  AABB[2], 1.0),
					glm::vec4(AABB[0], -AABB[1], -AABB[2], 1.0),
					glm::vec4(-AABB[0],  AABB[1],  AABB[2], 1.0),
					glm::vec4(-AABB[0],  AABB[1], -AABB[2], 1.0),
					glm::vec4(-AABB[0], -AABB[1],  AABB[2], 1.0),
					glm::vec4(-AABB[0], -AABB[1], -AABB[2], 1.0)
				};
				bool allright = true;
				bool allleft = true;
				bool allup = true;
				bool alldown = true;
				bool allfront = true;
				bool allback = true;
#pragma omp paraller for num_threads(4)
				for (auto it2 = AABBVertices.begin(); it2 != AABBVertices.end(); it2++) {
					*it2 = PVM * (*it2);
					*it2 = (*it2) / it2->w;
					if (it2->x < 1) allright = false;
					if (it2->x > -1) allleft = false;
					if (it2->y < 1) allup = false;
					if (it2->y > -1) alldown = false;
					if (it2->z < 1)	allfront = false;
					if (it2->z > -1) allback = false;
				}
				if (!allright && !allleft && !allup && !alldown && !allfront && !allback) {
							count++;
							CRenderManager::Instance().AddToRenderQueue(it->second);
				}
			}
		}
		else
		{
			// матрица преобразования в систему нормализованных координат устройств
			glm::mat4 PVM = Camera.GetProjectionMatrix() *
				Camera.GetViewMatrix() *
				it->second.getModelMatrix();
			auto AABB = it->second.getAABB();
			std::vector<glm::vec4> AABBVertices = {
				glm::vec4(AABB[0],  AABB[1],  AABB[2], 1.0),
				glm::vec4(AABB[0],  AABB[1], -AABB[2], 1.0),
				glm::vec4(AABB[0], -AABB[1],  AABB[2], 1.0),
				glm::vec4(AABB[0], -AABB[1], -AABB[2], 1.0),
				glm::vec4(-AABB[0],  AABB[1],  AABB[2], 1.0),
				glm::vec4(-AABB[0],  AABB[1], -AABB[2], 1.0),
				glm::vec4(-AABB[0], -AABB[1],  AABB[2], 1.0),
				glm::vec4(-AABB[0], -AABB[1], -AABB[2], 1.0)
			};
			bool allright = true;
			bool allleft = true;
			bool allup = true;
			bool alldown = true;
			bool allfront = true;
			bool allback = true;
#pragma omp paraller for num_threads(4)
			for (auto it2 = AABBVertices.begin(); it2 != AABBVertices.end(); it2++) {
				*it2 = PVM * (*it2);
				*it2 = (*it2) / it2->w;
				if (it2->x < 1) allright = false;
				if (it2->x > -1) allleft = false;
				if (it2->y < 1) allup = false;
				if (it2->y > -1) alldown = false;
				if (it2->z < 1)	allfront = false;
				if (it2->z > -1) allback = false;
			}
			if (!allright && !allleft && !allup && !alldown && !allfront && !allback) {
				count++;
				CRenderManager::Instance().AddToRenderQueue(it->second);
			}
		}
		
	}
	GrSend = count;
}

void CScene::ChangeOptimize()
{
	switch (CUR_OPT)
	{
	case Optim::NONE:
		CUR_OPT = Optim::Frustum;
		break;
	case Optim::Frustum:
		CUR_OPT = Optim::Frustum_LOD;
		break;
	case Optim::Frustum_LOD:
		CUR_OPT = Optim::NONE;
		break;
	}
}
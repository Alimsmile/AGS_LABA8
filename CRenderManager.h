#pragma once
#include "CLight.h"
#include "CShader.h"
#include "CCamera.h"
#include "CGraphicObject.h"
#include "CResourceManager.h"
#include "CFBO.h"
#include "Omp.h"
#include <vector>
#include <map>

struct SRenderManagerObjectState {
	CGraphicObject GraphicObject;
	bool UBOIndexUpdate = true;
	GLuint UBOIndex;
};

enum MSAAMode
{
	None,
	X2,
	X4,
	X8,
	mAmount
};
enum SHADER { DIFFUSE_TEXTURE_SHADER_ID = 0,
	SKYBOX_SHADER_ID, 
	AABB_SHADER_ID, POST_NORMAL_ID,
	POST_SEPIA_ID, POST_GREY_ID, 
	MAX_SHADER_ID };

class CRenderManager
{
private:
	
	//используемый шейдер
	CShader Shader[MAX_SHADER_ID];
	GLuint ShaderProgram;
	CCamera Camera;
	CLight Light;
	std::vector<CGraphicObject> GraphicObjects;
	std::map<GLuint, SRenderManagerObjectState> ObjectsState;
	GLuint PerSceneBlockBuffer;
	long int UboUpdateCount=0;
	unsigned int SkyBoxMeshId = CResourceManager::Instance().LoadSkyBoxMesh();
	unsigned int SkyBoxTextureId = CResourceManager::Instance().LoadSkyBoxTexture();
	// переменные дл€ вывода AABB (индекс меша и режим вывода AABB)
	unsigned int AABBMeshId = SkyBoxMeshId;
	bool AABBRenderEnable = false;
	MSAAMode CurMSAA;
	SHADER CUR_POST;
	CMesh rectangle;
	CFBO FBO_BUFFERS[MSAAMode::mAmount];
private:
	CRenderManager() {};
	CRenderManager(const CRenderManager& root) = delete;
	CRenderManager& operator=(const CRenderManager&) = delete;
public:
	static CRenderManager& Instance()
	{
		static CRenderManager RenderManager;
		return RenderManager;
	}
	bool GetAABBRenderState() { return AABBRenderEnable; }

	void ScaleMSAA();
	void ScalePost();
	void ChangeAABBRenderMode();
	//начало вывода новго кадра
	void start(void);
	//утснаовка используемой камеры
	void setCamera(CCamera* Camera);
	//установка используемого источника света
	void setLight(CLight& Light);
	//ƒобавление в очередь рендеринга очередного объекта дл€ вывода
	void AddToRenderQueue(CGraphicObject& GraphicObject);

	long int& getUboUpdateCount();

	//завершение вывода кадра
	void finish(void);
	void initShader(void);

	void updatePerSceneBlock(int UBOindex);
	void updatePerObjectBlock(int UBOindex, CGraphicObject& GraphicObject);

	void SkyBoxRender(void);
	void ObjectsRender(void);
	// вывод AABB всех объектов
	void AABBRender(void);
	void InitFBOBuffers(int width, int height);
	int GetMSAAstate();
	int GetPost();
};
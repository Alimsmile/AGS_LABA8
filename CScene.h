#pragma once

#include "glew.h"
#include <WinSock2.h>
#include <Windows.h>
#include "CCamera.h"
#include "CLight.h"
#include "CShader.h"
#include "CGraphicObject.h"
#include "pugixml.hpp"
#include "CResourceManager.h"
#include "CRenderManager.h"
#include "CNetProtocol.h"

#pragma comment(lib,"ws2_32.lib")

enum Optim
{
	NONE, Frustum, Frustum_LOD
};

// КЛАСС ДЛЯ РАБОТЫ СО СЦЕНОЙ
class CScene
{
private:
	CNetProtocol NetProtocol;
	// камера
	CCamera Camera;
	// источник света
	CLight Light;
	// все графические объекты
	std::map<int,CGraphicObject> GraphicObjects;
	// xml‐документ в котором приводится описание всех объектов
	// чтобы не загружать xml‐файл каждый раз, он загружается на этапе инициализации
	pugi::xml_document resources_description;
	long int UboUpdateCount = 0;
	unsigned short transactionID = 0;
	SOCKET s;
	unsigned int GrCount=0;
	unsigned int GrSend = 0;
	Optim CUR_OPT;
public:
	int GetObjCount() { return GrCount; }
	int GetSendObjCount() { return GrSend; }
	int GetOptState() { return CUR_OPT; }
	// Инициализация сцены
	void init(void);
	// Симуляция сцены
	void simulate(float sec);
	// Вывод всей сцены
	void draw(void);
	// Получение камеры для модификации её в некоторых функциях обратного вызова
	// например: reshape или mouseWheel
	CCamera* getCamera(void);
	void initserver();
	void frustumCulling();
	void ChangeOptimize();
private:
	// создание объекта, используя описание модели из файла "Resources.xml"
	CGraphicObject CreateGraphicObject(std::string name);
};
#pragma once

#include "glew.h"
#include <windows.h>
#include "stdio.h"
#include "GL/freeglut.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "CMesh.h"
#include "CTexture.h"
#include "CCubeTexture.h"
#include <map>

class CResourceManager
{
private:
	//Счетчик мешей
	int meshCount;
	//Счетчик текстур
	int texCount;
	//вектор для хранения всех мешей
	std::vector<CMesh*> meshes;
	std::vector<CTexture*> textures;
	std::map<std::string, int> LoadedMeshes;
	std::map<std::string, int> LoadedTextures;

	CResourceManager(void);
	CResourceManager(const CResourceManager& root) = delete;
	CResourceManager& operator=(const CResourceManager&) = delete;
public:
	//загрузка скайбокса
	int LoadSkyBoxMesh(void);
	int LoadSkyBoxTexture(void);
	//загрузка одного меша
	//Возвращаемое значение: индекс меша в менеджере ресурсов.
	int LoadMesh(std::string filename);
	
	//загрузка текстуры
	//Возвращаемое значение: индекс текстуры в менеджере ресурсов
	int LoadTexture(std::string filename);
	//Получение меша по его индексу
	//Если такого меша нет - возвращается nullptr
	CMesh* GetMesh(int index);
	CTexture* GetTexture(int index);

	static CResourceManager& Instance()
	{
		static CResourceManager ResourceManager;
		return ResourceManager;
	}
};
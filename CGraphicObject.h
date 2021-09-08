#pragma once

#include "CLight.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CTexture.h"


// КЛАСС ДЛЯ РАБОТЫ С ГРАФИЧЕСКИМ ОБЪЕКТОМ
class CGraphicObject
{
private:
	GLuint ID;
	// используемый меш (индекс в менеджере ресурсов)
	int meshID;
	// используемый материал
	CMaterial material;
	//texture index
	int textureID;
	GLuint ObjType;
	// матрица модели (задает позицию и ориентацию)
	glm::mat4 modelMatrix;
	glm::vec3 AABB;
	glm::vec3 Pos;
public:
	// конструктор по умолчанию
	CGraphicObject(void);
	void setAABB(glm::vec3 aabb);
	glm::vec3 getAABB(void);
	// установка внутренних полей
	void setObjType(GLuint type);
	void setMesh(int meshID);
	void setTexture(int textureID);
	void setMaterial(CMaterial& material);
	void setPosition(glm::vec3 position);
	glm::vec3 getPositon() { return Pos; }
	void setRotation(float degree);
	void setID(unsigned int ID);
	// получение различных параметров
	int getMesh(void);
	int getTexture(void);
	GLuint& getID(void);
	GLuint& getType(void);
	CMaterial& getMaterial(void);
	glm::mat4 getModelMatrix(void);
	bool operator==(CGraphicObject &a);
	bool operator!=(CGraphicObject &a);
};
#pragma once

#include "CLight.h"
#include "CMaterial.h"
#include "CMesh.h"
#include "CTexture.h"


// ����� ��� ������ � ����������� ��������
class CGraphicObject
{
private:
	GLuint ID;
	// ������������ ��� (������ � ��������� ��������)
	int meshID;
	// ������������ ��������
	CMaterial material;
	//texture index
	int textureID;
	GLuint ObjType;
	// ������� ������ (������ ������� � ����������)
	glm::mat4 modelMatrix;
	glm::vec3 AABB;
	glm::vec3 Pos;
public:
	// ����������� �� ���������
	CGraphicObject(void);
	void setAABB(glm::vec3 aabb);
	glm::vec3 getAABB(void);
	// ��������� ���������� �����
	void setObjType(GLuint type);
	void setMesh(int meshID);
	void setTexture(int textureID);
	void setMaterial(CMaterial& material);
	void setPosition(glm::vec3 position);
	glm::vec3 getPositon() { return Pos; }
	void setRotation(float degree);
	void setID(unsigned int ID);
	// ��������� ��������� ����������
	int getMesh(void);
	int getTexture(void);
	GLuint& getID(void);
	GLuint& getType(void);
	CMaterial& getMaterial(void);
	glm::mat4 getModelMatrix(void);
	bool operator==(CGraphicObject &a);
	bool operator!=(CGraphicObject &a);
};
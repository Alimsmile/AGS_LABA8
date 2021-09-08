#include "CGraphicObject.h"
GLuint IdCount = 0;

CGraphicObject::CGraphicObject()
{
	ObjType = 0;
	IdCount++;
	ID = IdCount;
	meshID = 0;
	material = CMaterial();
	modelMatrix = glm::mat4(
		glm::vec4(1, 0, 0, 0), // 1‐ый столбец: направление оси ox
		glm::vec4(0, 1, 0, 0), // 2‐ой столбец: направление оси oy
		glm::vec4(0, 0, 1, 0), // 3‐ий столбец: направление оси oz
		glm::vec4(0, 0, 0, 1));
}

GLuint& CGraphicObject::getID(void)
{
	return ID;
}

void CGraphicObject::setMaterial(CMaterial& material)
{
	this->material = material;
}

void CGraphicObject::setObjType(GLuint type)
{
	ObjType = type;
}

GLuint& CGraphicObject::getType()
{
	return ObjType;
}

void CGraphicObject::setID(unsigned int ID)
{
	this->ID = ID;
}

void CGraphicObject::setMesh(int meshID)
{
	this->meshID = meshID;
}

void CGraphicObject::setPosition(glm::vec3 position)
{
	Pos = position;
	modelMatrix = glm::mat4(
		glm::vec4(1, 0, 0, 0), // 1‐ый столбец: направление оси ox
		glm::vec4(0, 1, 0, 0), // 2‐ой столбец: направление оси oy
		glm::vec4(0, 0, 1, 0), // 3‐ий столбец: направление оси oz
		glm::vec4(position.x, position.y, position.z, 1));
}

void CGraphicObject::setRotation(float degree)
{
	glm::mat4 rotateMatrix = glm::mat4(
		glm::vec4(cos(glm::radians(degree)), 0.0f, sin(glm::radians(degree)), 0),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(-sin(glm::radians(degree)), 0, cos(glm::radians(degree)), 0),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
	modelMatrix = modelMatrix * rotateMatrix;
}

int CGraphicObject::getMesh(void)
{
	return meshID;
}

int CGraphicObject::getTexture(void)
{
	return textureID;
}

CMaterial& CGraphicObject::getMaterial(void)
{
	return material;
}

glm::mat4 CGraphicObject::getModelMatrix(void)
{
	return modelMatrix;
}

bool CGraphicObject::operator==(CGraphicObject &a)
{
	if ((this->meshID == a.meshID) & (this->modelMatrix == a.modelMatrix) & (this->ID == a.ID) & (this->material == a.material))
	{
		return true;
	}
	else return false;
}

bool CGraphicObject::operator!=(CGraphicObject &a)
{
	if ((this->meshID == a.meshID) & (this->modelMatrix == a.modelMatrix) & (this->ID == a.ID) & (this->material == a.material))
	{
		return false;
	}
	else return true;
}

void CGraphicObject::setTexture(int textureID)
{
	this->textureID = textureID;
}

glm::vec3 CGraphicObject::getAABB() {
	return AABB;
}

void CGraphicObject::setAABB(glm::vec3 aabb) {
	this->AABB = aabb;
}
#include "CMaterial.h"

CMaterial::CMaterial(void)
{
	ambient = glm::vec4(0.0, 0.0, 0.0, 0.0);
	diffuse = glm::vec4(0.0, 0.0, 0.0, 0.0);
	specular = glm::vec4(0.0, 0.0, 0.0, 0.0);
	shininess = (float) 0.0;
}

glm::vec4 CMaterial::GetAmbient()
{
	return ambient;
}

glm::vec4 CMaterial::GetDiffuse()
{
	return diffuse;
}

glm::vec4 CMaterial::GetSpecular()
{
	return specular;
}

float CMaterial::GetShininess()
{
	return shininess;
}

void CMaterial::SetAmbient(glm::vec4 ambient)
{
	this->ambient = ambient;
}

void CMaterial::SetDiffuse(glm::vec4 diffuse)
{
	this->diffuse = diffuse;
}

void CMaterial::SetShininess(float shininess)
{
	this->shininess = shininess;
}

void CMaterial::SetSpecular(glm::vec4 specular)
{
	this->specular = specular;
}

bool CMaterial::operator==(CMaterial &a)
{
	if ((this->ambient == a.ambient)&(this->diffuse == a.diffuse)&(this->shininess == a.shininess)&(this->specular == a.specular))
		return true;
	else return false;
}

bool CMaterial::operator!=(CMaterial &a)
{
	if ((this->ambient == a.ambient)&(this->diffuse == a.diffuse)&(this->shininess == a.shininess)&(this->specular == a.specular))
		return false;
	else return true;
}
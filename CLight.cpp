#include "CLight.h"

CLight::CLight(void)
{
	position = glm::vec4(0.0, 0.0, 0.0, 0.0);
	ambient = glm::vec4(0.0, 0.0, 0.0, 0.0);
	diffuse = glm::vec4(0.0, 0.0, 0.0, 0.0);
	specular = glm::vec4(0.0, 0.0, 0.0, 0.0);
}

void CLight::SetAmbient(glm::vec4 ambient)
{
	this->ambient = ambient;
}

void CLight::SetDiffuse(glm::vec4 diffuse)
{
	this->diffuse = diffuse;
}

void CLight::SetPosition(glm::vec4 position)
{
	this->position = position;
}

void CLight::SetSpecular(glm::vec4 specular)
{
	this->specular = specular;
}

glm::vec4 CLight::GetAmbient()
{
	return ambient;
}

glm::vec4 CLight::GetDiffuse()
{
	return diffuse;
}

glm::vec4 CLight::GetPosition()
{
	return position;
}

glm::vec4 CLight::GetSpecular()
{
	return specular;
}
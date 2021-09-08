#pragma once
#include <windows.h>
#include "stdio.h"
#include "glew.h"
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
#include <map>

class CMaterial
{
protected:
	//различные составляющие материала
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	GLfloat shininess;
public:
	CMaterial(void);

	void SetAmbient(glm::vec4 ambient);
	void SetDiffuse(glm::vec4 diffuse);
	void SetSpecular(glm::vec4 specular);
	void SetShininess(float shininess);

	glm::vec4 GetAmbient();
	glm::vec4 GetDiffuse();
	glm::vec4 GetSpecular();
	float GetShininess();

	bool operator==(CMaterial &a);
	bool operator!=(CMaterial &a);
};
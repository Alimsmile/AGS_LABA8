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

struct Face
{
	int v[3], vt[3], vn[3];
};

struct Vertex
{
	// Геометрические координаты вершины
	GLfloat vPosition[3];
	// Нормали к поверхности в данной точке
	GLfloat vNormal[3];
	// Текстурные координаты вершины
	GLfloat vTexCoord[2];
};

class CMesh
{
private:
	//индекс VAO буффера
	GLuint VAO_Index;
	//индекс VBO буффера
	GLuint VBO_Index;
	//колчиество вершин
	int VertexCount;
public:
	CMesh(void);
	int LoadOBJ(const char* filename);
	void LoadRect();
	void Render(void);
};
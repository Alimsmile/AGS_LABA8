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
	// �������������� ���������� �������
	GLfloat vPosition[3];
	// ������� � ����������� � ������ �����
	GLfloat vNormal[3];
	// ���������� ���������� �������
	GLfloat vTexCoord[2];
};

class CMesh
{
private:
	//������ VAO �������
	GLuint VAO_Index;
	//������ VBO �������
	GLuint VBO_Index;
	//���������� ������
	int VertexCount;
public:
	CMesh(void);
	int LoadOBJ(const char* filename);
	void LoadRect();
	void Render(void);
};
#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include "glew.h"
#include "GL/freeglut.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//����� ��� ������ � �������� FBO
class CFBO
{
	//������ ������� fbo
	GLuint FBO_Index;
	GLuint Color_tex;
	GLuint Depth_tex;
	static int Width;
	static int Height;

	int Samples;
public:
	CFBO();
	void init(int w, int h, int samples = 1);
	void bind(void);
	static void unbind(void);

	void resolveToFbo(const CFBO& fbo);
	//��������� �������� �����
	void bindColorTexture(GLenum TexUnit = GL_TEXTURE0);
	void bindDepthTexture(GLenum TexUnit = GL_TEXTURE1);
};
#pragma once

#include <windows.h>
#include "stdio.h"
#include "glew.h"
#include <string>
#include <iostream>
#include "GL/freeglut.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <map>

using namespace glm;

// КЛАСС ДЛЯ РАБОТЫ С ШЕЙДЕРОМ
class CShader
{
private:
	std::map<std::string,int> shad;
	// вершинный шейдер
	GLuint	Vsh;
	// фрагментный шейдер
	GLuint	Fsh;
	// шейдерная программа (шейдер)
	GLuint	Program;
	//Колчиестов строк в шейдерах
	GLuint VshStrCount;
	GLuint FshStrCount;
public:
	CShader(void) { Fsh = Program = VshStrCount = Vsh = FshStrCount = 0; };
	char** ReadTextFile(const char* FileName,int &str);
	// загрузить вершинный шейдер
	int LoadVertexShader(const char *VertexFileName, bool DebugOutput = true);
	// загрузить фрагментный шейдер
	int LoadFragmentShader(const char *FragmentFileName, bool DebugOutput = true);
	// слинковать шейдерную программу
	int Link(bool DebugOutput = true);

	//	активизировать шейдер (сделать текущим)
	void Activate(void);
	//	отключить шейдер (использовать нулевую шейдерную программу)
	static void	Deactivate(void);

	// получение индекса атрибут-переменной
	int GetAttribLocation(const char *name)
	{
		return glGetAttribLocation(Program, name);
	};

	GLuint GetProgram();

	// запись вектора из 4-х комопнент в uniform-переменную
	void SetUniformVec4(std::string str, vec4 value)
	{
		int k;
		if (shad[str] != 0)
			k = shad[str];
		else
		{
			k = glGetUniformLocation(Program,str.c_str());
			shad[str] = k;
		}
		if (k < 0) return;
		glUseProgram(Program);
		glUniform4fv(k, 1, value_ptr(value));
	}

	void SetUniformMat4(std::string str, mat4 value)
	{
		int k;
		if (shad[str] != 0)
			k = shad[str];
		else
		{
			k = glGetUniformLocation(Program, str.c_str());
			shad[str] = k;
		}
		if (k < 0) return;
		glUseProgram(Program);
		glUniformMatrix4fv(k, 1, false, value_ptr(value));
	}
	void SetUniformFloat(std::string str, float value)
	{
		int k;
		if (shad[str] != 0)
		{
			k = shad[str];
		}
		else
		{
			k = glGetUniformLocation(Program, str.c_str());
			shad[str] = k;
		}
		if (k < 0) return;
		glUseProgram(Program);
		glUniform1f(k, value);
	}

	void SetUniform1i(std::string str, int value)
	{
		int k;
		if (shad[str] != 0)
		{
			k = shad[str];
		}
		else
		{
			k = glGetUniformLocation(Program, str.c_str());
			shad[str] = k;
		}
		if (k < 0) return;
		glUseProgram(Program);
		glUniform1i(k, value);
	}
};
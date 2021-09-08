#define _CRT_SECURE_NO_WARNINGS
#include "CShader.h"
#include <fstream>
#include <vector>
#include <iostream>


char** CShader::ReadTextFile(const char* FileName,int &str)
{
	int i = 0;
	std::vector<std::string> temp;
	std::string temporaryStr;
	std::ifstream fin(FileName);

	if (fin.is_open())
	{
		while (!fin.eof())
		{
			getline(fin, temporaryStr);
			temp.emplace_back(temporaryStr);
			temp[i] += "\n";
			i++;
		}
	}
	fin.close();

	char **MyCode = new char *[temp.size()];
	for (uint i = 0; i < temp.size(); i++)
	{
		MyCode[i] = new char[temp[i].size() + 1];
		strcpy(MyCode[i], temp[i].c_str());
		str++;
	}
	return MyCode;
}

int CShader::LoadVertexShader(const char* VertexFileName, bool DebugOutput)
{
	Vsh=glCreateShader(GL_VERTEX_SHADER);
	int* temp = new int();
	char ** Vertex_Shader = CShader::ReadTextFile(VertexFileName,*temp);
	VshStrCount = *temp;
	glShaderSource(Vsh, VshStrCount, Vertex_Shader, NULL);
	if (DebugOutput)
	{
		GLint CompileStatus;
		glCompileShader(Vsh);
		glGetShaderiv(Vsh, GL_COMPILE_STATUS, &CompileStatus);
		if (CompileStatus == GL_FALSE)
		{
			char *InfoLog = new char[100];
			GLsizei log_length = 100;
			glGetShaderInfoLog(Vsh, 100, &log_length, InfoLog);
			std::cout << "Compile error(vertex shader)" << std::endl;
			std::cout << InfoLog << std::endl << std::endl;
		}
		else std::cout << "Compile succesful(vertex shader)" << std::endl << std::endl;
	}
	return Vsh;
}

int CShader::LoadFragmentShader(const char* FragmentFileName, bool DebugOutput)
{
	Fsh = glCreateShader(GL_FRAGMENT_SHADER);
	int *temp = new int();
	char ** Fragment_Shader = CShader::ReadTextFile(FragmentFileName,*temp);
	FshStrCount = *temp;
	glShaderSource(Fsh, FshStrCount, Fragment_Shader, NULL);

	if (DebugOutput)
	{
		GLint CompileStatus;
		glCompileShader(Fsh);
		glGetShaderiv(Fsh, GL_COMPILE_STATUS, &CompileStatus);
		if (CompileStatus == GL_FALSE)
		{
			char * InfoLog = new char[100];
			GLsizei log_length = 100;
			glGetShaderInfoLog(Fsh, 100, &log_length, InfoLog);
			std::cout << "Compile error (fragment shader)" << std::endl;
			std::cout << InfoLog << std::endl << std::endl;
		}
		else std::cout << "Compile successful (fragment shader)" << std::endl << std::endl;
	}

	return Fsh;
}

int CShader::Link(bool DebugOutput)
{
	Program = glCreateProgram();
	glAttachShader(Program, Vsh);
	glAttachShader(Program, Fsh);
	glLinkProgram(Program);
	if (DebugOutput)
	{
		GLint LinkStatus;
		glCompileShader(Fsh);
		glGetShaderiv(Fsh, GL_COMPILE_STATUS, &LinkStatus);
		if (LinkStatus == GL_FALSE)
		{
			char * InfoLog = new char[100];
			GLsizei log_length = 100;
			glGetProgramInfoLog(Program, 100, &log_length, InfoLog);
			std::cout << "Link error" << std::endl;
			std::cout << InfoLog << std::endl << std::endl;
		}
		else std::cout << "Link successful" << std::endl << std::endl;
	}
	return Program;
}

void CShader::Activate()
{
	glUseProgram(Program);
}

void CShader::Deactivate()
{
	glUseProgram(0);
}

GLuint CShader::GetProgram()
{
	return Program;
}
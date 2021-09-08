#define _CRT_SECURE_NO_WARNINGS
#include "CScene.h"
#include <windows.h>
#include "stdio.h"

#include "glew.h"
#include "GL/freeglut.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "CShader.h"

#include "CRenderManager.h"


using namespace glm;

CScene Scene;
// используемый шейдер (пока только один)
CShader		Shader;

// переменные для вывода объекта (прямоугольника из двух треугольников)
GLuint	VAO_Index = 0;		// индекс VAO-буфера
GLuint	VBO_Index = 0;		// индекс VBO-буфера
int		VertexCount = 0;	// количество вершин
CCamera Camera;
LARGE_INTEGER oldvalue, newvalue, freq;
float sec;
int MeshId[3];
CMesh temp;
CMaterial Material;
double sum_sec = 0;
int fps = 0;
GLchar OutTitle[120];
int MSAA;
std::string Post;
std::string Opt;

void MouseWheel(int wheel, int direction, int x, int y)
{
	Scene.getCamera()->Zoom(direction,sec);
}

// функция вызывается при перерисовке окна
// в том числе и принудительно, по командам glutPostRedisplay
void Display(void)
{
	CRenderManager::Instance().start();

	Scene.draw();
	
	CRenderManager::Instance().finish();
};

// функция, вызываемая при изменении размеров окна
void Reshape(int w, int h)
{
	CRenderManager::Instance().InitFBOBuffers(w, h);
	// установить новую область просмотра, равную всей области окна
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	Scene.getCamera()->SetProjectionMatrix(45.0, (float)w/h, 1, 999999);
};



// функция вызывается когда процессор простаивает, т.е. максимально часто
void Simulation(void)
{
	glutMouseWheelFunc(MouseWheel);
	QueryPerformanceCounter(&newvalue);
	QueryPerformanceFrequency(&freq);
	sec = GLfloat(newvalue.QuadPart - oldvalue.QuadPart) / freq.QuadPart;
	sum_sec += sec;
	fps++;
	if (sum_sec >= 1)
	{
		switch (CRenderManager::Instance().GetMSAAstate())
		{
		case MSAAMode::None: MSAA = 0; break;
		case MSAAMode::X2: MSAA = 2; break;
		case MSAAMode::X4: MSAA = 4; break;
		case MSAAMode::X8: MSAA = 8; break;
		}
		switch (CRenderManager::Instance().GetPost())
		{
		case SHADER::POST_NORMAL_ID: Post = "Normal"; break;
		case SHADER::POST_GREY_ID: Post = "Grey"; break;
		case SHADER::POST_SEPIA_ID: Post = "Sepia"; break;
		}
		switch (Scene.GetOptState())
		{
		case Optim::NONE: Opt = "NONE"; break;
		case Optim::Frustum: Opt = "Frustum"; break;
		case Optim::Frustum_LOD: Opt = "Frustum&LOD"; break;
		}
		sprintf(OutTitle, "Lab 08 | FPS: [%d] [MSAA] x%d;  [Optimize: %s] [Objects: %d/%d] [PostProc: %s]", fps, MSAA, Opt.c_str(),
			Scene.GetSendObjCount(), Scene.GetObjCount(), Post.c_str());
		glutSetWindowTitle(OutTitle);
		sum_sec = 0;
		fps = 0;
	}
	oldvalue = newvalue;
	Scene.simulate(sec);
	glutPostRedisplay();
};


void PressKey(unsigned char key, int x, int y)
{
	if (key == '2')
	{
		CRenderManager::Instance().ScaleMSAA();
	}
	if (key == '3')
	{
		CRenderManager::Instance().ScalePost();
	}
	if (key == '1')
	{
		Scene.ChangeOptimize();
	}
}

void main(int argc, char **argv)
{
	//// инициализация библиотеки DevIL для загрузки изображений
	ilInit();
	iluInit();
	ilutInit();
	// инициализация библиотеки GLUT
	glutInit(&argc, argv);
	// инициализация дисплея (формат вывода)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	// требования к версии OpenGL (версия 4.0 без поддержки обратной совместимости)
	//glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	// устанавливаем верхний левый угол окна
	glutInitWindowPosition(300, 100);
	// устанавливаем размер окна
	glutInitWindowSize(800, 600);
	// создание окна
	glutCreateWindow("laba_08");
	//	инициализация GLEW 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Glew error: %s\n", glewGetErrorString(err));
		return;
	}
	// определение текущей версии OpenGL
	printf("OpenGL Version = %s\n\n", glGetString(GL_VERSION));
	Scene.init();

	CRenderManager::Instance().InitFBOBuffers(800, 600);
	glutKeyboardFunc(PressKey);
	// устанавливаем функцию, которая будет вызываться для перерисовки окна
	glutDisplayFunc(Display);
	// устанавливаем функцию, которая будет вызываться при изменении размеров окна
	glutReshapeFunc(Reshape);
	// устанавливаем функцию которая вызывается всякий раз, когда процессор простаивает
	glutIdleFunc(Simulation);
	// основной цикл обработки сообщений ОС
	glutMainLoop();
	return;
};
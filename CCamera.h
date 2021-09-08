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

const float M_PI=3.14159265358979323846;
const float ANGULAR_SPEED_ROTATION = 90;
const float MOOVING_SPEED = 60;

class CCamera
{
	float X;
	float Z;
	//радиус
	float r;
	//угол по оси xy
	float pitch;
	//угол по оси xz
	float yaw;
	//позиция камеры
	glm::vec3 EyePos;
	//точка, на которую направлена камера
	glm::vec3 FocusOn;
	//вектор вверх
	glm::vec3 Up;
	//матрица проекции
	glm::mat4 ProjectionMatrix;
	//матрица наблюдения
	glm::mat4 ViewMatrix=glm::lookAt(EyePos,FocusOn,Up);
	void UpdateViewmatrix(void);
public:
	CCamera(void);
	~CCamera(void);
	//установка матрицы проекции
	void SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
	//получени матрицы проекции
	glm::mat4 GetProjectionMatrix(void);
	glm::vec3 GetEyePos() { return EyePos+FocusOn; };
	//получить матрицу наблюдения
	glm::mat4 GetViewMatrix(void);
	glm::vec3 GetPos(void);
	//передвинуть камеру по горизонтали
	void MoveOXZ(float dForward,float dRight);
	//вращение в горизонтальной и вертикальной плоскости
	void Rotate(long double dHorizAngle, long double dVertAngle);
	//приближение/удаление
	void Zoom(int dR,float sec);
	bool read_file(const char* namefile);
	bool write_file(const char* namefile);
	bool operator==(CCamera &a);
	bool operator!=(CCamera &a);
};
#include "CCamera.h"
std::string namef = "text.txt";
CCamera::CCamera(void)
{
	if (read_file(namef.c_str()) != 0)
	{
		r = 15;
		yaw = 45.0f; pitch = 45.0f;
		FocusOn = glm::vec3(0.0, 0.0, 0.0);
	}
	EyePos = glm::vec3(r* cos(pitch*M_PI / 180.0f)*(cos(yaw*M_PI / 180.0f))+FocusOn.x,
		r*sin(pitch* M_PI / 180.0f), r*cos(pitch* M_PI / 180.0f)*(sin(yaw* M_PI / 180.0f))+FocusOn.z);
	//точка, на которую направлена камера
	//вектор вверх
	Up=glm::vec3(0, 1, 0);
	ViewMatrix = glm::lookAt(EyePos + FocusOn, FocusOn, Up);
};

CCamera::~CCamera(void)
{};

void CCamera::SetProjectionMatrix(float fovy, float aspect, float zNear, float zFar)
{
	ProjectionMatrix = glm::perspective(fovy, aspect, zNear, zFar);
}

glm::mat4 CCamera::GetViewMatrix(void)
{
	UpdateViewmatrix();
	return ViewMatrix;
}

glm::mat4 CCamera::GetProjectionMatrix(void)
{
	return ProjectionMatrix;
}

void CCamera::Zoom(int dR,float sec)
{
	r+= dR*sec * 10;
	EyePos = glm::vec3(r* cos(pitch*M_PI / 180.0f)*(cos(yaw*M_PI / 180.0f)),
		r*sin(pitch* M_PI / 180.0f), r*cos(pitch* M_PI / 180.0f)*(sin(yaw* M_PI / 180.0f)));
	ViewMatrix = glm::lookAt(EyePos + FocusOn, FocusOn, Up);
}

void CCamera::UpdateViewmatrix(void)
{
	
}

void CCamera::MoveOXZ(float dForward,float dRight)
{
	float tempf = dForward * MOOVING_SPEED;
	float tempr = dRight * MOOVING_SPEED;
	X = FocusOn.x;
	Z = FocusOn.z;
	glm::vec3 VF = glm::normalize(-EyePos);
	glm::vec3 MoveF = glm::vec3(VF.x * tempf, 0, VF.z * tempf);

	glm::vec3 MoveR = glm::normalize(glm::cross(VF, Up));
	MoveR = glm::vec3(MoveR.x * tempr, 0, MoveR.z * tempr);
	FocusOn += MoveF + MoveR;

	ViewMatrix = glm::lookAt(EyePos + FocusOn, FocusOn, Up);
}

void CCamera::Rotate(long double HAngle, long double VAngle)
{
	//if ((VAngle > 0) & (pitch < 85)) pitch += VAngle/5;
	//if ((VAngle < 0) & (pitch > 5)) pitch += VAngle/5;
	if (VAngle < 0) pitch += VAngle / 5;
	if (VAngle > 0) pitch += VAngle / 5;
	yaw += HAngle/5;

	//if (pitch < 5) pitch = 5;
	//if (pitch > 85) pitch = 85;
	EyePos = glm::vec3(r* cos(pitch*M_PI / 180.0f)*(cos(yaw*M_PI / 180.0f)),
		r*sin(pitch* M_PI / 180.0f), r*cos(pitch* M_PI / 180.0f)*(sin(yaw* M_PI / 180.0f)));
	ViewMatrix = glm::lookAt(EyePos + FocusOn, FocusOn, Up);
}

bool CCamera::read_file(const char*namefile)
{
	if (!strcmp(namefile, ""))
		return -1;
	std::ifstream in;
	in.open(namefile);
	if (in.is_open())
	{
		in >> r >> yaw >> pitch>>FocusOn.x>>FocusOn.z;
		in.close();
		std::cout << "read" << std::endl;
		return 0;
	}
	else return -1;
}

bool CCamera::write_file(const char* namefile)
{
	std::ofstream out(namefile);
	out << r << std::endl << yaw << std::endl << pitch<<std::endl
		<< FocusOn.x << std::endl << FocusOn.z;
	out.close();
	return 0;
}

bool CCamera::operator==(CCamera &a)
{
	if ((this->EyePos == a.EyePos) & (this->FocusOn == a.FocusOn) & (this->pitch == a.pitch) & (this->ProjectionMatrix == a.ProjectionMatrix) & (this->r == a.r) & (this->yaw == a.yaw)
		& (this->X == a.X) & (this->Z == a.Z))
	{
		return true;
	}
	else return false;
}

glm::vec3 CCamera::GetPos()
{
	return FocusOn;
}

bool CCamera::operator!=(CCamera &a)
{
	if ((this->FocusOn == a.FocusOn) & (this->pitch == a.pitch) & (this->ProjectionMatrix == a.ProjectionMatrix) & (this->r == a.r) & (this->yaw == a.yaw)
		& (this->X == a.X) & (this->Z == a.Z))
	{
		return false;
	}
	else return true;
}
#include "CCubeTexture.h"
#include <iostream>

void CCubeTexture::Apply(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TexIndex);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	switch (TexFilterMode)
	{
	case 0:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	case 1:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case 2:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case 3:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	case 4:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2);
		break;
	case 5:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
		break;
	case 6:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8);
		break;
	case 7:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);
		break;
	case 8:
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, 32);
		break;
	}
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void CCubeTexture::Load(const char *filename1, const char *filename2, const char *filename3,
	const char *filename4, const char *filename6, const char *filename5)
{
	const char* filename[6];
	filename[0] = filename1; filename[1] = filename2; filename[2] = filename5;
	filename[3] = filename6; filename[4] = filename3; filename[5] = filename4;
	int width[6];
	int height[6];
	int format[6];
	int type[6];
	ILubyte *Data[6];
	ILuint ImageId[6];
	for (int i = 0; i < 6; i++)
	{
		ImageId[i] = ilGenImage();
		ilBindImage(ImageId[i]);
		ilLoadImage(filename[i]);
		int k = ilGetError();
		if (k == IL_NO_ERROR)
		{
			std::cout << "Skybox " << i + 1 << " Texture loaded" << std::endl;
		}
		if (k == IL_COULD_NOT_OPEN_FILE)
		{
			std::cout << "COULD_NOT_OPEN_FILE" << std::endl;
		}
		width[i] = ilGetInteger(IL_IMAGE_WIDTH);
		height[i] = ilGetInteger(IL_IMAGE_HEIGHT);
		format[i] = ilGetInteger(IL_IMAGE_FORMAT);
		type[i] = ilGetInteger(IL_IMAGE_TYPE);
		Data[i] = ilGetData();
	}
	// создание текстурного объекта    
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &TexIndex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, TexIndex);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format[0], width[0], height[0], 0, format[0], type[0], Data[0]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format[1], width[1], height[1], 0, format[1], type[1], Data[1]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format[2], width[2], height[2], 0, format[2], type[2], Data[2]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format[3], width[3], height[3], 0, format[3], type[3], Data[3]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format[4], width[4], height[4], 0, format[4], type[4], Data[4]);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format[5], width[5], height[5], 0, format[5], type[5], Data[5]);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	//Установка параметров адресации текселей
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (int i = 0; i < 6; i++)
	{
		ilDeleteImage(ImageId[i]);
	}
}
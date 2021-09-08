#pragma once
#include "CTexture.h"

class CCubeTexture :public CTexture
{
public:
	void Load(const char* filename1, const char *filename2, const char *filename3,
		const char *filename4, const char *filename5, const char *filename6);
	void Apply(GLenum texUnit = GL_TEXTURE0);
};
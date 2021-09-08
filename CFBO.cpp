#include "CFBO.h"
int CFBO::Width, CFBO::Height;
CFBO::CFBO()
{
	FBO_Index=Color_tex=Depth_tex=0;
}
//инициализация буфера FBO
void CFBO::init(int w,int h,int samples)
{
	if (FBO_Index != 0)
	{
		glDeleteTextures(1, &Color_tex);
		glDeleteTextures(1, &Depth_tex);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteFramebuffers(1, &FBO_Index);
	}
	CFBO::Width = w;
	CFBO::Height = h;
	Samples = samples;
	switch (samples)
	{
	case 1:
		glGenTextures(1, &Color_tex);
		glBindTexture(GL_TEXTURE_2D, Color_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			CFBO::Width, CFBO::Height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

		glGenTextures(1, &Depth_tex);
		glBindTexture(GL_TEXTURE_2D, Depth_tex);
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24,
			CFBO::Width, CFBO::Height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, nullptr);

		glGenFramebuffers(1, &FBO_Index);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO_Index);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Color_tex, 0);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Depth_tex, 0);
		GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
		
	}
	glGenTextures(1, &Color_tex);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, Color_tex);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples,
		GL_RGBA, CFBO::Width, CFBO::Height, false);

	glGenTextures(1, &Depth_tex);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, Depth_tex);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples,
		GL_DEPTH_COMPONENT24, CFBO::Width, CFBO::Height, false);

	glGenFramebuffers(1, &FBO_Index);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_Index);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, Color_tex, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, Depth_tex, 0);
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//выбор в качестве текущего буфера FBO
void CFBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO_Index);
	glViewport(0, 0, CFBO::Width, CFBO::Height);
}
//возврат к буферу кадра по умолчанию
void CFBO::unbind(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, CFBO::Width, CFBO::Height);
}
//копирование данного буфера FBO в другой
void CFBO::resolveToFbo(const CFBO &fbo)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO_Index);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo.FBO_Index);
	glBlitFramebuffer(
		0, 0, CFBO::Width, CFBO::Height,
		0, 0, CFBO::Width, CFBO::Height,
		GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
//привязать текстуру цвета к текстурному блоку
void CFBO::bindColorTexture(GLenum texunit)
{
	glActiveTexture(texunit);
	glBindTexture(GL_TEXTURE_2D, Color_tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
//привязать текстуру глубины к текстурному блоку
void CFBO::bindDepthTexture(GLenum texunit)
{
		glActiveTexture(texunit);
		glBindTexture(GL_TEXTURE_2D, Depth_tex);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}
#pragma once
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "soil/src/SOIL.h"
#include "string.h"

class cTexture
{
	GLuint texHandle;
	char texPath[1024];

	public:
	cTexture(void);
	virtual ~cTexture(void);
	void LoadFromFile(const char *filePathIn);
	GLuint GetTextureHandle();
	const char * GetTexturePath();
};

extern cTexture textures[100];

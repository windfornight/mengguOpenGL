#pragma once
#include "ggl.h"

unsigned char* LoadFileContent(const char* path, int& filesize);
unsigned char* DecodeBMP(unsigned char* bmpFileData, int& width, int& height);
GLuint CreateTexture2D(unsigned char* pixelData, int width, int height, GLenum type);
GLuint CreateTexture2DFromBMP(const char *bmpPath);
GLuint CreateDisplayList(std::function<void()> foo);
float GetFrameTime();
GLuint CreateTexture2DFromPNG(const char *imgFilePath, bool invertY = true);
GLuint CreateProcedureTexture(int size);

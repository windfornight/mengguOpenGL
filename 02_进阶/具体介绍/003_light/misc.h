#pragma once
#include "glew.h"

GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void*data = nullptr);
char *LoadFileContent(const char*path);
GLuint CompileShader(GLenum shaderType,const char*shaderPath);
GLuint CreateGPUProgram(const char*vsShaderPath, const char*fsShaderPath);
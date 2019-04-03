#include "misc.h"
#include <stdio.h>

GLuint CreateBufferObject(GLenum bufferType, GLsizeiptr size, GLenum usage, void*data /* = nullptr */)
{
	GLuint object;
	glGenBuffers(1, &object);
	glBindBuffer(bufferType, object);
	glBufferData(bufferType, size, data, usage);
	glBindBuffer(bufferType, 0);
	return object;
}


char *LoadFileContent(const char*path)
{
	FILE*pFile = fopen(path, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		char*buffer = nullptr;
		if (nLen!=0)
		{
			buffer=new char[nLen + 1];
			rewind(pFile);
			fread(buffer, nLen, 1, pFile);
			buffer[nLen] = '\0';
		}
		else
		{
			printf("load file fail %s content len is 0\n", path);
		}
		fclose(pFile);
		return buffer;
	}
	else
	{
		printf("open file %s fail\n",path);
	}
	fclose(pFile);
	return nullptr;
}


GLuint CompileShader(GLenum shaderType, const char*shaderPath)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader==0)
	{
		printf("glCreateShader fail\n");
		return 0;
	}
	const char* shaderCode = LoadFileContent(shaderPath);
	if (shaderCode==nullptr)
	{
		printf("load shader code from file : %s fail\n",shaderPath);
		glDeleteShader(shader);
		return 0;
	}
	glShaderSource(shader, 1, &shaderCode, nullptr);
	glCompileShader(shader);
	GLint compileResult = GL_TRUE;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&compileResult);
	if (compileResult==GL_FALSE)
	{
		char szLog[1024] = {0};
		GLsizei logLen = 0;
		glGetShaderInfoLog(shader,1024,&logLen,szLog);
		printf("Compile Shader fail error log : %s \nshader code :\n%s\n",szLog,shaderCode);
		glDeleteShader(shader);
		shader = 0;
	}
	delete shaderCode;
	return shader;
}


GLuint CreateGPUProgram(const char*vsShaderPath, const char*fsShaderPath)
{
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, vsShaderPath);
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER,fsShaderPath);
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShader);
	glAttachShader(program, fsShader);
	glLinkProgram(program);
	glDetachShader(program, vsShader);
	glDetachShader(program, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	GLint linkResult = GL_TRUE;
	glGetProgramiv(program,GL_LINK_STATUS,&linkResult);
	if (linkResult==GL_FALSE)
	{
		char szLog[1024] = { 0 };
		GLsizei logLen = 0;
		glGetProgramInfoLog(program, 1024, &logLen, szLog);
		printf("link program fail error log : %s \n vs : %s\n fs : %s\n", szLog, vsShaderPath,fsShaderPath);
		glDeleteProgram(program);
		program = 0;
	}
	return program;
}

GLuint CreateTextureFromFile(const char*imagePath)
{
	unsigned char*imgData = (unsigned char*)LoadFileContent(imagePath);
	if (*((unsigned short*)imgData)!=0x4D42)//bmp
	{
		printf("cannot decode %s\n",imagePath);
		delete imgData;
		return 0;
	}
	//decode bmp
	int pixelDataOffset = *((int*)(imgData + 10));
	int width = *((int*)(imgData + 18));
	int height = *((int*)(imgData + 22));
	unsigned char*pixelData = (imgData + pixelDataOffset);
	for (int i=0;i<width*height*3;i+=3)
	{
		//bgr->rgb
		unsigned char temp = pixelData[i+2];
		pixelData[i+2]= pixelData[i];
		pixelData[i] = temp;
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete imgData;
	return texture;
}
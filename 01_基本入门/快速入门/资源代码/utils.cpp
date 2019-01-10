#include "utils.h"
#include "soil.h"

unsigned  char* LoadFileContent(const char* path, int& filesize)
{
	unsigned char* fileContent = nullptr;
	filesize = 0;
	FILE* pFile = fopen(path, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		if (nLen > 0)
		{
			rewind(pFile);
			fileContent = new unsigned char[nLen + 1];
			fread(fileContent, sizeof(unsigned char), nLen, pFile);
			fileContent[nLen] = '\0';
			filesize = nLen;
		}
		fclose(pFile);
	}
	return fileContent;
}

unsigned char* DecodeBMP(unsigned char* bmpFileData, int& width, int& height)
{ 
	if (0x4D42 == *(unsigned short*)bmpFileData)
	{
		int pixelDataOffset = *((int *)(bmpFileData + 10));
		width = *((int*)(bmpFileData + 18));
		height = *((int*)(bmpFileData + 22));
		unsigned char* pixelData = bmpFileData + pixelDataOffset;
		for (int i = 0; i < width * height * 3; i += 3)  //bgr->rgb(opengl)
		{
			unsigned char temp = pixelData[i];
			pixelData[i] = pixelData[i + 2];
			pixelData[i + 2] = temp;
		}
		return pixelData;
	}
	return nullptr;
}

GLuint CreateTexture2D(unsigned char* pixelData, int width, int height, GLenum type)
{
	GLuint texture;
	glGenTextures(1, &texture); //创建一个纹理描述符
	glBindTexture(GL_TEXTURE_2D, texture);  //绑定当前纹理
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  //GL_NEAREST
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);  //GL_REPEAT
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  //针对天空盒交界处的黑线
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixelData); //第二个0的具体意义
	glBindTexture(GL_TEXTURE_2D, 0); //解绑，防止之后的操作会改变上述纹理
	return texture;
}

GLuint CreateTexture2DFromBMP(const char* bmpPath)
{
	int nFileSize = 0;
	unsigned char* bmpFileContent = LoadFileContent(bmpPath, nFileSize);
	if (bmpFileContent == nullptr)
	{
		return 0;
	}
	int bmpWidth = 0, bmpHeight = 0;
	unsigned char* pixelData = DecodeBMP(bmpFileContent, bmpWidth, bmpHeight);
	if (bmpWidth == 0)
	{
		delete bmpFileContent;
		return 0;
	}
	GLuint texture = CreateTexture2D(pixelData, bmpWidth, bmpHeight, GL_RGB);
	delete bmpFileContent;
	return texture;
}

GLuint CreateDisplayList(std::function<void()> foo)
{
	GLuint displayList = glGenLists(1);
	glNewList(displayList, GL_COMPILE);
	foo();
	glEndList();
	return displayList;
}

GLuint CreateTexture2DFromPNG(const char* imgFilePath, bool invertY)
{
	int nFileSize = 0;
	unsigned char *filecontent = LoadFileContent(imgFilePath, nFileSize);
	if (filecontent == nullptr)
	{
		return 0;
	}
	
	unsigned int flags = SOIL_FLAG_POWER_OF_TWO;
	if (invertY)
	{
		flags |= SOIL_FLAG_INVERT_Y;
	}

	GLuint texture = SOIL_load_OGL_texture_from_memory(filecontent, nFileSize, 0, 0, flags);
	delete filecontent;
	return texture;
}

GLuint CreateProcedureTexture(int size)
{
	unsigned char *imageData = new unsigned char[size * size * 4];
	float halfSize = (float)size / 2.0f;
	float maxDistance = sqrtf(halfSize * halfSize + halfSize * halfSize);
	float centerX = halfSize;
	float centerY = halfSize;


	for (int y = 0; y < size; ++y)
	{
		for (int x = 0; x < size; ++x)
		{
			int currentPixelOffset = (x + y * size) * 4;
			imageData[currentPixelOffset] = 255;
			imageData[currentPixelOffset + 1] = 255;
			imageData[currentPixelOffset + 2] = 255;
			float deltaX = (float)x - centerX;
			float deltaY = (float)y - centerY;
			float distance = sqrtf(deltaX*deltaX + deltaY * deltaY);
			float alpha = pow(1.0f - (distance / maxDistance), 8.0f);
			alpha = (alpha > 1.0f) ? 1.0f : alpha;
			imageData[currentPixelOffset + 3] = (unsigned char)(alpha * 255.0f);
		}
	}
	GLuint texture = CreateTexture2D(imageData, size, size, GL_RGBA);
	delete imageData;
	return texture;
}
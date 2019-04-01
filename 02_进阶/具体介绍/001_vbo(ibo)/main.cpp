#include <windows.h>
#include <stdio.h>
#include "glew.h"
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glu32.lib")

struct Vertex
{
	float pos[3];
	float color[4];
};
LRESULT CALLBACK GLWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}
	return DefWindowProc(hwnd,msg,wParam,lParam);
}

char *LoadFileContent(const char*path)
{
	FILE*pFile = fopen(path, "rb");
	if (pFile)
	{
		fseek(pFile, 0, SEEK_END);
		int nLen = ftell(pFile);
		char*buffer = new char[nLen+1];
		rewind(pFile);
		fread(buffer, nLen, 1, pFile);
		buffer[nLen]='\0';
		fclose(pFile);
		return buffer;
	}
	fclose(pFile);
	return nullptr;
}

GLuint CreateGPUProgram(const char*vsShaderPath, const char*fsShaderPath)
{
	GLuint vsShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fsShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vsCode = LoadFileContent(vsShaderPath);
	const char* fsCode = LoadFileContent(fsShaderPath);
	glShaderSource(vsShader, 1, &vsCode, nullptr);
	glShaderSource(fsShader, 1, &fsCode, nullptr);//ram -> vram
	glCompileShader(vsShader);
	glCompileShader(fsShader);
	GLuint program = glCreateProgram();
	glAttachShader(program, vsShader);
	glAttachShader(program, fsShader);
	glLinkProgram(program);
	glDetachShader(program,vsShader);
	glDetachShader(program, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);
	return program;
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASSEX wndClass;
	wndClass.cbClsExtra = 0;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.cbWndExtra = 0;
	wndClass.hbrBackground = NULL;
	wndClass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndClass.hIcon = NULL;
	wndClass.hIconSm = NULL;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc=GLWindowProc;
	wndClass.lpszClassName = L"OpenGL";
	wndClass.lpszMenuName = NULL;
	wndClass.style = CS_VREDRAW | CS_HREDRAW;
	ATOM atom = RegisterClassEx(&wndClass);

	HWND hwnd = CreateWindowEx(NULL, L"OpenGL", L"RenderWindow", WS_OVERLAPPEDWINDOW, 100, 100, 800, 600, NULL, NULL, hInstance, NULL);
	HDC dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA | PFD_DOUBLEBUFFER;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixelFormatID = ChoosePixelFormat(dc, &pfd);

	SetPixelFormat(dc,pixelFormatID,&pfd);

	HGLRC rc = wglCreateContext(dc);
	wglMakeCurrent(dc, rc);
	glewInit();
	GLuint program = CreateGPUProgram("sample.vs", "sample.fs");
	GLint posLocation, colorLocation, MLocation, VLocation, PLocation;
	posLocation = glGetAttribLocation(program, "pos");
	colorLocation = glGetAttribLocation(program, "color");

	MLocation = glGetUniformLocation(program, "M");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program, "P");

	Vertex vertex[3];
	vertex[0].pos[0] = 0;
	vertex[0].pos[1] = 0;
	vertex[0].pos[2] = -30.0f;
	vertex[0].color[0] = 1.0f;
	vertex[0].color[1] = 1.0f;
	vertex[0].color[2] = 1.0f;
	vertex[0].color[3] = 1.0f;

	vertex[1].pos[0] = 10;
	vertex[1].pos[1] = 0;
	vertex[1].pos[2] = -30.0f;
	vertex[1].color[0] = 1.0f;
	vertex[1].color[1] = 1.0f;
	vertex[1].color[2] = 1.0f;
	vertex[1].color[3] = 1.0f;

	vertex[2].pos[0] = 0;
	vertex[2].pos[1] = 10;
	vertex[2].pos[2] = -30.0f;
	vertex[2].color[0] = 1.0f;
	vertex[2].color[1] = 1.0f;
	vertex[2].color[2] = 1.0f;
	vertex[2].color[3] = 1.0f;

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 3, vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int indexes[] = {0,1,2};
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3, indexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glClearColor(41.0f / 255.0f, 71.0f / 255.0f, 121.0f / 255.0f, 1.0f);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	float identity[] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 1000.0f);
	MSG msg;

	while (true)
	{
		if (PeekMessage(&msg,NULL,NULL,NULL,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glUseProgram(program); 
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(posLocation);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertexAttribArray(colorLocation);
		glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(float) * 3));
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);

		SwapBuffers(dc);
	}
	return 0;
}
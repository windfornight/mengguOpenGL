#include <windows.h>
#include <stdio.h>
#include "glew.h"
#include "misc.h"
#include "model.h"
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glu32.lib")

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
	RECT rect;
	rect.left = 0;
	rect.right = 800;
	rect.bottom = 600;
	rect.top = 0;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindowEx(NULL, L"OpenGL", L"RenderWindow", WS_OVERLAPPEDWINDOW, 100, 100, rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, hInstance, NULL);
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
	int width, height;

	GetClientRect(hwnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	glewInit();

	GLuint program = CreateGPUProgram("res/shader/UIFullScreenQuad.vs", "res/shader/UI.fs");
	GLint posLocation, texcoordLocation,normalLocation, MLocation, VLocation, PLocation,NMLocation,textureLocation;
	posLocation = glGetAttribLocation(program, "pos");
	texcoordLocation = glGetAttribLocation(program, "texcoord");
	normalLocation = glGetAttribLocation(program, "normal");

	MLocation = glGetUniformLocation(program, "M");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program, "P");
	NMLocation = glGetUniformLocation(program, "NM");
	textureLocation = glGetUniformLocation(program, "U_MainTexture");

	// load obj model :  vertexes��vertex count��indexes��index count
	unsigned int *indexes = nullptr;
	int vertexCount = 0, indexCount = 0;
	VertexData*vertexes = LoadObjModel("res/model/Quad.obj", &indexes, vertexCount, indexCount);
	if (vertexes==nullptr)
	{
		printf("load obj model fail\n");
	}
	
	//obj model -> vbo & ibo
	GLuint vbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, GL_STATIC_DRAW, vertexes);
	GLuint ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, GL_STATIC_DRAW, indexes);
	GLuint mainTexture = CreateTextureFromFile("res/image/niutou.bmp");
	printf("vertex count %d index count %d\n",vertexCount,indexCount);

	glClearColor(0.1f, 0.4f, 0.7f,1.0f);
	glEnable(GL_DEPTH_TEST);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glViewport(0, 0, width,height);
	float identity[] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	glm::mat4 model = glm::translate(0.0f,0.0f,0.0f);
	glm::mat4 projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	//glm::mat4 uiMatrix = glm::ortho(-400.0f, 400.0f,-300.0f,300.0f);
	glm::mat4 normalMatrix = glm::inverseTranspose(model);
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
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(NMLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		glBindTexture(GL_TEXTURE_2D, mainTexture);
		glUniform1i(texcoordLocation, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glEnableVertexAttribArray(posLocation);
		glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0);
		glEnableVertexAttribArray(texcoordLocation);
		glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3));
		glEnableVertexAttribArray(normalLocation);
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 5));
		
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);

		SwapBuffers(dc);
	}
	return 0;
}
#include <windows.h>
#include <stdio.h>
#include "glew.h"
#include "wglew.h"
#include "misc.h"
#include "model.h"
#include "Glm/glm.hpp"
#include "Glm/ext.hpp"
#include "timer.h"
#include "frustum.h"
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glew32.lib")
#pragma comment(lib,"glu32.lib")

HGLRC CreateNBRC(HDC dc)
{
	HGLRC rc;
	GLint attribs[]{
		WGL_DRAW_TO_WINDOW_ARB,GL_TRUE,
		WGL_ACCELERATION_ARB,WGL_FULL_ACCELERATION_ARB,
		WGL_DOUBLE_BUFFER_ARB,GL_TRUE,
		WGL_RED_BITS_ARB,8,
		WGL_GREEN_BITS_ARB,8,
		WGL_BLUE_BITS_ARB,8,
		WGL_ALPHA_BITS_ARB,8,
		WGL_DEPTH_BITS_ARB,24,
		WGL_STENCIL_BITS_ARB,8,
		WGL_SAMPLE_BUFFERS_ARB,GL_TRUE,
		WGL_SAMPLES_ARB,16,
		NULL,NULL
	};
	int pixelFormat[256] = {0};
	UINT formatNum = 0;
	wglChoosePixelFormatARB(dc, attribs, NULL, 256, pixelFormat, &formatNum);
	printf("support format number is %u\n",formatNum);
	if (formatNum>0)
	{
		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(dc, pixelFormat[0], sizeof(pfd),&pfd);
		SetPixelFormat(dc, pixelFormat[0], &pfd);

		int contexAttributes[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,4,
			WGL_CONTEXT_MINOR_VERSION_ARB,3,
			WGL_CONTEXT_PROFILE_MASK_ARB,WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
			0
		};
		rc = wglCreateContextAttribsARB(dc, nullptr, contexAttributes);
	}
	return rc;
}

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

	glewInit();
	if (wglChoosePixelFormatARB)
	{
		//destroy window
		wglMakeCurrent(dc, nullptr);
		wglDeleteContext(rc);
		rc = nullptr;
		ReleaseDC(hwnd, dc);
		dc = nullptr;
		DestroyWindow(hwnd);
		hwnd = CreateWindowEx(NULL, L"OpenGL", L"RenderWindow", WS_OVERLAPPEDWINDOW, 100, 100, rect.right - rect.left, rect.bottom - rect.top, NULL, NULL, hInstance, NULL);
		//create msaa rc
		dc = GetDC(hwnd);
		rc = CreateNBRC(dc);
		wglMakeCurrent(dc, rc);
	}

	int width, height;

	GetClientRect(hwnd, &rect);
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	GLuint program = CreateGPUProgram("res/shader/SimpleTexture.vs", "res/shader/SimpleTexture.fs");
	GLint posLocation, texcoordLocation,normalLocation, MLocation, VLocation, PLocation,textureLocation;
	posLocation = glGetAttribLocation(program, "pos");
	texcoordLocation = glGetAttribLocation(program, "texcoord");
	normalLocation = glGetAttribLocation(program, "normal");

	MLocation = glGetUniformLocation(program, "M");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program, "P");
	textureLocation = glGetUniformLocation(program, "U_MainTexture");

	// load obj model :  vertexes¡¢vertex count¡¢indexes¡¢index count
	unsigned int *indexes = nullptr;
	int vertexCount = 0, indexCount = 0;
	VertexData*vertexes = LoadObjModel("res/model/niutou.obj", &indexes, vertexCount, indexCount);
	if (vertexes==nullptr)
	{
		printf("load obj model fail\n");
	}
	//obj model -> vbo & ibo
	GLuint vao = CreateVAOWithVBOSettings([&]()->void
	{
		GLuint vbo = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * vertexCount, GL_STATIC_DRAW, vertexes);
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
		GL_CALL(glEnableVertexAttribArray(posLocation));
		GL_CALL(glVertexAttribPointer(posLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)0));
		GL_CALL(glEnableVertexAttribArray(texcoordLocation));
		GL_CALL(glVertexAttribPointer(texcoordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 3)));
		/*GL_CALL(glEnableVertexAttribArray(normalLocation));
		GL_CALL(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 5)));*/
		GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	});

	GLuint ibo = CreateBufferObject(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexCount, GL_STATIC_DRAW, indexes);

	unsigned char*imgData = (unsigned char*)LoadFileContent("res/image/niutou.bmp");
	unsigned char*pixelData = nullptr;
	int imgWidth, imgHeight;
	pixelData = DecodeBMPData(imgData, imgWidth, imgHeight);

	GLuint computeProgram = CreateComputeProgram("res/shader/simple.compute");
	GLuint computeDstTexture;
	glGenTextures(1, &computeDstTexture);
	glBindTexture(GL_TEXTURE_2D, computeDstTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexStorage2D(GL_TEXTURE_2D,1,GL_RGBA8,imgWidth,imgHeight);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint mainTexture;

	glGenTextures(1, &mainTexture);
	glBindTexture(GL_TEXTURE_2D, mainTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, imgWidth, imgHeight);
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,imgWidth,imgHeight,GL_RGB,GL_UNSIGNED_BYTE,pixelData);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pixelData);
	glBindTexture(GL_TEXTURE_2D, 0);

	Timer t;
	t.Start();
	//0.002 second gpu version : 0.000156
	/*for (int i=0;i<imgWidth*imgHeight *3;i++)
	{
		pixelData[i] = 255 - pixelData[i];
	}
	printf("time cost %f\n",t.GetPassedTime());*/

	glUseProgram(computeProgram);
	glBindImageTexture(0, mainTexture, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
	glBindImageTexture(1, computeDstTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA8);
	glDispatchCompute(imgWidth / 16, imgHeight / 16, 1);
	//sync cpu with gpu
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
	printf("time cost %f\n", t.GetPassedTime());
	delete imgData;

	GL_CALL(glClearColor(0.1f, 0.4f, 0.7f,1.0f));
	glEnable(GL_DEPTH_TEST);
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	glViewport(0, 0, width,height);

	float identity[] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	glm::mat4 model = glm::translate(0.0f,-0.5f,-1.5f)*glm::rotate(-90.0f,0.0f,1.0f,0.0f)*glm::scale(0.01f,0.01f,0.01f);
	glm::mat4 projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	glm::mat4 normalMatrix = glm::inverseTranspose(model);
	MSG msg;
	auto waht = [&]()->void 
	{
		glUseProgram(program);
		glUniformMatrix4fv(MLocation, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(VLocation, 1, GL_FALSE, identity);
		glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glBindTexture(GL_TEXTURE_2D, computeDstTexture);
		glUniform1i(textureLocation, 0);
		glBindVertexArray(vao);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	};

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		waht();
		glFlush();
		SwapBuffers(dc);
	}
	return 0;
}
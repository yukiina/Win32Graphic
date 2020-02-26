#include "GLGraphic.h"

#pragma comment(lib, "opengl32.lib")

GLGraphic::GLGraphic(HWND hWnd, float width, float height)
{
	dc = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_TYPE_RGBA | PFD_DOUBLEBUFFER;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int pixelFormatId = ChoosePixelFormat(dc, &pfd);
	SetPixelFormat(dc, pixelFormatId, &pfd);

	rc = wglCreateContext(dc);

	wglMakeCurrent(dc, rc);
}

GLGraphic::~GLGraphic()
{
}

void GLGraphic::ClearBuffer(float red, float green, float blue)
{
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GLGraphic::EndFrame()
{
	SwapBuffers(dc);
}

float GLGraphic::AspectRatio()
{
	return 0.0f;
}


void GLGraphic::DrawIndex(int indexCount)
{
}

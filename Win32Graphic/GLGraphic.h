#pragma once
#include "Graphic.h"
#include <Windows.h>
#include <gl/GL.h>


class GLGraphic : public Graphic {
public:
	GLGraphic(HWND hWnd, float width, float height);
	~GLGraphic();
public:
	// Í¨¹ý Graphic ¼Ì³Ð
	virtual void ClearBuffer(float red, float green, float blue) override;
	virtual void EndFrame() override;
	virtual float AspectRatio() override;
	virtual void DrawIndex(int indexCount) override;

private:
	HDC dc;
	HGLRC rc;
};
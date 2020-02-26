#pragma once

/*
 * 图形接口的基类, DirectX, OpenGL等图形接口的封装类的基类
 * 定义渲染过程中的固定流程
 */

class Graphic{
public:
	// 负责用指定颜色清除颜色缓冲
	virtual void ClearBuffer(float red, float green, float blue) =  0;
	// 双缓冲交换, 呈现图像在屏幕上
	virtual void EndFrame() = 0;     
	// return width / height
	virtual float AspectRatio() = 0;
	// Draw with IndexBuffer
	virtual void DrawIndex(int indexCount) = 0;

};
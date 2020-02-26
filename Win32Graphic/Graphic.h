#pragma once

/*
 * ͼ�νӿڵĻ���, DirectX, OpenGL��ͼ�νӿڵķ�װ��Ļ���
 * ������Ⱦ�����еĹ̶�����
 */

class Graphic{
public:
	// ������ָ����ɫ�����ɫ����
	virtual void ClearBuffer(float red, float green, float blue) =  0;
	// ˫���彻��, ����ͼ������Ļ��
	virtual void EndFrame() = 0;     
	// return width / height
	virtual float AspectRatio() = 0;
	// Draw with IndexBuffer
	virtual void DrawIndex(int indexCount) = 0;

};
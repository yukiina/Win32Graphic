#pragma once
#include <exception>
#include <string>
#include <Windows.h>

// Win32 HRESULT�����װΪ�쳣, ����Ϊ��ֱ��ʹ��
#define HR_ERROR(hr) throw HRException(hr, __LINE__,__FILE__)

class HRException : public std::exception
{
public:
	HRException(HRESULT hr, int line, const char* file) noexcept;
	const char* what() const noexcept override;
private:
	std::string TranslateErrorCode(HRESULT hr) const;
private:
	int line;
	std::string file;
	HRESULT hr;
	mutable std::string whatBuffer;
};

// Obj����, ����һ�������ַ���
#define STR_ERROR(str) throw ObjModelException(str,__LINE__,__FILE__)
// Obj ģ�ͼ��ش���
class ObjModelException : public std::exception {
public:
	ObjModelException(const char* errorDesc, int line, const char* file) noexcept;
	const char* what() const noexcept override;
private:
	int line;
	std::string file;
	mutable std::string whatBuffer;
};

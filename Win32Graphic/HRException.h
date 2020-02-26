#pragma once
#include <exception>
#include <string>
#include <Windows.h>

// Win32 HRESULT错误封装为异常, 定义为宏直接使用
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

// Obj错误, 给定一个错误字符串
#define STR_ERROR(str) throw ObjModelException(str,__LINE__,__FILE__)
// Obj 模型加载错误
class ObjModelException : public std::exception {
public:
	ObjModelException(const char* errorDesc, int line, const char* file) noexcept;
	const char* what() const noexcept override;
private:
	int line;
	std::string file;
	mutable std::string whatBuffer;
};

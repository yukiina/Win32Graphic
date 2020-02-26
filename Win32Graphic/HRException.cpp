#include "HRException.h"
#include <sstream>

HRException::HRException(HRESULT hr, int line, const char * file) noexcept
	:hr(hr),
	line(line),
	file(file)
{
}

// 输出详细的错误信息
const char * HRException::what() const noexcept
{
	std::ostringstream oss;
	oss << "HRException" << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << hr
		<< std::dec << " (" << (unsigned long)hr << ")" << std::endl
		<< "[Description] " << TranslateErrorCode(hr) << std::endl
		<< "[File] " << file << std::endl
		<< "[Line] " << line;
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

// HRESULT 翻译成字符串
std::string HRException::TranslateErrorCode(HRESULT hr) const
{
	char* pMsgBuf = nullptr;
	// windows 分配错误字符串的内存
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// 没有这个HRESULT
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// 拷贝
	std::string errorString = pMsgBuf;
	// 清理windows内存
	LocalFree(pMsgBuf);
	return errorString;
}

ObjModelException::ObjModelException(const char * errorDesc, int line, const char * file) noexcept
	:line(line), file(file), whatBuffer(errorDesc)
{
}

const char * ObjModelException::what() const noexcept
{
	return whatBuffer.c_str();
}
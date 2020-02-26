#include "HRException.h"
#include <sstream>

HRException::HRException(HRESULT hr, int line, const char * file) noexcept
	:hr(hr),
	line(line),
	file(file)
{
}

// �����ϸ�Ĵ�����Ϣ
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

// HRESULT ������ַ���
std::string HRException::TranslateErrorCode(HRESULT hr) const
{
	char* pMsgBuf = nullptr;
	// windows ��������ַ������ڴ�
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);
	// û�����HRESULT
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// ����
	std::string errorString = pMsgBuf;
	// ����windows�ڴ�
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
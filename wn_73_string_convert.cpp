
#include <iostream>
#include <string>
#include <windows.h>

// unicode > multibyte
std::string str_unicode_to_mbyte(const std::wstring& src) {
	std::string ret;
	int len = WideCharToMultiByte( CP_ACP, 0, src.c_str(), src.length(), NULL, 0, NULL, NULL );
	if(0>=len)
		return ret;
	ret.resize(len);
	len = WideCharToMultiByte( CP_ACP, 0, src.c_str(), src.length(), &ret[0], len, NULL, NULL );
	if(0>=len)
		return {};
	return ret;
}

// multibyte > unicode
std::wstring str_mbyte_to_unicode(const std::string& src) {
	std::wstring ret;
	int len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), src.length(), NULL, NULL);
	if(0>=len)
		return ret;
	ret.resize(len);
	len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), src.length(), &ret[0], len);
	if(0>=len)
		return {};
	return ret;
}

// unicode > utf8
std::string str_unicode_to_utf8(const std::wstring& src) {
	std::string ret;
	int len = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), src.length(), NULL, 0, NULL, NULL);
	if(0>=len)
		return ret;
	ret.resize(len);
	len = WideCharToMultiByte (CP_UTF8, 0, src.c_str(), src.length(), &ret[0], len, NULL, NULL);
	if(0>=len)
		return {};
	return ret;
}

// utf8 > unicode
std::wstring str_utf8_to_unicode(const std::string& src) {
	std::wstring ret;
	int len = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), src.length(), NULL, NULL);
	if(0>=len)
		return ret;
	ret.resize(len);
	len = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), src.length(), &ret[0], len);
	if(0>=len)
		return {};
	return ret;
}

// utf8 > mbyte
std::string str_utf8_to_mbyte(const std::string& src) {
	auto w_str = str_utf8_to_unicode(src);
	return str_unicode_to_mbyte(w_str);
}

// mbyte -> utf8
std::string str_mbyte_to_utf8(const std::string& src) {
	auto w_str = str_mbyte_to_unicode(src);
	return str_unicode_to_utf8(w_str);
}

int main() {
	char str_mb [] = "안녕하세요";
	std::wstring str_unicode = str_mbyte_to_unicode(str_mb);
	std::wcout << "unicode: " << str_unicode << std::endl;

	std::string str_mbyte = str_unicode_to_mbyte(str_unicode);
    std::cout << "mbyte: " << str_mbyte << std::endl;

	std::string str_utf8 = str_unicode_to_utf8(str_unicode);
	std::cout << "utf8: " << str_utf8 << std::endl;

	str_utf8 = str_mbyte_to_utf8(str_mb);
	str_mbyte = str_utf8_to_mbyte(str_utf8);
	std::cout << "mbyte: " << str_mbyte << std::endl;
}

#include <iostream>
#include <string>
#include <codecvt>
#include <locale.h>

// win32 api -----------------------------------------------------------------------------------------------------------
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

// std:: code cvt ------------------------------------------------------------------------------------------------------

// convert UTF-8 string to wstring
std::wstring cvt_utf8_to_unicode(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.from_bytes(str);
}

// convert wstring to UTF-8 string
std::string cvt_unicode_to_utf8(const std::wstring& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	return conv.to_bytes(str);
}

std::string cpp_string_nullptr()
{
	return std::string("");
}

void cpp_str2(const std::string& ref={})
{
	printf("%s\n", ref.c_str());
}

int main(int, char**)
{
	::setlocale(LC_ALL,"");
	char str_mb [] = "안녕하세요. Hello world";
	std::wstring str_unicode = str_mbyte_to_unicode(str_mb);
	std::wcout << "unicode: " << str_unicode << std::endl;

	std::string str_mbyte = str_unicode_to_mbyte(str_unicode);
	std::cout << "mbyte: " << str_mbyte << std::endl;

	std::string str_utf8 = str_unicode_to_utf8(str_unicode);
	std::cout << "utf8: " << str_utf8 << std::endl;

	str_utf8 = str_mbyte_to_utf8(str_mb);
	str_mbyte = str_utf8_to_mbyte(str_utf8);
	std::cout << "mbyte: " << str_mbyte << std::endl;

	str_utf8 = cvt_unicode_to_utf8(str_unicode);
	str_unicode = cvt_utf8_to_unicode(str_utf8);
	std::wcout << "code cvt: " << str_unicode << std::endl;

	//std::string ret ="Hello world";
	//ret = cpp_string_nullptr();
	//printf("%s\n", ret.c_str());
	//size_t cc = std::string::npos;
	//int dd    = std::string::npos;
	//int e = EOF;
	return 0;
}

#include <clocale>
#include <cwchar>
int use_locale()
{
	//std::setlocale(LC_ALL, "en_US.utf8");
	//std::setlocale(LC_ALL, "korean");
	std::setlocale(LC_ALL, "");
	std::string str_mbs = "안녕하세요. Hello world 방가와염";

	std::wstring str_wchar;
	auto w_len = std::mbstowcs({}, str_mbs.data(), str_mbs.length());
	str_wchar.resize(w_len);
	w_len = std::mbstowcs(str_wchar.data(), str_mbs.data(), str_mbs.length());

	// std::wcstombs
	// ...
	//

	const wchar_t* wstr = str_wchar.data();
	std::size_t str_len = std::wcsrtombs(nullptr, &wstr, 0, {});
	std::string mbstr(str_len, 0);
	str_len = std::wcsrtombs(mbstr.data(), &wstr, mbstr.length(), {});
	return 0;
}


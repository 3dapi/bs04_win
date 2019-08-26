#include <iostream>
#include <string>
#include <sstream>

std::string stream_to_string(const std::stringstream& src_stream)
{
	std::string ret = src_stream.str();
	return ret;
}

std::stringstream string_to_stream(const std::string& src_str)
{
	return std::stringstream(src_str);
}

void string_fit(std::string& src_str)
{
	src_str.resize( [&]()->size_t {
		auto r =  src_str.find_first_of('\0');
		return r == std::string::npos ? 0: r;
	}());
}

void* stl_memcpy(void* dst, void* src, size_t len)
{
	char* c_dst = (char*)dst;
	char* c_src = (char*)src;
	char* ret = std::copy(c_src, c_src+len, c_dst);
	return ret;
}

int crt_memsize(void* ptr)
{
	size_t mmsize = 0;
#if defined(_MSC_VER)
	mmsize = (size_t)_msize(ptr);
#elif defined(ANDROID)
	mmsize = (size_t)malloc_usable_size(ptr);
#elif defined(__IOS__)
	mmsize = (size_t)malloc_size(ptr);
#endif
	return mmsize;
}

static int main()
{
	std::string str_fit_test;
	str_fit_test.resize(64);
	strcpy(&str_fit_test[0], "Hello world welcome 123445");
	string_fit(str_fit_test);

	std::stringstream str_stream("1234567890123456789012345678901234567890");
	std::string src_str = stream_to_string(str_stream);
	long long deci_64 = atoll(src_str.c_str());
	str_stream = string_to_stream("Hello world");
	str_stream = string_to_stream(src_str);
	str_stream >> deci_64;
	return 0;
}

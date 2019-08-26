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

static int main()
{
	std::stringstream str_stream("1234567890123456789012345678901234567890");
	std::string src_str = stream_to_string(str_stream);
	long long deci_64 = atoll(src_str.c_str());
	str_stream = string_to_stream("Hello world");
	str_stream = string_to_stream(src_str);
	str_stream >> deci_64;
	return 0;
}

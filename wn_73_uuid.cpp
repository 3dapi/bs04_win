#include <iostream>
#include <string>

#include <windows.h>
#pragma comment(lib, "rpcrt4.lib")  // UuidCreate - Minimum supported OS Win 2000

std::string cbi_uuid()
{
	std::string ret;
	UUID uuid;
	if(RPC_S_OK != ::UuidCreate(&uuid))
		return {};
	char* s;
	::UuidToStringA(&uuid, (RPC_CSTR*)&s);
	ret = (const char*)s;
	::RpcStringFreeA((RPC_CSTR*)&s);
	return ret;
}

int main()
{
	std::string uuid = cbi_uuid();
	std::cout<<uuid<<std::endl;
	return 0;
}

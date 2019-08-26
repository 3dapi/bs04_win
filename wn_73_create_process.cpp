
#include <stdio.h>
#include <windows.h>

void main()
{
	printf("ShellExecute--------------------");
	//ShellExecute(NULL, "open", "curl", " --help", NULL, SW_SHOW);

	STARTUPINFO startup{};
	startup.cb = sizeof(startup);
	PROCESS_INFORMATION processInfo;
	CreateProcess("curl --help", NULL, NULL, NULL, FALSE, 0, NULL, NULL,&startup, &processInfo);
	getc(stdin);
}
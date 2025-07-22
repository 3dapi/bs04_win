#include <windows.h>
#include <stdio.h>

#define MAX_SHARE_SIZE 256


HANDLE	g_hMap;			// File Mapping Handle
HANDLE	g_hFile;		// Dest File

char*	g_pMsg;



void main()
{
	printf("File Memory Mapping\n");

	char sMsgCur[MAX_SHARE_SIZE];
	memset(sMsgCur, 0, sizeof sMsgCur);

	// 1. 파일을 만든다.
	g_hFile=CreateFile("Mapping.txt",GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (g_hFile == INVALID_HANDLE_VALUE)
	{
		printf("Could not open file.\n");   // process error
		g_hFile=CreateFile("Mapping.txt",GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	// 2. 파일 맵핑 오브젝트를 만든다.
	//	g_hMap=CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MAX_SHARE_SIZE, g_sShare);
	g_hMap=CreateFileMapping(g_hFile, NULL, PAGE_READWRITE, 0, 0, NULL);

	// 다른 쓰레드에서 공유: OpenFileMapping <= CreateFileMapping


	// 3. 주소 공간에 맵핑한다.
	//g_pMsg=(char*)MapViewOfFile(g_hMap, FILE_MAP_ALL_ACCESS, 0, 0, MAX_SHARE_SIZE);
	g_pMsg=(char*)MapViewOfFile(g_hMap, FILE_MAP_ALL_ACCESS,0,0,0);

	printf("%s", g_pMsg);

	while(1)
	{
		fgets(sMsgCur, sizeof sMsgCur, stdin);
		strncpy(g_pMsg+strlen(g_pMsg)-1, sMsgCur, strlen(sMsgCur));
	}

	if(g_hMap)
	{
		UnmapViewOfFile(g_pMsg);
		g_pMsg = NULL;

		CloseHandle(g_hMap);
		g_hMap = NULL;
	}

	if(g_hFile)
	{
		CloseHandle(g_hFile);
		g_hFile = NULL;
	}


}
#include <windows.h>
#include <stdio.h>

#define MAX_SHARE_SIZE 256

HANDLE	g_hMap;
char*	g_pMsg;
char*	g_sShare="MemoryShareMapping";

void main()
{
	char sMsgCur[MAX_SHARE_SIZE];
	char sMsgOld[MAX_SHARE_SIZE];

	memset(sMsgCur, 0, sizeof sMsgCur);
	memset(sMsgOld, 0, sizeof sMsgOld);

	printf("Memory Share:2번 실행 하세요.\n");

	g_hMap=CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MAX_SHARE_SIZE, g_sShare);
	g_pMsg=(char*)MapViewOfFile(g_hMap, FILE_MAP_ALL_ACCESS, 0, 0, MAX_SHARE_SIZE);

	if(g_pMsg && 0==strlen(g_pMsg))
	{
		strcpy(g_pMsg, "Start Memory Share");

		while(1)
		{
			memcpy(sMsgOld, sMsgCur, sizeof sMsgOld);
			strcpy(sMsgCur, g_pMsg);

			if( 0 != memcmp(sMsgCur, sMsgOld, sizeof sMsgCur))
			{
				printf("%s\n", g_pMsg);
			}
		}
	}

	else
	{
		while(1)
		{
			fgets(sMsgCur, sizeof sMsgCur, stdin);
			char * pdst = strrchr(sMsgCur, '\n');
			sMsgCur[pdst-sMsgCur] = 0;

			strcpy(g_pMsg, sMsgCur);


		}
	}

	if(g_hMap)
	{
		UnmapViewOfFile(g_pMsg);
		g_pMsg = NULL;

		CloseHandle(g_hMap);
		g_hMap = NULL;
	}


}
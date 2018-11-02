

#include <stdio.h>
#include <windows.h>

#include "LnFile.h"


void main()
{
	char sMsg[] = "Hello world\n";

	// 1. ǥ�����
	printf(sMsg);

	// 2. ���ڿ��� ���
	char sBuf[1024];
	int	 nIdx=10;

	sprintf(sBuf,"key1: %d	Key2: %s", nIdx, sMsg);
//	MessageBox(0, sBuf, "���ڿ��� ���", 0);

	//3. ���Ͽ� ���
	HANDLE pFpOut;

	pFpOut = LnFile_fopen("FileOut/Output.txt", "wt");

	if(NULL == pFpOut)
	{
		MessageBox(0, "Cannot open file", "Err", 0);
		return;
	}

	for(int i=0; i<10; ++i)
		LnFile_fprintf(pFpOut,"key1: %d	Key2: %s", nIdx+i, sMsg);

	LnFile_fclose(pFpOut);
	pFpOut =NULL;





	// 4. ǥ�� �Է�
	char sStr[128];
//	scanf("%s", sStr);
//	LnFile_fprintf(pFpOut, "%s\n", sStr);

	// 5. ���ڿ����� �Է�
	int nIdx2;
	sscanf(sBuf,"%*s %d %*s %s", &nIdx2, sStr);



	//6. ���Ͽ��� �Է�
	HANDLE pFpIn;

	pFpIn = LnFile_fopen("FileOut/Output.txt", "rt");

	if(NULL == pFpIn)
	{
		MessageBox(0, "Cannot open file", "Err", 0);
		return;
	}

	while(1)
	{
		char sLine[512];
		memset(sLine, 0, sizeof(sLine));

		INT hr = LnFile_fgets(pFpIn, sLine, sizeof(sLine));

		if(0 !=strlen(sLine) && sLine[0])
		{
			sscanf(sLine,"%*s %d %*s %s", &nIdx2, sStr);
			printf("Index: %d\n", nIdx2);
			printf("String: %s\n", sStr);
		}

		if(FAILED(hr))
			break;
	}

	LnFile_fclose(pFpIn);
}
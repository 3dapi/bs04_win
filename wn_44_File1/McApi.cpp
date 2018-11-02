

#include <stdio.h>
#include <windows.h>


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
	FILE* pFpOut;

	pFpOut = fopen("FileOut/Output.txt", "wt");

	if(NULL == pFpOut)
	{
		MessageBox(0, "Cannot open file", "Err", 0);
		return;
	}

	for(int i=0; i<10; ++i)
		fprintf(pFpOut,"key1: %d	Key2: %s", nIdx+i, sMsg);

	fclose(pFpOut);
	pFpOut =NULL;





	// 4. ǥ�� �Է�
	char sStr[128];
//	scanf("%s", sStr);
//	fprintf(pFpOut, "%s\n", sStr);

	// 5. ���ڿ����� �Է�
	int nIdx2;
	sscanf(sBuf,"%*s %d %*s %s", &nIdx2, sStr);



	//6. ���Ͽ��� �Է�
	FILE* pFpIn;

	pFpIn = fopen("FileOut/Output.txt", "rt");

	if(NULL == pFpIn)
	{
		MessageBox(0, "Cannot open file", "Err", 0);
		return;
	}

	while( !feof(pFpIn))
	{
		char sLine[512];
		memset(sLine, 0, sizeof(sLine));
		fgets(sLine, sizeof(sLine), pFpIn);

		if(0 !=strlen(sLine) && sLine[0])
		{
			sscanf(sLine,"%*s %d %*s %s", &nIdx2, sStr);
			printf("Index: %d\n", nIdx2);
			printf("String: %s\n", sStr);
		}
	}

	fclose(pFpIn);
}
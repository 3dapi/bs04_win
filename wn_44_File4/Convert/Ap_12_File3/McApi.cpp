// binary data read
// �ۼ��� : ����
// �ۼ��� : 2002. 3. 5.
// ���� : ���� ���� ���� ������ ������ �Է� �޾� ������/ �̸������� �����Ѵ�.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LnFile.h"

#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);	(p)=NULL;	} }

struct CStudent
{
	int nIndex;																	//Index
	char sName[64];																//�̸�
	char sIdNum[64];															//�й�
	int	iKor;																	//����
	int iEng;																	//����
	int iMth;																	//����

	int Rank;

	CStudent()
	{
		memset(sName, 0, sizeof(sName));
		memset(sIdNum, 0, sizeof(sIdNum));
	}

	~CStudent()
	{
	}


	int Total()
	{
		return (iKor + iEng + iMth) ;
	}

	float Avg()
	{
		return (Total()/3.f);
	}
};


int main(void)
{
	LNFILE fp;

	int i;
	CStudent * pBan = NULL;
	int nCount=0;


	printf("\n\nRead binary file\n");

	fp = LnFile_fopen("data/ScoreResult.brn", "rb");

	if(NULL == fp)
		return -1;

	LnFile_fread(fp, &nCount, sizeof(int),1);

	pBan = new CStudent[nCount];

	for(i=0 ;i<nCount;++i)
		LnFile_fread(fp, &pBan[i], sizeof(CStudent),1);

	LnFile_fclose(fp);


	for(i=0 ;i<nCount;++i)
	{
		printf("%s", pBan[i].sName);
		printf("	%s", pBan[i].sIdNum);
		printf("	%d", pBan[i].iKor);
		printf("	%d", pBan[i].iEng);
		printf("	%d", pBan[i].iMth);
		printf("	%d", pBan[i].Total());
		printf("	%3.3f", pBan[i].Avg());
		printf("	%2d", pBan[i].Rank);
		printf("\n");
	}

	SAFE_DELETE_ARRAY(	pBan	);												//new�� �޸𸮸� �� ���� ���� ����

	return 0;
}
// binary data read
// 작성자 : 오희성
// 작성일 : 2002. 3. 5.
// 개요 : 다음 예와 같은 파일의 내용을 입력 받아 총점순/ 이름순으로 정렬한다.

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LnFile.h"

#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);	(p)=NULL;	} }

struct CStudent
{
	int nIndex;																	//Index
	char sName[64];																//이름
	char sIdNum[64];															//학번
	int	iKor;																	//국어
	int iEng;																	//영어
	int iMth;																	//수학

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

	SAFE_DELETE_ARRAY(	pBan	);												//new로 메모리를 할 당한 것을 해제

	return 0;
}
// 성적관리 프로그램
// 개요 : 다음 예와 같은 파일의 내용을 입력 받아 총점순/ 이름순으로 정렬한다.
//
// 예)
//CLASS 1 {
//STUDENTS 12
//STUDENTLIST {
//	*REC 1 이주일 2002008 80 70 90
//	*REC 2 남기남 2002009 90 80 90
//	*REC 3 남희석 2002010 90 80 90
//	*REC 4 김석중 2002011 30 50 70
//	*REC 5 홍길동 2002002 65 80 90
//	*REC 6 조영남 2002007 90 80 90
//	*REC 7 오중달 2002012 80 80 100
//	*REC 8 이무일 2002013 90 80 90
//	*REC 9 이소룡 2002014 90 80 90
//	*REC 10 박찬호 2002015 100 80 90
//	*REC 11 박이령 2002017 90 80 50
//	*REC 12 고길동 2002018 40 100 50
//	}
//}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void RankSwap(CStudent*, int, int);
void RankScore(CStudent*, int);

int main()
{
	FILE * fp;

	CStudent * pBan = NULL;

	int nTmpCnt=0;

	char nClassNumber[4] = {0,};

	char sTmp[128]={0,};
	char sLine[128]={0,};

	char sTmpClass[7]={0,};
	char sTmpClassNo[3]={0,};

	char sTmpStudents[10]={0,};
	char sTmpStudentNum[3]={0,};
	int	 iNumStudent=0;

	char sTmpStList[13]={0,};

	char sTmpStudentScore[256]={0,};
	char sStScore[256]={0,};


	int nSize;
	int i,j, k;

	fp = fopen("data/StudentScore.txt", "rt");

	if(fp==NULL)
	{
		printf("Not read file\n");
		return -1;
	}

	char * sCmd[]=
	{
		"CLASS ",					// 0
			"STUDENTS ",			// 1
			"STUDENTLIST ",			// 2
			"REC "					// 3
	};

	while( !feof(fp))
	{
		fgets(sTmp,256,fp);

		nSize = sizeof(sTmp)/sizeof(sTmp[0]);


		for (i=0; i<nSize; ++i)													//'*'문자를 찾는다.
		{
			if(sTmp[i] =='*')													// *다음 문자부터 끝까지 읽는다.
			{
				memset(sLine, 0, sizeof sLine);

				for (j=i+1;j<nSize;j++)
				{
					k=j-i-1;

					if(sTmp[j]=='{'|| sTmp[j]=='}' || sTmp[j] =='\r' ||
						sTmp[j] =='\n')
					{
						sTmp[j]=0;
						break;
					}

					sLine[k] = sTmp[j];
				}


				strncpy(sTmpClass,sLine, strlen(sCmd[0]) );						//sLine 에서 키워드를 분리한다.
				if (strcmp(sTmpClass,sCmd[0])==0)								//CLASS 인가 ?
				{
					int iBgn = strlen(sCmd[0]);
					int iFnd = sizeof(sLine)/sizeof(sLine[0]);

					for( k= iBgn ; k < iFnd; ++k)								//CLASS이후의 문자열을 불러 들인다.
					{
						sTmpClassNo[ k-strlen(sCmd[0]) ] = sLine[k];			//CLASS 번호를 기억

						if(sLine[k] ==NULL)
							break;
					}

					break;
				}


				strncpy(sTmpStudents,sLine,strlen(sCmd[1]));
				if (strcmp(sTmpStudents,sCmd[1])==0)							//STUDENTS 인가
				{
					int iBgn = strlen(sCmd[1]);
					int iFnd = sizeof(sLine)/sizeof(sLine[0]);

					for( k= iBgn ; k < iFnd; ++k)								//STUDENTS 이후의 문자열을 불러 들인다.
					{
						sTmpStudentNum[ k-strlen(sCmd[1]) ]=sLine[k];			//학생수를 찾는다.

						if(sLine[k] ==NULL)
							break;
					}

					iNumStudent = atoi(sTmpStudentNum);						//문자열 학생수를 정수로 바꾼다.

					pBan = new CStudent[iNumStudent];							//학생에 대한 구조체를 할당한다.
					break;
				}


				strncpy(sTmpStList,sLine, strlen(sCmd[2]) );

				if (strcmp(sTmpStList,sCmd[2])==0)								//STUDENTLIST 인가
				{
					break;
				}


				strncpy(sTmpStudentScore,sLine, strlen(sCmd[3]) );
				if (strcmp(sTmpStudentScore,sCmd[3])==0)						//STUDENT 인가
				{

					int iBgn = strlen(sCmd[3]);
					int iFnd = sizeof(sLine)/sizeof(sLine[0]);

					memset(sStScore, 0, sizeof sStScore);

					for( k= iBgn ; k < iFnd; ++k)								//STUDENT 이후의 문자열을 불러 들인다.
					{
						sStScore[k-strlen(sCmd[3])]=sLine[k];					//index부터 수학까지 점수 모두를 붓는다.

						if(sLine[k] ==NULL)
							break;
					}

					sscanf(sStScore,"%d %s %s %d %d %d"							//구조체에 넣는다.
						,	&pBan[nTmpCnt].nIndex
						,	pBan[nTmpCnt].sName
						,	pBan[nTmpCnt].sIdNum
						,	&pBan[nTmpCnt].iKor
						,	&pBan[nTmpCnt].iEng
						,	&pBan[nTmpCnt].iMth);

					pBan[nTmpCnt].Rank = nTmpCnt+1;								//등수를 그냥 올린다.

					nTmpCnt++;
				}

				break;

			}//if *

		}//for
	}

	fclose(fp);


	if(iNumStudent !=nTmpCnt)													// 학생수 불일치
		return -1;

	printf("파일입력직후\nName\tStudent ID\tKorea\tEnglish\tMath\tTotal\tAverage\tRank\n");


	for(i=0 ;i<iNumStudent;++i)
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

	printf("\n\n");

	//전체성적으로 정렬
	for (i=0 ; i<iNumStudent-1; ++i)
	{
		for(j=i; j<iNumStudent; ++j)
		{
			if (
				pBan[i].Total() < pBan[j].Total()														//성적비교
				||	(pBan[i].Total() == pBan[j].Total() && (strcmp(pBan[j].sName, pBan[i].sName)<0))		//성적이 같을 경우 이름순으로 정렬
				)
			{
				RankSwap(pBan, i,j);
			}
		}
	}

	//등수 매기기
	RankScore(pBan, iNumStudent);




	// 텍스트 파일로 결과 출력
	fp = fopen("data/ScoreResult.txt", "wt");

	fprintf(fp,"%d\n", iNumStudent);

	for(i=0 ;i<iNumStudent;++i)
	{
		fprintf(fp,"%s", pBan[i].sName);
		fprintf(fp,"	%s", pBan[i].sIdNum);
		fprintf(fp,"	%d", pBan[i].iKor);
		fprintf(fp,"	%d", pBan[i].iEng);
		fprintf(fp,"	%d", pBan[i].iMth);
		fprintf(fp,"	%4d", pBan[i].Total());
		fprintf(fp,"	%3.3f", pBan[i].Avg());
		fprintf(fp,"	%2d", pBan[i].Rank);
		fprintf(fp,"\n");
	}

	fclose(fp);


	// 이진 파일로 결과 출력
	fp = fopen("data/ScoreResult.brn", "wb");
	fwrite(&iNumStudent, sizeof(int),1 , fp);

	for(i=0 ;i<iNumStudent;++i)
	{
		CStudent	tmp;
		tmp = pBan[i];
		fwrite(&tmp, sizeof(CStudent),1 , fp);
	}

	fclose(fp);

	SAFE_DELETE_ARRAY(	pBan	);												//new로 메모리를 할 당한 것을 해제

	return 0;
}


// Data Swapping
void RankSwap(CStudent * pBan, int i,int j)
{
	CStudent temp;

	memcpy (&temp, &pBan[i], sizeof(CStudent));
	memcpy (&pBan[i], &pBan[j], sizeof(CStudent));
	memcpy (&pBan[j], &temp, sizeof(CStudent));
}

//등수 매기는 거..
void RankScore(CStudent * pBan, int nStNumber)
{

	int i, nTempRank, nTempTotal;

	pBan[0].Rank = 1;

	nTempRank = 1;
	nTempTotal = pBan[0].Total ();

	for (i=1; i< nStNumber; ++i)
	{


		if (pBan[i].Total () == nTempTotal)			//총점이 같은가?
		{
			pBan[i].Rank = nTempRank;				//Rank를 동일하게 한다.
		}

		else
		{
			nTempRank = i+1;
			pBan[i].Rank = nTempRank ;
			nTempTotal = pBan[i].Total ();
		}
	}



	printf("\n\n전체 성적순으로 정렬\n");
	printf("sName\tStudent ID\tKorea\tEnglish\tMath\tTotal\tAverage\tRank\n");

	for(i=0 ;i<nStNumber;++i)
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
}

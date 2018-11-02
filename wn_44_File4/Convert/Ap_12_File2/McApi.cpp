// �������� ���α׷�
// ���� : ���� ���� ���� ������ ������ �Է� �޾� ������/ �̸������� �����Ѵ�.
//
// ��)
//CLASS 1 {
//STUDENTS 12
//STUDENTLIST {
//	*REC 1 ������ 2002008 80 70 90
//	*REC 2 ���Ⳳ 2002009 90 80 90
//	*REC 3 ���� 2002010 90 80 90
//	*REC 4 �輮�� 2002011 30 50 70
//	*REC 5 ȫ�浿 2002002 65 80 90
//	*REC 6 ������ 2002007 90 80 90
//	*REC 7 ���ߴ� 2002012 80 80 100
//	*REC 8 �̹��� 2002013 90 80 90
//	*REC 9 �̼ҷ� 2002014 90 80 90
//	*REC 10 ����ȣ 2002015 100 80 90
//	*REC 11 ���̷� 2002017 90 80 50
//	*REC 12 ��浿 2002018 40 100 50
//	}
//}


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

void RankSwap(CStudent*, int, int);
void RankScore(CStudent*, int);

int main()
{
	LNFILE fp;

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

	fp = LnFile_fopen("data/StudentScore.txt", "rt");

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

	while(1)
	{
		INT hr = LnFile_fgets(fp, sTmp,256);

		nSize = sizeof(sTmp)/sizeof(sTmp[0]);


		for (i=0; i<nSize; ++i)													//'*'���ڸ� ã�´�.
		{
			if(sTmp[i] =='*')													// *���� ���ں��� ������ �д´�.
			{

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


				strncpy(sTmpClass,sLine, strlen(sCmd[0]) );						//sLine ���� Ű���带 �и��Ѵ�.
				if (strcmp(sTmpClass,sCmd[0])==0)								//CLASS �ΰ� ?
				{
					int iBgn = strlen(sCmd[0]);
					int iFnd = sizeof(sLine)/sizeof(sLine[0]);

					for( k= iBgn ; k < iFnd; ++k)								//CLASS������ ���ڿ��� �ҷ� ���δ�.
					{
						sTmpClassNo[ k-strlen(sCmd[0]) ] = sLine[k];			//CLASS ��ȣ�� ���

						if(sLine[k] ==NULL)
							break;
					}

					break;
				}


				strncpy(sTmpStudents,sLine,strlen(sCmd[1]));
				if (strcmp(sTmpStudents,sCmd[1])==0)							//STUDENTS �ΰ�
				{
					int iBgn = strlen(sCmd[1]);
					int iFnd = sizeof(sLine)/sizeof(sLine[0]);

					for( k= iBgn ; k < iFnd; ++k)								//STUDENTS ������ ���ڿ��� �ҷ� ���δ�.
					{
						sTmpStudentNum[ k-strlen(sCmd[1]) ]=sLine[k];			//�л����� ã�´�.

						if(sLine[k] ==NULL)
							break;
					}

					iNumStudent = atoi(sTmpStudentNum);						//���ڿ� �л����� ������ �ٲ۴�.

					pBan = new CStudent[iNumStudent];							//�л��� ���� ����ü�� �Ҵ��Ѵ�.
					break;
				}


				strncpy(sTmpStList,sLine, strlen(sCmd[2]) );

				if (strcmp(sTmpStList,sCmd[2])==0)								//STUDENTLIST �ΰ�
				{
					break;
				}


				strncpy(sTmpStudentScore,sLine, strlen(sCmd[3]) );
				if (strcmp(sTmpStudentScore,sCmd[3])==0)						//STUDENT �ΰ�
				{

					int iBgn = strlen(sCmd[3]);
					int iFnd = sizeof(sLine)/sizeof(sLine[0]);

					for( k= iBgn ; k < iFnd; ++k)								//STUDENT ������ ���ڿ��� �ҷ� ���δ�.
					{
						sStScore[k-strlen(sCmd[3])]=sLine[k];					//index���� ���б��� ���� ��θ� �״´�.

						if(sLine[k] ==NULL)
							break;
					}

					sscanf(sStScore,"%d %s %s %d %d %d"							//����ü�� �ִ´�.
						,	&pBan[nTmpCnt].nIndex
						,	pBan[nTmpCnt].sName
						,	pBan[nTmpCnt].sIdNum
						,	&pBan[nTmpCnt].iKor
						,	&pBan[nTmpCnt].iEng
						,	&pBan[nTmpCnt].iMth);

					pBan[nTmpCnt].Rank = nTmpCnt+1;								//����� �׳� �ø���.

					nTmpCnt++;
				}

				break;

			}//if *

		}//for



		if(FAILED(hr))
			break;
	}

	LnFile_fclose(fp);


	if(iNumStudent !=nTmpCnt)													// �л��� ����ġ
		return -1;

	printf("�����Է�����\nName\tStudent ID\tKorea\tEnglish\tMath\tTotal\tAverage\tRank\n");


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

	//��ü�������� ����
	for (i=0 ; i<iNumStudent-1; ++i)
	{
		for(j=i; j<iNumStudent; ++j)
		{
			if (
				pBan[i].Total() < pBan[j].Total()														//������
				||	(pBan[i].Total() == pBan[j].Total() && (strcmp(pBan[j].sName, pBan[i].sName)<0))		//������ ���� ��� �̸������� ����
				)
			{
				RankSwap(pBan, i,j);
			}
		}
	}

	//��� �ű��
	RankScore(pBan, iNumStudent);




	// �ؽ�Ʈ ���Ϸ� ��� ���
	fp = LnFile_fopen("data/ScoreResult.txt", "wt");

	LnFile_fprintf(fp,"%d\n", iNumStudent);

	for(i=0 ;i<iNumStudent;++i)
	{
		LnFile_fprintf(fp,"%s", pBan[i].sName);
		LnFile_fprintf(fp,"	%s", pBan[i].sIdNum);
		LnFile_fprintf(fp,"	%d", pBan[i].iKor);
		LnFile_fprintf(fp,"	%d", pBan[i].iEng);
		LnFile_fprintf(fp,"	%d", pBan[i].iMth);
		LnFile_fprintf(fp,"	%4d", pBan[i].Total());
		LnFile_fprintf(fp,"	%3.3f", pBan[i].Avg());
		LnFile_fprintf(fp,"	%2d", pBan[i].Rank);
		LnFile_fprintf(fp,"\n");
	}

	LnFile_fclose(fp);


	// ���� ���Ϸ� ��� ���
	fp = LnFile_fopen("data/ScoreResult.brn", "wb");
	LnFile_fwrite(fp, &iNumStudent, sizeof(int) , 1);

	for(i=0 ;i<iNumStudent;++i)
	{
		CStudent	tmp;
		tmp = pBan[i];
		LnFile_fwrite(fp, &tmp, sizeof(CStudent) , 1);
	}

	LnFile_fclose(fp);

	SAFE_DELETE_ARRAY(	pBan	);												//new�� �޸𸮸� �� ���� ���� ����

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

//��� �ű�� ��..
void RankScore(CStudent * pBan, int nStNumber)
{

	int i, nTempRank, nTempTotal;

	pBan[0].Rank = 1;

	nTempRank = 1;
	nTempTotal = pBan[0].Total ();

	for (i=1; i< nStNumber; ++i)
	{


		if (pBan[i].Total () == nTempTotal)			//������ ������?
		{
			pBan[i].Rank = nTempRank;				//Rank�� �����ϰ� �Ѵ�.
		}

		else
		{
			nTempRank = i+1;
			pBan[i].Rank = nTempRank ;
			nTempTotal = pBan[i].Total ();
		}
	}



	printf("\n\n��ü ���������� ����\n");
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

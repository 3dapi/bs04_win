#include <stdio.h>
#include <malloc.h>
#include <windows.h>
#include <time.h>

void McMem_Sleep(clock_t);

void McMem_NewOperator();
void McMem_Malloc();
void McMem_Malloc_Memset();
void McMem_Calloc();
void McMem_GlobalAlloc();
void McMem_HeapAlloc();

int		g_nLoop;
int		g_nCnt;
DWORD	g_dSize;
clock_t	g_dBgn;
clock_t	g_dEnd;
double  duration;

class CTst1
{
public:
	char	a;
	int		b;
	double	c;
	CTst1() : a(0), b(0), c(0){}
};


struct CTst2
{
public:
	char	a;
	int		b;
	double	c;

	CTst2() : a(0), b(0), c(0){}
	virtual ~CTst2(){}
	virtual	void	SetV1(int _b)	{	b= _b;	}
};


struct CTst3 : public CTst2
{
	char	d;
	CTst3() : d(0){}
	virtual ~CTst3(){}
	virtual	void	SetV1(double _c)	{	c= _c;	}
};


typedef BYTE	TypeTst;


int main()
{
	printf( "\n\nMemory alloc time check\n\n");

	g_nCnt	= 1024;
	g_dSize	= DWORD(g_nCnt* sizeof(TypeTst));
	g_nLoop	= 300000000/g_dSize;
	printf( "Small memory size: %lu KB	g_nLoop: %d\n\n", g_dSize/1024, g_nLoop);

	McMem_NewOperator();
	McMem_Malloc();
	McMem_Malloc_Memset();
	McMem_Calloc();
	McMem_HeapAlloc();
	McMem_GlobalAlloc();


	g_nCnt	= 64 * 1024;
	g_dSize	= DWORD(g_nCnt* sizeof(TypeTst));
	g_nLoop	= 300000000/g_dSize;
	printf( "\n\nMiddle memory size: %lu KB	g_nLoop: %d\n\n", g_dSize/(1024), g_nLoop);

	McMem_NewOperator();
	McMem_Malloc();
	McMem_Malloc_Memset();
	McMem_Calloc();
	McMem_HeapAlloc();
	McMem_GlobalAlloc();


	g_nCnt	= 512 * 1024;
	g_dSize	= DWORD(g_nCnt* sizeof(TypeTst));
	g_nLoop	= 300000000/g_dSize;
	printf( "\n\nMiddle memory size: %lu KB	g_nLoop: %d\n\n", g_dSize/(1024), g_nLoop);

	McMem_NewOperator();
	McMem_Malloc();
	McMem_Malloc_Memset();
	McMem_Calloc();
	McMem_HeapAlloc();
	McMem_GlobalAlloc();


	g_nCnt	= 4 * 1024 * 1024;
	g_dSize	= DWORD(g_nCnt* sizeof(TypeTst));
	g_nLoop	= 300000000/g_dSize;
	printf( "\n\nLarge memory size: %lu MB	g_nLoop: %d\n\n", g_dSize/(1024*1024), g_nLoop);

	McMem_NewOperator();
	McMem_Malloc();
	McMem_Malloc_Memset();
	McMem_Calloc();
	McMem_HeapAlloc();
	McMem_GlobalAlloc();


	g_nCnt	= 64 * 1024 * 1024;
	g_dSize	= DWORD(g_nCnt* sizeof(TypeTst));
	g_nLoop	= 300000000/g_dSize * 5;
	printf( "\n\nLarge memory size: %lu MB	g_nLoop: %d\n\n", g_dSize/(1024*1024), g_nLoop);

	McMem_NewOperator();
	McMem_Malloc();
	McMem_Malloc_Memset();
	McMem_Calloc();
	McMem_HeapAlloc();
	McMem_GlobalAlloc();

	return 1;
}



void McMem_NewOperator()
{
	TypeTst*	sBuf= NULL;
	int k;
	printf( "Start new operator..		");
	McMem_Sleep( (clock_t)(1.f * CLOCKS_PER_SEC) );

	g_dBgn = clock();

	for(k=0; k<g_nLoop; ++k)
	{
		sBuf = new TypeTst[ g_dSize ];

		for(INT i=0; i<g_nCnt; ++i)
		{
//			sBuf[i].a = char(i);
			sBuf[i] = char(i);
		}

		delete [] sBuf;
		sBuf = NULL;
	}


	g_dEnd = clock();
	duration = (double)(g_dEnd - g_dBgn);
	printf( "%12.4f mili-seconds\n", duration );
}




void McMem_Malloc()
{
	TypeTst*	sBuf= NULL;
	int k;
	printf( "Start malloc..			");
	McMem_Sleep( (clock_t)(2 * CLOCKS_PER_SEC) );

	g_dBgn = clock();

	for(k=0; k<g_nLoop; ++k)
	{
		sBuf = (TypeTst*)malloc( g_dSize );

		for(INT i=0; i<g_nCnt; ++i)
		{
//			sBuf[i].a = char(i);
			sBuf[i] = char(i);
		}

		free(sBuf);
		sBuf = NULL;
	}


	g_dEnd = clock();
	duration = (double)(g_dEnd - g_dBgn);
	printf( "%12.4f mili-seconds\n", duration );
}


void McMem_Malloc_Memset()
{
	TypeTst*	sBuf= NULL;
	int k;
	printf( "Start malloc+memset..		");
	McMem_Sleep( (clock_t)(2 * CLOCKS_PER_SEC) );

	g_dBgn = clock();

	for(k=0; k<g_nLoop; ++k)
	{
		sBuf = (TypeTst*)malloc( g_dSize );
		memset(sBuf, 0, g_dSize);

		for(INT i=0; i<g_nCnt; ++i)
		{
//			sBuf[i].a = char(i);
			sBuf[i] = char(i);
		}

		free(sBuf);
		sBuf = NULL;
	}


	g_dEnd = clock();
	duration = (double)(g_dEnd - g_dBgn);
	printf( "%12.4f mili-seconds\n", duration );
}


void McMem_Calloc()
{
	TypeTst*	sBuf= NULL;
	int k;
	printf( "Start calloc..			");
	McMem_Sleep( (clock_t)(2 * CLOCKS_PER_SEC) );

	g_dBgn = clock();

	for(k=0; k<g_nLoop; ++k)
	{
		sBuf = (TypeTst*)calloc( g_nCnt ,sizeof(TypeTst));

		for(INT i=0; i<g_nCnt; ++i)
		{
//			sBuf[i].a = char(i);
			sBuf[i] = char(i);
		}

		free(sBuf);
		sBuf = NULL;
	}


	g_dEnd = clock();
	duration = (double)(g_dEnd - g_dBgn);
	printf( "%12.4f mili-seconds\n", duration );
}


void McMem_GlobalAlloc()
{
	TypeTst*	sBuf= NULL;
	int k;
	HGLOBAL hMem = 0;
	printf( "Start GlobalMemory Allocation..	");
	McMem_Sleep( (clock_t)(1.f * CLOCKS_PER_SEC) );

	g_dBgn = clock();

	for(k=0; k<g_nLoop; ++k)
	{
		hMem = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, g_dSize);
		sBuf = (TypeTst*)GlobalLock(hMem);

		for(INT i=0; i<g_nCnt; ++i)
		{
//			sBuf[i].a = char(i);
			sBuf[i] = char(i);
		}

		GlobalUnlock(hMem);
		GlobalFree(hMem);
		sBuf = NULL;
		hMem = NULL;
	}

	g_dEnd = clock();
	duration = (double)(g_dEnd - g_dBgn);
	printf( "%12.4f mili-seconds\n", duration );
}


void McMem_HeapAlloc()
{
	HANDLE		hHeap = 0;
	TypeTst*	sBuf = 0;
	int k;

	McMem_Sleep( (clock_t)(1.f * CLOCKS_PER_SEC) );

	printf( "Start HeapAlloc Allocation..	");
	g_dBgn = clock();

	for(k=0; k<g_nLoop; ++k)
	{
		hHeap = HeapCreate(0, g_dSize, 0);
		sBuf = (TypeTst*)HeapAlloc(hHeap, 0, g_dSize);

		for(INT i=0; i<g_nCnt; ++i)
		{
//			sBuf[i].a = char(i);
			sBuf[i] = char(i);
		}

		HeapFree(hHeap, 0, sBuf);
		sBuf = NULL;
		HeapDestroy(hHeap);
	}

	g_dEnd = clock();
	duration = (double)(g_dEnd - g_dBgn);
	printf( "%12.4f mili-seconds\n", duration );
}





void McMem_Sleep( clock_t w)
{
	clock_t goal;
	goal = w + clock();
	while( goal > clock() )
		;
}

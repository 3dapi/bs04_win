#pragma comment(lib, "Winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>

#include <malloc.h>

INT		i=0, j=0;
INT		iN;
INT		iMemSize;
DWORD	dBgn;
DWORD	dEnd;


void Memory_Alloc();
void Virtual_Alloc();
void Global_Alloc();
void Heap_Alloc();


void main()
{
	iN=100;
	iMemSize = 8 * (100 * 1000 * 1000/iN);
	printf("MemoryBlock: %d Byte\n", iMemSize);
	printf("Total Count: %d\n", iN);

//	Memory_Alloc();
//	Virtual_Alloc();
	Global_Alloc();
	Heap_Alloc();
}





void Memory_Alloc()
{
	char*	pMem = NULL;
	INT		iSize =0;

	printf("\n");
	printf("-- new funciton\n");

	dBgn = timeGetTime();

	for(i=0; i<iN; ++i)
	{
		pMem = new char[iMemSize];
		delete [] pMem;
	}

	dEnd= timeGetTime();

	printf("Elapse Time: %d ms\n", dEnd- dBgn);






	printf("\n");
	printf("-- malloc funciton\n");

	dBgn = timeGetTime();

	for(i=0; i<iN; ++i)
	{
		pMem = (char*)malloc(iMemSize);
		free(pMem);
	}

	dEnd= timeGetTime();

	printf("Elapse Time: %d ms\n", dEnd- dBgn);




//	printf("\n");
//	printf("-- malloc + memset funciton\n");
//
//	dBgn = timeGetTime();
//
//	for(i=0; i<iN; ++i)
//	{
//		pMem = (char*)malloc(iMemSize);
//		memset(pMem, 0, iMemSize);
//		free(pMem);
//	}
//
//	dEnd= timeGetTime();
//
//	printf("Elapse Time: %d ms\n", dEnd- dBgn);






	printf("\n");
	printf("-- calloc funciton\n");

	dBgn = timeGetTime();

	for(i=0; i<iN; ++i)
	{
		pMem = (char*)calloc(iMemSize, 1);
		free(pMem);
	}

	dEnd= timeGetTime();

	printf("Elapse Time: %d ms\n", dEnd- dBgn);







	printf("\n");
	printf("-- realloc function\n");

	pMem = (char*)malloc(iMemSize);

	iSize = _msize( pMem );
	printf("Memory Size Before: %d", iSize);

	pMem = (char*)realloc(pMem, iMemSize + 4000000);

	iSize = _msize( pMem );
	printf("    After: %d\n", iSize);

	free(pMem);
}

void Virtual_Alloc()
{
	printf("\n");
	printf("-- WINAPI VirtualAlloc funciton\n");


	// 메모리 주소 공간 예약(MEM_RESERVE)
	INT	iSmallSize = 12 * sizeof(char);
	char* pBegin = (char*)VirtualAlloc(NULL, iMemSize, MEM_RESERVE, PAGE_READWRITE);

	//MEM_RESERVE일 경우 주소 공간만 할당한 것이라 에러
	//memset(pBegin, 0, iMemSize);

	//예약 된 메모리 사용하기
	char* pNext= (char*)VirtualAlloc(pBegin, iSmallSize, MEM_COMMIT, PAGE_READWRITE);


	memset(pNext, 0xFF, iSmallSize);

	VirtualFree(pBegin, iSmallSize, MEM_DECOMMIT);
	VirtualFree(pBegin, iMemSize, MEM_RELEASE);


	pBegin = (char*)VirtualAlloc(NULL, iMemSize, MEM_RESERVE| MEM_COMMIT, PAGE_READWRITE);
	memset(pBegin, 0xAA, iMemSize);
	VirtualFree(pBegin, iSmallSize, MEM_DECOMMIT);
}



void Global_Alloc()
{
	printf("\n");
	printf("-- WINAPI GlobalAlloc funciton\n");

	HGLOBAL hMem;
	char*	pMem = NULL;

	dBgn = timeGetTime();

	for(i=0; i<iN; ++i)
	{
		pMem=(char*)GlobalAlloc(GMEM_FIXED, iMemSize);
		GlobalFree(pMem);
	}

	dEnd= timeGetTime();

	// if uFlag is GMEM_FIXED, it likes malloc
	pMem=(char*)GlobalAlloc(GMEM_FIXED, iMemSize);
	memset(pMem, 0xFF, iMemSize);
	GlobalFree(pMem);

	printf("Elapse Time: %d ms\n", dEnd- dBgn);



	// if uFlag is GMEM_MOVEABLE, it need Lock/Unlock

	dBgn = timeGetTime();

	for(i=0; i<iN; ++i)
	{
		hMem=GlobalAlloc(GMEM_MOVEABLE, iMemSize);
		pMem=(char*)GlobalLock(hMem);
//		memset(pMem, 0, iMemSize);
		GlobalUnlock(hMem);
		GlobalFree(hMem);
	}

	dEnd= timeGetTime();

	printf("Elapse Time: %d ms\n", dEnd- dBgn);
}



void Heap_Alloc()
{
	printf("\n");
	printf("-- WINAPI HeapAlloc funciton\n");

	char*	pMem = NULL;

	HANDLE hMem;

//	hMem = HeapCreate( 0, 0, 0);
	hMem = HeapCreate( 0, iMemSize, 0);
//	hMem = GetProcessHeap();

	dBgn = timeGetTime();



	for(i=0; i<iN; ++i)
	{
		// Win32 힙 함수를 사용한 메모리 할당
		pMem=(char*)HeapAlloc(hMem, 0, iMemSize);
		memset(pMem, 0, iMemSize);
		HeapFree(GetProcessHeap(), 0, pMem);
	}

	dEnd= timeGetTime();

	printf("Elapse Time: %d ms\n", dEnd- dBgn);

	HeapDestroy(hMem);
}

// Event Test
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <process.h>
#include <stdio.h>


HANDLE		m_hTh	= 0;									// Thread Handle
DWORD		m_dTh	= 0;
HANDLE		m_Evnt	= 0;									// Event Handle

DWORD WINAPI WorkProc(void* pParam);

void main()
{
	// Event Create
	m_Evnt = CreateEvent(
			NULL
		,	FALSE	// 이 부분이 TRUE면 WaitForSingleObject 후에도 계속 Signaled가 돈다. 수동으로 ResetEvent를 호출해야 한다.
		,	TRUE	// 처음 시작을 Signaled로 할것인가 결정한다.
		,	NULL
		);


//	SetEvent(m_Evnt);

	m_hTh = (HANDLE)_beginthreadex(NULL, 0
		, (unsigned (__stdcall*)(void*))WorkProc
		, NULL, 0, (unsigned*)&m_dTh);


//	WaitForSingleObject(m_hTh, INFINITE);
	WaitForSingleObject(m_hTh, 5000);

	CloseHandle(m_Evnt);
	CloseHandle(m_hTh);
}



//입 출력 완료 처리
DWORD WINAPI WorkProc(void* pParam)
{
	INT	hr=-1;

	while(1)
	{
		WaitForSingleObject(m_Evnt, INFINITE);

		printf("Process event\n");

//		ResetEvent(m_Evnt);							// Manual Reset Mode

//		SetEvent(m_Evnt);							// Reseume event
	}

	_endthreadex(0);

	return 1;
}


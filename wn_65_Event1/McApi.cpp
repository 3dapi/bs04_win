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
		,	FALSE	// �� �κ��� TRUE�� WaitForSingleObject �Ŀ��� ��� Signaled�� ����. �������� ResetEvent�� ȣ���ؾ� �Ѵ�.
		,	TRUE	// ó�� ������ Signaled�� �Ұ��ΰ� �����Ѵ�.
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



//�� ��� �Ϸ� ó��
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


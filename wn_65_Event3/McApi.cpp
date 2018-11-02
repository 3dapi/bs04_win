// Event Test
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <process.h>
#include <stdio.h>


HANDLE		m_hTh		= 0;									// Thread Handle
DWORD		m_dTh		= 0;
HANDLE		m_Evnt[8]	= {0};									// Event Handle

DWORD WINAPI WorkProc(void* pParam);

void main()
{
	// Event Create
	m_Evnt[0] = CreateEvent(
			NULL
		,	FALSE	// �� �κ��� TRUE�� WaitForSingleObject �Ŀ��� ��� Signaled�� ����. �������� ResetEvent�� ȣ���ؾ� �Ѵ�.
		,	FALSE	// ó�� ������ Signaled�� �Ұ��ΰ� �����Ѵ�.
		,	NULL
		);


	m_Evnt[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Evnt[2] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Evnt[3] = CreateEvent(NULL, FALSE, FALSE , NULL);
	m_Evnt[4] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Evnt[5] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Evnt[6] = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_Evnt[7] = CreateEvent(NULL, FALSE, FALSE, NULL);


	SetEvent(m_Evnt[rand()%8]);

	m_hTh = (HANDLE)_beginthreadex(NULL, 0
		, (unsigned (__stdcall*)(void*))WorkProc
		, NULL, 0, (unsigned*)&m_dTh);


//	WaitForSingleObject(m_hTh, INFINITE);
	WaitForSingleObject(m_hTh, 10000);

	CloseHandle(m_Evnt);
	CloseHandle(m_hTh);
}



//�� ��� �Ϸ� ó��
DWORD WINAPI WorkProc(void* pParam)
{
	INT	nIdx =-1;

	while(1)
	{
		nIdx = WaitForMultipleObjects(8, m_Evnt, FALSE, INFINITE);

		nIdx -= WAIT_OBJECT_0;

		printf("Process event: %d\n", nIdx);


		INT nRnd = rand()%8;
		printf("Random Event Index: %d    ", nRnd);
		SetEvent(m_Evnt[nRnd]);		// ������ �̺�Ʈ�� Ȱ��ȭ���� ����.

		Sleep(300);
	}

	_endthreadex(0);

	return 1;
}


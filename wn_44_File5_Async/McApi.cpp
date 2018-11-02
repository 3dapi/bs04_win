#include <windows.h>
#include <stdio.h>


#define MAX_BUFSIZE	2000000

HANDLE		g_hFile;

DWORD		g_dBuf;


//�Ϸᰡ ���� ������ �� �޸𸮴� ��ȿ�ؾ� �ϹǷ�...
// �����Ƽ� �׳� ��������..

OVERLAPPED	g_OL;


void main()
{
	printf("�񵿱� I/O\n");

	// 1. ������ �����.
	g_hFile=CreateFile("Test.bin",GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);

	if(INVALID_HANDLE_VALUE == g_hFile)
	{
		return;
	}

	// �ӽ� ������
	BYTE*	pBuf = (BYTE*)malloc( MAX_BUFSIZE );

	memset(&g_OL, 0, sizeof g_OL);


	g_OL.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);


	if(WriteFile(g_hFile, pBuf, MAX_BUFSIZE, &g_dBuf, &g_OL))
	{
		printf("Write Complete\n");
	}

	else
	{
		DWORD hr = GetLastError();

		if(ERROR_IO_PENDING !=hr)
		{
			printf("IO Error\n");

			//	�ѹ��� �� �����Ͱ� �ʹ� ũ��
			//	ERROR_NO_SYSTEM_RESOURCES �޽����� �޴´�.
			return;
		}
	}


	while(1)
	{
		BOOL hr = GetOverlappedResult(g_hFile, &g_OL, &g_dBuf, FALSE);

		if(!hr)
		{
			DWORD hr = GetLastError();

			// GetOverlappedResult(g_hFile, &g_OL, &g_dBuf, FALSE);���� ��������
			// FALSE�̸�
			// ERROR_IO_INCOMPLETE �޽����� �޴´�. <-- �� MSDN�� Ȯ���غ� ��.

			if( ERROR_IO_PENDING !=hr && ERROR_IO_INCOMPLETE != hr)
			{
				LPVOID lpMsgBuf;

				FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					hr,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &lpMsgBuf,
					0,
					NULL
					);

				printf("IO Error(%ld): %s\n", hr, (char*)lpMsgBuf);

				// Free the buffer.
				LocalFree( lpMsgBuf );

				break;
			}

			else
			{
				printf("While Writting...\n");
			}
		}

		else
		{
			printf("Wait IO Complete. Written: %ld\n", g_dBuf);

			break;
		}
	}

	CloseHandle(g_OL.hEvent);
	CloseHandle(g_hFile);

	free(pBuf);
}
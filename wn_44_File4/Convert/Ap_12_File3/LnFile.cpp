//
//
////////////////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>

#include "LnFile.h"


LNFILE LnFile_fopen(char* sFile, char* sT)
{
	// rt, rb, wt, wb만 지원하자.

	LNFILE hFile = NULL;

	DWORD					dwDesiredAccess=0;
	DWORD					dwShareMode=0;
	LPSECURITY_ATTRIBUTES	lpSecurityAttributes=NULL;
	DWORD					dwCreationDispostion=0;
	DWORD					dwFlagsAndAttributes=0;
	LNFILE					hTemplateFile = NULL;


	char sUsage[4];

	strcpy(sUsage, sT);

	strcat(sUsage, " ");

	// 1. Desired Access
	if('r' == sUsage[0])
		dwDesiredAccess = GENERIC_READ;

	else if('w' == sUsage[0])
		dwDesiredAccess = GENERIC_WRITE;

	else if('a' == sUsage[0])
		dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;


	// 2. Share Mode
	//	if('r' == sUsage[0])
	//		dwShareMode = FILE_SHARE_READ;
	//
	//	else if('w' == sUsage[0])
	//		dwShareMode = FILE_SHARE_WRITE;
	//
	//	else if('a' == sUsage[0])
	//		dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;

	//3. Security Attributes
	lpSecurityAttributes=NULL;


	// 4. Creation Deposition
	if(GENERIC_READ == dwDesiredAccess)
		dwCreationDispostion = OPEN_EXISTING;

	else if(GENERIC_WRITE == dwDesiredAccess)
		dwCreationDispostion = CREATE_ALWAYS;


	// 5. Flags and Attributes
	dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;	// The file has no other attributes set.

	// 6. Template File
	hTemplateFile = NULL;


	hFile = CreateFile(
			sFile						// open File
		,	dwDesiredAccess				// GENERIC_READ or GENERIC_WRITE....
		,	dwShareMode					// share for reading FILE_SHARE_READ, FILE_SHARE_WRITE
		,	lpSecurityAttributes		// security
		,	dwCreationDispostion		// existing ?
		,	dwFlagsAndAttributes		// normal file
		,	hTemplateFile				// no attr. template
		);

	if(INVALID_HANDLE_VALUE == hFile)
		return NULL;

	return hFile;
}


INT	LnFile_fclose(LNFILE hFile)
{
	if(NULL == hFile)
		return -1;

	return CloseHandle(hFile) ? 1: -1;
}


DWORD LnFile_fsize(LNFILE hFile)
{
	DWORD size= ::GetFileSize(hFile,NULL);

	return size;
}


INT	LnFile_fread(LNFILE hFile, void* p, INT bufSize, INT nCount)
{
	DWORD iSize = bufSize * nCount;
	DWORD dRead=0;
	DWORD dError;

	BOOL hr = ReadFile(hFile, p, iSize, &dRead, NULL);

	if(0==hr)
	{
		// hr==-1 is End of File
		dError = GetLastError();

		if(NO_ERROR != dError)
		{
			LnFile_Err(dError);
		}
	}

	return 0==hr ? -1: dRead;
}



INT	LnFile_fwrite(LNFILE hFile, void* p, INT bufSize, INT nCount)
{
	DWORD iSize = bufSize * nCount;
	DWORD dWritten=0;
	DWORD dError;

	INT hr = WriteFile(hFile, p, iSize, &dWritten, NULL);

	if(0==hr)
	{
		// hr==-1 is End of File
		dError = GetLastError();

		if(NO_ERROR != dError)
		{
			LnFile_Err(dError);
		}
	}

	return 0==hr ? -1: dWritten;
}


INT LnFile_fprintf(LNFILE hFile, char *format,...)
{
	va_list ap;
	char s[2048];

	if (NULL == format)
		return -1;

	va_start(ap, format);
	vsprintf((char *)s, format, ap);
	va_end(ap);

	if (NULL ==s)
		return -1;


	DWORD dWritten=0;
	DWORD iSize = strlen(s);
	INT hr = WriteFile(hFile, s, iSize, &dWritten, NULL);

	return 0==hr ? -1: dWritten;
}


INT LnFile_fgets(LNFILE hFile, char* sLine, int iMax, BOOL bFillEndLineZero)
{
	INT		nTotal=0;
	DWORD dRead=0;
	INT hr = 0;

	for(int i=0; i<iMax; ++i)
	{
		hr = ReadFile(hFile, &sLine[i], 1, &dRead, NULL);

		nTotal +=dRead;

		// Fail
		if(0==hr)
		{
			hr = LnFile_Err();
			hr = -1;
			break;
		}

		// End of File
		if (hr && 0== dRead)
		{
			hr = -1;
			break;
		}

		// End of Line
//		if('\n' == sLine[i] || '\r' == sLine[i] || '\n\r' == sLine[i] || '\r\n' == sLine[i])
		// window의 텍스트 파일은 '\r\n'값이 들어오더라...
		if('\n' == sLine[i])
		{
			hr = 1;
			sLine[i+1] =0;
			break;
		}
	}

	if(bFillEndLineZero)
	{
		if(i>0)
		{
			if('\r' == sLine[i-1])
				sLine[i-1] =0;
			else
				sLine[i] =0;
		}
	}

	return nTotal? nTotal*hr: hr;
}

DWORD LnFile_fseek(LNFILE hFile, LONG lOff, DWORD nFrom/*FILE_BEGIN, FILE_CURRENT, FILE_END*/)
{
	DWORD dError;
	DWORD dFilePos = ::SetFilePointer(hFile, lOff, NULL, nFrom);

	if(0xFFFFFFFF ==dFilePos)
	{
		dError = GetLastError();

		if(NO_ERROR != dError)
			LnFile_Err(dError);
	}


	return dFilePos;
}

DWORD	LnFile_fseekBegin(LNFILE hFile)
{
	return LnFile_fseek(hFile, 0, FILE_BEGIN);
}

DWORD	LnFile_fseekEnd(LNFILE hFile)
{
	return LnFile_fseek(hFile, 0, FILE_END);
}



DWORD LnFile_ftell(LNFILE hFile)
{
	DWORD dError;
	DWORD dFilePos = ::SetFilePointer(hFile, 0, NULL, FILE_CURRENT);

	if(0xFFFFFFFF ==dFilePos)
	{
		dError = GetLastError();

		if(NO_ERROR != dError)
		{
			LnFile_Err(dError);
		}
	}



	return dFilePos;
}


void LnFile_fsetLength(LNFILE hFile, DWORD dwNewLen)
{
	DWORD dError=0;
	LnFile_fseek(hFile, (LONG)dwNewLen, FILE_BEGIN);

	if(!::SetEndOfFile(hFile))
	{
		dError = GetLastError();

		if(NO_ERROR != dError)
			LnFile_Err(dError);
	}

}

DWORD LnFile_fgetLength(LNFILE hFile)
{
	DWORD dwLen, dwCur;
	dwCur = LnFile_ftell(hFile);
	dwLen = LnFile_fseekEnd(hFile);

	LnFile_fseek(hFile, dwCur, FILE_BEGIN);

	return dwLen;
}








DWORD LnFile_Err()
{
	LPVOID	lpMsgBuf;
	DWORD	dMsgId = GetLastError();

	FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
		,	NULL
		,	dMsgId
		,	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)	// Default language
		,	(LPTSTR) &lpMsgBuf
		,	0,NULL
		);

	printf("%s\n", (char*)lpMsgBuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );

	return dMsgId;
}


void LnFile_Err(DWORD dError)
{
	LPVOID	lpMsgBuf;

	FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS
		,	NULL
		,	dError
		,	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT)	// Default language
		,	(LPTSTR) &lpMsgBuf
		,	0,NULL
		);

	printf("%s\n", (char*)lpMsgBuf);
	// Free the buffer.
	LocalFree( lpMsgBuf );

//	case NO_ERROR:
//		break;
//	case ERROR_FILE_NOT_FOUND:
//		break;
//	case ERROR_PATH_NOT_FOUND:
//		break;
//	case ERROR_TOO_MANY_OPEN_FILES:
//		break;
//	case ERROR_ACCESS_DENIED:
//		break;
//	case ERROR_INVALID_HANDLE:
//		break;
//	case ERROR_BAD_FORMAT:
//		break;
//	case ERROR_INVALID_ACCESS:
//		break;
//	case ERROR_INVALID_DRIVE:
//		break;
//	case ERROR_CURRENT_DIRECTORY:
//		break;
//	case ERROR_NOT_SAME_DEVICE:
//		break;
//	case ERROR_NO_MORE_FILES:
//		break;
//	case ERROR_WRITE_PROTECT:
//		break;
//	case ERROR_BAD_UNIT:
//		break;
//	case ERROR_NOT_READY:
//		break;
//	case ERROR_BAD_COMMAND:
//		break;
//	case ERROR_CRC:
//		break;
//	case ERROR_BAD_LENGTH:
//		break;
//	case ERROR_SEEK:
//		break;
//	case ERROR_NOT_DOS_DISK:
//		break;
//	case ERROR_SECTOR_NOT_FOUND:
//		break;
//	case ERROR_WRITE_FAULT:
//		break;
//	case ERROR_READ_FAULT:
//		break;
//	case ERROR_SHARING_VIOLATION:
//		break;
//	case ERROR_LOCK_VIOLATION:
//		break;
//	case ERROR_WRONG_DISK:
//		break;
//	case ERROR_SHARING_BUFFER_EXCEEDED:
//		break;
//	case ERROR_HANDLE_EOF:
//		break;
//	case ERROR_HANDLE_DISK_FULL:
//		break;
//	case ERROR_DUP_NAME:
//		break;
//	case ERROR_BAD_NETPATH:
//		break;
//	case ERROR_NETWORK_BUSY:
//		break;
//	case ERROR_DEV_NOT_EXIST:
//		break;
//	case ERROR_ADAP_HDW_ERR:
//		break;
//	case ERROR_BAD_NET_RESP:
//		break;
//	case ERROR_UNEXP_NET_ERR:
//		break;
//	case ERROR_BAD_REM_ADAP:
//		break;
//	case ERROR_NO_SPOOL_SPACE:
//		break;
//	case ERROR_NETNAME_DELETED:
//		break;
//	case ERROR_NETWORK_ACCESS_DENIED:
//		break;
//	case ERROR_BAD_DEV_TYPE:
//		break;
//	case ERROR_BAD_NET_NAME:
//		break;
//	case ERROR_TOO_MANY_NAMES:
//		break;
//	case ERROR_SHARING_PAUSED:
//		break;
//	case ERROR_REQ_NOT_ACCEP:
//		break;
//	case ERROR_FILE_EXISTS:
//		break;
//	case ERROR_CANNOT_MAKE:
//		break;
//	case ERROR_ALREADY_ASSIGNED:
//		break;
//	case ERROR_INVALID_PASSWORD:
//		break;
//	case ERROR_NET_WRITE_FAULT:
//		break;
//	case ERROR_DISK_CHANGE:
//		break;
//	case ERROR_DRIVE_LOCKED:
//		break;
//	case ERROR_BUFFER_OVERFLOW:
//		break;
//	case ERROR_DISK_FULL:
//		break;
//	case ERROR_NO_MORE_SEARCH_HANDLES:
//		break;
//	case ERROR_INVALID_TARGET_HANDLE:
//		break;
//	case ERROR_INVALID_CATEGORY:
//		break;
//	case ERROR_INVALID_NAME:
//		break;
//	case ERROR_INVALID_LEVEL:
//		break;
//	case ERROR_NO_VOLUME_LABEL:
//		break;
//	case ERROR_NEGATIVE_SEEK:
//		break;
//	case ERROR_SEEK_ON_DEVICE:
//		break;
//	case ERROR_DIR_NOT_ROOT:
//		break;
//	case ERROR_DIR_NOT_EMPTY:
//		break;
//	case ERROR_LABEL_TOO_LONG:
//		break;
//	case ERROR_BAD_PATHNAME:
//		break;
//	case ERROR_LOCK_FAILED:
//		break;
//	case ERROR_BUSY:
//		break;
//	case ERROR_INVALID_ORDINAL:
//		break;
//	case ERROR_ALREADY_EXISTS:
//		break;
//	case ERROR_INVALID_EXE_SIGNATURE:
//		break;
//	case ERROR_BAD_EXE_FORMAT:
//		break;
//	case ERROR_FILENAME_EXCED_RANGE:
//		break;
//	case ERROR_META_EXPANSION_TOO_LONG:
//		break;
//	case ERROR_DIRECTORY:
//		break;
//	case ERROR_OPERATION_ABORTED:
//		break;
//	case ERROR_IO_INCOMPLETE:
//		break;
//	case ERROR_IO_PENDING:
//		break;
//	case ERROR_SWAPERROR:
//		break;
}
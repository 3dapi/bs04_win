//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _LNFILE_H_
#define _LNFILE_H_


#include <windows.h>
#include <stdio.h>


typedef HANDLE		LNFILE;

DWORD	LnFile_Err();
void	LnFile_Err(DWORD dError);

LNFILE	LnFile_fopen(char* sFile, char* sT);
INT		LnFile_fclose(LNFILE hFile);
DWORD	LnFile_fsize(LNFILE hFile);

INT		LnFile_fread(LNFILE hFile, void* p, INT bufSize, INT nCount);
INT		LnFile_fwrite(LNFILE hFile, void* p, INT bufSize, INT nCount);
INT		LnFile_fprintf(LNFILE hFile, char *format,...);
INT		LnFile_fgets(LNFILE hFile, char* sLine, int iMax, BOOL bFillEndLineZero=TRUE);
DWORD	LnFile_fseek(LNFILE hFile, LONG lOff, DWORD nFrom/*FILE_BEGIN, FILE_CURRENT, FILE_END*/);
DWORD	LnFile_fseekBegin(LNFILE hFile);
DWORD	LnFile_fseekEnd(LNFILE hFile);

void	LnFile_fsetLength(LNFILE hFile, DWORD dwNewLen);
DWORD	LnFile_fsetLength(LNFILE hFile);

DWORD	LnFile_ftell(LNFILE hFile);

#endif
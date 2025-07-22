
#include <windows.h>
#include <stdio.h>

#include "LnFile.h"



void main()
{
//	printf("%d\n", '\n');
//	printf("%d\n", '\r');
//	printf("%d\n", '\r\n');

	printf("For WinAPI File Management From C Style Functions\n\n\n");

	LNFILE	fp = LnFile_fopen("test.txt", "rt");
	INT		iFileSize = LnFile_fsize(fp);
	char	sLine[512];

	INT hr = 0;
	INT iSize =0;

	while(1)
	{
		hr = LnFile_fgets(fp, sLine, 512);
		printf("%s\n", sLine);


		// End of File
		if(FAILED(hr))
		{
			iSize += abs(hr);
			break;
		}

		else
		{
			iSize +=hr;
		}
	}

	LnFile_fseekEnd(fp);
	int iFileLen = LnFile_ftell(fp);

	printf("%d\n", iFileLen);

	LnFile_fclose(fp);
}

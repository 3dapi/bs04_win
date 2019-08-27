#include <string>

#if defined(WIN32)
  #pragma comment(lib, "User32.lib")
  #include <crtdbg.h>
  #include <Windows.h>
  #include <direct.h>
  #include <strsafe.h>
#else
  #include <dirent.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(WIN32)
  #define FOLDER_DELIMITER "\\"
#else
  #define FOLDER_DELIMITER "/"
#endif

static int remove_all_sub_dir(const std::string& root)
{
	int             err;
	const int       DIR_LEN = 1024;
	char            root_bgn[DIR_LEN + 4]={0};
	std::string     root_dir;

#if defined(WIN32)
	WIN32_FIND_DATA find_data;
	HANDLE          find_handle = INVALID_HANDLE_VALUE;
#else
	struct dirent*  find_data = NULL;
	DIR*            find_handle = NULL;
#endif

#if defined(WIN32)
	if(!GetFullPathNameA(root.c_str(), DIR_LEN, &root_bgn[0], NULL))
	{
		return -1;
	}
	root_dir = root_bgn;
	strcat_s(root_bgn, DIR_LEN, "\\*");
	
	find_handle = FindFirstFileA(root_bgn, &find_data);
	if(INVALID_HANDLE_VALUE == find_handle)
	{
		return -1;
	}
#else
	if(!realpath(root.c_str(), &root_bgn[0]))
	{
		return -1;
	}
	root_dir = root_bgn;

	find_handle = opendir(root_bgn);
	if(!find_handle)
	{
		return -1;
	}
	find_data = readdir(find_handle);
	if(!find_data)
	{
		return -1;
	}
#endif

	do
	{
#if defined(WIN32)
		const char* d_name = find_data.cFileName;
		bool is_dir  = (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ? true:false;
		bool is_file = true;
#else
		const char* d_name = find_data->d_name;
		bool is_dir  = DT_DIR == find_data->d_type;
		bool is_file = DT_REG == find_data->d_type;
#endif
		if(!d_name || !strcmp(d_name, ".") || !strcmp(d_name, "..") )
		{
			continue;
		}

		if(is_dir)
		{
			//printf("    <folder>: %s\n", d_name);
			std::string path_sub = root_dir;
			path_sub += std::string(FOLDER_DELIMITER);
			path_sub += std::string(d_name);
			remove_all_sub_dir(path_sub);
			continue;
		}
		if(is_file)
		{
			std::string path_file = root_dir;
			path_file += std::string(FOLDER_DELIMITER);
			path_file += std::string(d_name);

			err = remove(path_file.c_str());
			printf("remove f[%2d] :  %s\n", err, path_file.c_str());
		}

#if defined(WIN32)
	} while(FALSE != FindNextFileA(find_handle, &find_data));
	FindClose(find_handle);
	err = _rmdir(root_dir.c_str());
#else
	} while(NULL != (find_data = readdir(find_handle)));
	closedir(find_handle);
	err = remove(root_dir.c_str());
#endif
	printf("remove d[%2d] :  %s\n", err, root_dir.c_str());
	return err;
}

int main()
{
	std::string root_dir = "./test_folder";
	int hr = remove_all_sub_dir(root_dir);
	printf("%s folder removed %s\n", root_dir.c_str(), hr? "failed" : "success");
	return 0;
}


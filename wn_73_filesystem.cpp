#include <algorithm>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#if defined(_MSC_VER)
  #if 1900 <= _MSC_VER
    namespace file_sys = std::experimental::filesystem;
  #else
    namespace file_sys = std::tr2::sys;
  #endif
#else
  namespace file_sys = std::filesystem;
#endif

#include <stdio.h>
#include <stdarg.h>
#ifdef _MSC_VER
#define NOMINMAX
#include <windows.h>
#endif

std::string path_fullPath(const std::string &filename)
{
	file_sys::path fs_path(filename);
	auto abs_path = file_sys::absolute(fs_path);
	std::string ret = abs_path.generic_string();
	return ret;
}

std::string path_systemCompletePath(const std::string &filename)
{
	file_sys::path fs_path(filename);
	std::string ret = file_sys::system_complete(fs_path).generic_string();
	return ret;
}

std::string path_fileExtension(const std::string& filePath)
{
	file_sys::path fs_path(filePath);
	std::string ret = fs_path.extension().generic_string();
	return ret;
}

std::string path_filePath(const std::string& filePath)
{
	file_sys::path fs_path(filePath);
	file_sys::path parent_path = fs_path.parent_path();
	std::string ret = parent_path.generic_string();
	return ret;
}

std::string path_fileName(const std::string& filePath)
{
	file_sys::path fs_path(filePath);
	std::string ret = fs_path.stem().generic_string();
	return ret;
}

bool path_fileExist(const std::string& filename)
{
	file_sys::file_status fs_st = file_sys::status(filename);
	auto ret = file_sys::exists(fs_st);
	return ret;
}

bool path_isAbsolutePath(const std::string& path)
{
	file_sys::path fs_path(path);
	bool ret = fs_path.is_absolute();
	return ret;
}

bool path_directoryExist(const std::string& dirPath)
{
	return path_fileExist(dirPath);
}

std::string path_currentPath()
{
	std::string ret = file_sys::current_path().generic_string();
	return ret;
}

std::string path_currentExecPath()
{
	std::string ret;
#ifdef _MSC_VER
	std::string t_path(2048+4,0);
	int str_len = (int)::GetModuleFileNameA(::GetModuleHandleA(NULL), &t_path[0], 2048);
	ret = path_filePath(std::string(t_path.c_str()));
#endif
	return ret;
}

std::string path_relativePath(const std::string& path)
{
	file_sys::path fs_path(path);
	std::string ret = fs_path.relative_path().generic_string();
	return ret;
}


static std::string format_string_t(const char* format, va_list va)
{
	va_list va_copy;
#ifndef va_copy
	va_copy = va;
#else
	va_copy(va_copy, va);
#endif
	int len = vsnprintf(NULL, 0, format, va_copy);
	if(0>= len)
		return std::string("");
	len++;
	std::string ret((len/4 +3)*4, 0);
	len = vsnprintf(&ret[0], len, format, va);
	ret.erase(ret.find_first_of('\0'));
	return ret;
}

std::string formatString(const char* format, ...)
{
	va_list va;
	va_start(va, format);
	std::string ret = ::format_string_t(format, va);
	va_end(va);
	return ret;
}

void path_fileStatus(const std::string& path)
{
	file_sys::path fs_path(path);
	file_sys::file_status fs_st = file_sys::status(fs_path);
	if(file_sys::is_regular_file    (fs_st)) std::cout << " is a regular file\n";
	if(file_sys::is_directory       (fs_st)) std::cout << " is a directory\n";
	if(file_sys::is_symlink         (fs_st)) std::cout << " is a symlink\n";
	if(!file_sys::exists            (fs_st)) std::cout << " does not exist\n";
}

bool path_gatherFolders(const std::string& root)
{
	if(!file_sys::exists(file_sys::path(root)))
		return false;

	file_sys::directory_iterator it_end;		// default construction yields past-the-end
	file_sys::directory_iterator it_cur(root);	// current iterator

	for(; it_cur != it_end; ++it_cur)
	{
		auto fs_st  = it_cur->status();
		auto fs_path= it_cur->path();
		std::string str_path = it_cur->path().generic_string();
		if(is_directory(fs_st))
		{
			std::cout << "path :: " << str_path << "\n";

			path_gatherFolders(fs_path.generic_string());
			continue;
		}

		if(is_regular_file(fs_st))
		{
			std::string ext = fs_path.extension().generic_string();
			std::string base_name = fs_path.stem().generic_string();
			std::cout << "file :: " << str_path << "\n";
		}
	}
	return false;
}

int main()
{
	std::string src_path = "../";
	file_sys::path  cc = path_filePath(src_path);
	std::cout
		<< "path           : " << cc << '\n'
		<< "Absolute       : " << src_path << " is " << path_fullPath(src_path) << '\n'
		<< "System complete: " << src_path << " is " << path_systemCompletePath(src_path) << '\n'
		<< std::endl;

	std::string  str_path = path_currentPath();
	std::cout << "Current path : " << str_path << '\n';
	str_path += "/../";
	auto rel = path_relativePath(str_path);
	std::cout << "Relative path: " << rel << '\n';

	formatString("%s %d %d %d", "안녕하세요 반갑습니다. Hello world welcome  ",1,2,3);
	path_fileStatus("D:/NDK/android-ndk-r17c/ndk-build.cmd");
	path_gatherFolders("D:/NDK/android-ndk-r17c/toolchains/llvm/prebuilt/windows-x86_64");
	return 0;
}

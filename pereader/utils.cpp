#include "stdafx.h"
#include "utils.hpp"

#include <windows.h>

std::string Utils::GetFullPathOfModule (const std::string& moduleName) {
	HMODULE handle = LoadLibraryA (moduleName.c_str ());
	if (handle == nullptr) {
		return std::string ();
	}

	const DWORD bufferSize = 64 * 1024;
	char modulePathBuffer[bufferSize];

	DWORD len = GetModuleFileNameA (handle, modulePathBuffer, bufferSize);
	if (GetLastError () == ERROR_INSUFFICIENT_BUFFER || len <= 0) {
		return std::string ();
	}

	return std::string (modulePathBuffer, modulePathBuffer + len);
}

std::string Utils::GetDirectoryOfModule (const std::string& modulePath) {
	std::experimental::filesystem::path path (modulePath);
	return path.parent_path ().string ();
}

std::string Utils::GetFullPathOfModuleAtPath (const std::string& searchPath, const std::string& moduleName) {
	std::experimental::filesystem::path modulePath (searchPath);
	modulePath /= moduleName;

	std::string path = modulePath.string ();
	HMODULE handle = LoadLibraryA (path.c_str ());
	if (handle == nullptr) {
		return std::string ();
	}

	const DWORD bufferSize = 64 * 1024;
	char modulePathBuffer[bufferSize];

	DWORD len = GetModuleFileNameA (handle, modulePathBuffer, bufferSize);
	if (GetLastError () == ERROR_INSUFFICIENT_BUFFER || len <= 0) {
		return std::string ();
	}

	return std::string (modulePathBuffer, modulePathBuffer + len);
}

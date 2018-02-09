#pragma once

class Utils {
public:
	static std::string GetFullPathOfModule (const std::string& moduleName);
	static std::string GetDirectoryOfModule (const std::string & modulePath);
	static std::string GetFullPathOfModuleAtPath (const std::string & searchPath, const std::string & moduleName);
};


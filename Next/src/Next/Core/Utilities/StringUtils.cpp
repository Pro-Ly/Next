#include "nxpch.h"
#include "StringUtils.h"


namespace Next {

	namespace StringUtils{

		std::string GetFileName(const std::string& filePath)
		{
			std::string name;
			size_t found = filePath.find_last_of("/\\");
			name = found != std::string::npos ? filePath.substr(found + 1) : filePath;
			found = name.find_last_of('.');
			name = found != std::string::npos ? name.substr(0, found) : name;
			return name;
		}

	}
}

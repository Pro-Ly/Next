#pragma once

#include <vector>
#include <string>

namespace Next {

	namespace FileUtils {

		std::vector<char> ReadFile(const std::string& filename);
	}
}


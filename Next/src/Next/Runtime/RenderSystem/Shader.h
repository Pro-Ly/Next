#pragma once

#include "Next/Core/Ref.h"

#include <string>
#include <vector>
#include <filesystem>

namespace Next {

	class Shader : public RefCounted
	{
	public:
		Shader(const std::string& filePath);
		Shader(const std::string& vertFilePath, const std::string& fragFilePath);
		virtual ~Shader();

		inline const std::vector<char> GetVertCode() const { return m_VertCode; }
		inline const std::vector<char> GetFragCode() const { return m_FragCode; }

	protected:
		std::filesystem::path m_Path;
		std::string m_Name;
		std::vector<char> m_VertCode;
		std::vector<char> m_FragCode;
	protected:
		std::vector<char> readFile(const std::string& filePath);
	};
}



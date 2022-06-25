#include "nxpch.h"
#include "Shader.h"
#include "Core/Utilities/StringUtils.h"

#include <fstream>

namespace Next {

	//TODO merge vert and frag shader
	Shader::Shader(const std::string& filePath)
		:m_Path(filePath)
	{
		m_Name = StringUtils::GetFileName(filePath);
	}

	//TODO REMOVE TEMP
	Shader::Shader(const std::string& vertFilePath, const std::string& fragFilePath)
	{
		m_Name = StringUtils::GetFileName(vertFilePath);
		m_VertCode = readFile(vertFilePath);
		m_FragCode = readFile(fragFilePath);
	}

	Shader::~Shader()
	{
		m_VertCode.clear();
		m_FragCode.clear();
	}

	std::vector<char> Shader::readFile(const std::string& filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		NX_CORE_ASSERT(file.is_open(), "failed to open file!");

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}
}
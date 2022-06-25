#pragma once

#include "VertexBuffer.h"
#include "Core/Ref.h"

#include <vector>
#include <string>
namespace Next {

	class Model : public RefCounted
	{
	public:
		Model();
		virtual ~Model();

		void LoadObjFile(const std::string& filePath);
		
		inline const std::vector<Vertex> GetVertices() const { return m_Vertices; }
		inline const std::vector<uint32_t> GetIndices() const { return m_Indices; }

	private:
		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};
}



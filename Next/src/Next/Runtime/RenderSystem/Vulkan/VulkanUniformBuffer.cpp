#include "nxpch.h"
#include "VulkanUniformBuffer.h"
#include "Runtime/RenderSystem/RenderSystem.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Next {

	VulkanUniformBuffer::VulkanUniformBuffer(size_t bufferSize)
		:m_BufferSize(bufferSize)
	{
		VulkanAllocator allocator("UniformBuffer");

		//Create Uniform Buffer

		VkBufferCreateInfo vertexBufferCreateInfo{};
		vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vertexBufferCreateInfo.size = m_BufferSize;
		vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		vertexBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		m_Allocation = allocator.AllocateBuffer(vertexBufferCreateInfo, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_Buffer);
	}

	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		Release();
	}

	void VulkanUniformBuffer::Update()
	{
		UniformBufferObject ubo{};

		float time = RenderSystem::GetCurTime();

		auto width = Application::Get().GetWindow().GetWidth();
		auto height = Application::Get().GetWindow().GetHeight();
		//TODO : USE CAMERA
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		ubo.proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 10.0f);

		auto res = glm::mat4x4(1.0f) * ubo.proj * ubo.view * ubo.model;


		VulkanAllocator allocator("UniformBuffer");

		uint8_t* destData = allocator.MapMemory<uint8_t>(m_Allocation);
		memcpy(destData, &ubo, sizeof(ubo));
		allocator.UnmapMemory(m_Allocation);
	}

	void VulkanUniformBuffer::Release()
	{
		if (m_Allocation)
		{
			VulkanAllocator allocator("UniformBuffer");
			allocator.DestroyBuffer(m_Buffer, m_Allocation);
			m_Allocation = nullptr;
		}
	}

}
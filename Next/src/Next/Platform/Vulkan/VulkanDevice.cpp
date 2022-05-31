#include "nxpch.h"
#include "VulkanDevice.h"
#include "Next/Core/Ref.h"

namespace Next {

	VulkanPhycicalDevice::VulkanPhycicalDevice()
	{
		
	}

	VulkanPhycicalDevice::~VulkanPhycicalDevice()
	{

	}

	Next::Ref<VulkanPhycicalDevice> VulkanPhycicalDevice::Select()
	{
		return Ref<VulkanPhycicalDevice>::Create();
	}

}
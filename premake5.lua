workspace "Next"
    architecture "x64"
    startproject "GameEngine"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder {solution directory}
IncludeDir = {}
IncludeDir["GLFW"] = "Next/vendor/GLFW/include"
IncludeDir["Glad"] = "Next/vendor/Glad/include"
IncludeDir["ImGui"] = "Next/vendor/imgui"
IncludeDir["glm"] = "Next/vendor/glm"
IncludeDir["stb_image"] = "Next/vendor/stb_image"
IncludeDir["tiny_obj_loader"] = "Next/vendor/tiny_obj_loader"
IncludeDir["VulkanSDK"] = "Next/vendor/Vulkan"

group "Dependecies"
    include "Next/vendor/GLFW"
    include "Next/vendor/Glad"
    include "Next/vendor/imgui"

group ""

project "Next"
    location "Next"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "nxpch.h"
    pchsource "Next/src/nxpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/tiny_obj_loader/**.h",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/Vulkan/Include/**.h",
        "%{prj.name}/vendor/Vulkan/Include/**.hpp",
        "%{prj.name}/vendor/VulkanMemoryAllocator/**.h",
        "%{prj.name}/vendor/VulkanMemoryAllocator/**.cpp",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLM_FORCE_RADIANS",
        "GLM_FORCE_LEFT_HANDED",
        "GLM_FORCE_DEPTH_ZERO_TO_ONE",
    }

    includedirs
    {
        "%{prj.name}/src/",
        "%{prj.name}/src/Next/",
        "%{prj.name}/src/Next/vendor/",
        "%{prj.name}/vendor/",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.tiny_obj_loader}",
        "%{IncludeDir.VulkanSDK}/Include",
    }

    libdirs
    {
        "%{IncludeDir.VulkanSDK}/Lib"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "Dwmapi.lib",
        "Vulkan-1.lib",
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

        defines
        {
            "NX_PLATFORM_WINDOWS",
            "NX_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
        }

        --postbuildcommands
        --{
        --    ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/GameEngine/\"")
        --}

    filter "configurations:Debug"
        defines "NX_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "NX_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "NX_DIST"
        runtime "Release"
        optimize "on"

project "GameEngine"
    location "GameEngine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    includedirs
    {
        "Next/vendor/spdlog/include",
        "Next/src",
        "Next/vendor",
        "%{IncludeDir.glm}",
    }

    links
    {
        "Next"
    }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"

        defines
        {
            "NX_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "NX_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "NX_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "NX_DIST"
        runtime "Release"
        optimize "on"

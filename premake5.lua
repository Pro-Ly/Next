workspace "Next"
    architecture "x64"
    startproject "Sandbox"

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

group "Dependecies"
    include "Next/vendor/GLFW"
    include "Next/vendor/Glad"
    include "Next/vendor/imgui"

group ""

project "Next"
    location "Next"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "hzpch.h"
    pchsource "Next/src/hzpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "Dwmapi.lib",
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"

        defines
        {
            "NX_PLATFORM_WINDOWS",
            "NX_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
        }

        --postbuildcommands
        --{
        --    ("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
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

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
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
        cppdialect "C++17"
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

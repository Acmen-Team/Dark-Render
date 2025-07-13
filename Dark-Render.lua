workspace "Dark-Render"
	architecture "x86_64"
	startproject "Render-Example"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder(solution directory)
IncludeDir = {}
IncludeDir["DarkToolsHeaders"] = "Dark-Render/vendor/Dark-Tools-Headers/Dark-Tools/include"
IncludeDir["DarkResourcesHeaders"] = "Dark-Render/vendor/Dark-Resources-Headers/Dark-Resources/include"
IncludeDir["Glad"] = "Dark-Render/vendor/glad/include"
IncludeDir["VulkanHeaders"] = "Dark-Render/vendor/Vulkan-Headers/include"
IncludeDir["DirectXHeaders"] = "Dark-Render/vendor/DirectX-Headers/include"
IncludeDir["Glfw"] = "Render-Example/vendor/glfw/include"
IncludeDir["ImGui"] = "Render-Example/vendor/imgui"

group "Dependencies"
	include "Render-Example/vendor/glfw"
	include "Render-Example/vendor/imgui"
	include "Dark-Render/vendor/glad"
group ""

project "Dark-Render"
	location "Dark-Render"
	kind "SharedLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/include",
		"%{IncludeDir.DarkToolsHeaders}",
		"%{IncludeDir.DarkResourcesHeaders}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.VulkanHeaders}",
		"%{IncludeDir.DirectXHeaders}",
	}

	links
	{
		"Dark-Tools",
		"Dark-Resources",
		"Glad",
	}

	libdirs { "Test/downloads" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DK_PLATFORM_WINDOWS",
			"DK_DYNAMIC_LINK",
			"DRENDER_BUILD_DLL",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Render-Example/\"")
		}

	filter "configurations:Debug"
		defines "DK_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "DK_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "DK_Dist"
		runtime "Release"
		optimize "On"

project "Render-Example"
	location "Render-Example"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"Dark-Render/include",
		"%{IncludeDir.Glfw}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Glad}",
	}

	links
	{
		"Dark-Render",
		"Glfw",
		"ImGui",
		"Glad",
		"opengl32.lib",
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"DK_PLATFORM_WINDOWS",
			"DK_DYNAMIC_LINK",
			"DK_ENABLE_ASSERTS",
		}

	filter "configurations:Debug"
		defines "DK_DEBUG"
		runtime "Debug"
		kind "ConsoleApp"
		symbols "On"

	filter "configurations:Release"
		defines "DK_RELEASE"
		runtime "Release"
		kind "WindowedApp"
		optimize "On"

	filter "configurations:Dist"
		defines "DK_Dist"
		runtime "Release"
		kind "WindowedApp"
		optimize "On"
-- premake5.lua
workspace "GraphicEngine"
	location "../"
	configurations { "Debug", "Release" }

project "LLR"
	kind "SharedLib"
	language "C++"
	location "../prj/"
	targetdir "../lib/%{cfg.buildcfg}"
	
	files { "../src/LLR/*.h", "../src/LLR/*.cpp" }
	includedirs {"../external/include/", "$(VULKAN_SDK)/include/" }
	libdirs { "../external/libs", "$(VULKAN_SDK)/Lib" }
	links { "glfw3", "glew32", "opengl32", "vulkan-1" }

	filter "configurations:Debug"
		targetname "%{prj.name}d"
		architecture "x86_64"
		defines { "DEBUG" }
		symbols "On"

   filter "configurations:Release"
		architecture "x86_64"
		defines { "NDEBUG" }
		optimize "On"
		
	configuration "windows"
		postbuildcommands { " copy /Y \"$(TargetDir)$(TargetFileName)\" \"$(SolutionDir)bin\\%{cfg.buildcfg}\\$(TargetFileName) \""}
		
project "GU"
	kind "SharedLib"
	language "C++"
	location "../prj/"
	targetdir "../lib/%{cfg.buildcfg}"
	files { "../src/GU/*.h", "../src/GU/*.cpp" }
	includedirs {"../external/include/"}
	
	filter "configurations:Debug"
		targetname "%{prj.name}d"
		architecture "x86_64"
		defines { "DEBUG" }
		symbols "On"

   filter "configurations:Release"
		architecture "x86_64"
		defines { "NDEBUG" }
		optimize "On"
		
	configuration "windows"
		postbuildcommands { " copy /Y \"$(TargetDir)$(TargetFileName)\" \"$(SolutionDir)bin/%{cfg.buildcfg}/$(TargetFileName) \""}
	
project "GAPI"
	kind "SharedLib"
	language "C++"
	location "../prj/"
	targetdir "../lib/%{cfg.buildcfg}"
	files { "../src/GAPI/*.h", "../src/GAPI/*.cpp" }
	includedirs {"../external/include/"}
	
	libdirs { "../external/libs", "../lib/%{cfg.buildcfg}" }
	
	filter "configurations:Debug"
		targetname "%{prj.name}d"
		dependson {"GU", "LLR"}
		architecture "x86_64"
		defines { "DEBUG" }
		symbols "On"
		links { "GTEngine.v14.lib", "GUd.lib" , "LLRd.lib" }

   filter "configurations:Release"
		architecture "x86_64"
		defines { "NDEBUG" }
		optimize "On"
		 
	configuration "windows"
		postbuildcommands { " copy /Y \"$(TargetDir)$(TargetFileName)\" \"$(SolutionDir)bin/%{cfg.buildcfg}/$(TargetFileName) \""}
	
project "Demo"
	kind "ConsoleApp"
	language "C++"
	location "../prj/"
	targetdir "../bin/%{cfg.buildcfg}"
	files { "../src/Demo/*.h", "../src/Demo/*.cpp" }
	includedirs {"../external/include/"}
	objdir ("../Build/Obj/%{prj.name}/%{cfg.longname}")
	
	libdirs { "../external/libs", "../lib/%{cfg.buildcfg}" }
	
	filter "configurations:Debug"
		targetname "%{prj.name}d"
		dependson {"GAPI"}
		architecture "x86_64"
		defines { "DEBUG" }
		symbols "On"
		links { "glfw3.lib", "glew32", "opengl32.lib", "GTEngine.v14.lib", "GAPId.lib" }

   filter "configurations:Release"
		architecture "x86_64"
		defines { "NDEBUG" }
		optimize "On"
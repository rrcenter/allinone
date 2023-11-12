-- premake5.lua
workspace "sdl2games"
   configurations { "Debug", "Release" }
   location "build"

project "AppTriangle"
   location "build/AppTriangle"
   kind "ConsoleApp"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"

   files { "src/AppTriangle.cpp", "src/glad.c" }

   includedirs { "libs/sdl2/win32/include/" }

   libdirs { "libs/sdl2/win32/lib/x86/" }
   links { "SDL2", "SDL2main" }

   filter { "system:windows" }
      -- copy a file from the objects directory to the target directory
      postbuildcommands {
         "{COPY} %{wks.location}/../libs/sdl2/win32/lib/x86/SDL2.dll %{cfg.targetdir}",
         "{COPY} %{wks.location}/../libs/angle/libEGL.dll %{cfg.targetdir}",
         "{COPY} %{wks.location}/../libs/angle/libGLESv2.dll %{cfg.targetdir}"
      }


   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
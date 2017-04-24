project(Basic_Graphics)

cmake_minimum_required (VERSION 2.8.2) 

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

if(NOT CMAKE_BUILD_TYPE)
  set( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
#MinSizeRel." FORCE)
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set (CMAKE_INSTALL_PREFIX "${${PROJECT_NAME}_SOURCE_DIR}/build/install" CACHE PATH "default install path" FORCE )
endif()


set (SHADERS_BASIC 	resources/basic.vert resources/basic.frag)
set (SHADERS_DEPTH_CUBEMAP 	resources/depth_cubemap.vert resources/depth_cubemap.frag resources/depth_cubemap.geom)
set (SHADERS_DEPTH 	resources/depth.vert resources/depth.frag)
set (SHADERS_DEFERRED resources/deferred_geom.vert resources/deferred_geom.frag
	resources/deferred_light.vert resources/deferred_light.frag)
set (SHADERS_UTILS 	resources/textureUtil.frag resources/textureUtil.vert
					resources/cubemapUtil.frag resources/cubemapUtil.vert )
set (SHADERS_OTHER
		resources/unlit.vert
		resources/unlit.frag
		resources/attributeless.vert
		resources/attributeless.frag)
set (SHADERS ${SHADERS_BASIC} ${SHADERS_DEPTH} ${SHADERS_DEPTH_CUBEMAP} ${SHADERS_UTILS} ${SHADERS_OTHER} ${SHADERS_DEFERRED})

file (GLOB IMAGES resources/*.jpg resources/*.png resources/*.tiff)
file (GLOB MODELS resources/*.obj)

set (ENGINE_SOURCE
		src/glad/src/glad.c
		src/GLSLProgram.cpp
		src/Mesh.cpp
		src/Texture.cpp
		src/Clock.cpp
		src/Framebuffer.cpp
		src/DepthTexture.cpp
		src/Spotlight.cpp
		src/Camera.cpp
		src/Light.cpp
		src/Importer.cpp
		src/DepthCubemap.cpp
		src/Texture2D.cpp
		src/Cubemap.cpp
		src/DeferredGeom.cpp
)
set (ENGINE_HEADERS
		src/Graphics.h
		src/GLSLProgram.h
		src/Mesh.h
		src/Texture.h
		src/Clock.h
		src/Framebuffer.h
		src/DepthTexture.h
		src/Spotlight.h
		src/Camera.h
		src/Light.h
		src/Importer.h
		src/DepthCubemap.h
		src/Texture2D.h
		src/Cubemap.h
		src/DeferredGeom.h
		src/Engine.h
)
set (KINECT_SOURCE
		src/main.cpp
		src/Paddle.cpp
		src/Ball.cpp
		src/Table.cpp
		src/PingPong.cpp
		src/Fire.cpp
)
set (KINECT_HEADERS
		src/Paddle.h
		src/Ball.h
		src/Table.h
		src/PingPong.h
		src/Fire.h
)
set (OTHER_SOURCE 
		src/Utils.cpp
		src/StandardShader.cpp
		src/ShaderRegistry.cpp
		)
set (OTHER_HEADERS
		src/Utils.h
		src/StandardShader.h
		src/ShaderRegistry.h
)
set (SOURCEFILES ${ENGINE_SOURCE} ${KINECT_SOURCE} ${OTHER_SOURCE})

set (HEADERFILES  ${ENGINE_HEADERS} ${KINECT_HEADERS} ${OTHER_HEADERS})

source_group("Source Files\\Utils" FILES ${OTHER_SOURCE})
source_group("Header Files\\Utils" FILES ${OTHER_HEADERS})

#source_group("Header Files" FILES ${HEADERFILES})
source_group("Shaders" FILES ${SHADERS_OTHER})
source_group("Shaders\\basic" FILES ${SHADERS_BASIC})
source_group("Shaders\\depth" FILES ${SHADERS_DEPTH})
source_group("Shaders\\depth_cubemap" FILES ${SHADERS_DEPTH_CUBEMAP})
source_group("Shaders\\utils" FILES ${SHADERS_UTILS})
source_group("Shaders\\deferred" FILES ${SHADERS_DEFERRED})
source_group("Images" FILES ${IMAGES})
source_group("Models" FILES ${MODELS})

############################################################
#Start of library detection
#Any libraries outside of your source should have a corresponding
#find module and the 
#find_package method should be used here.
############################################################

#------------------------------------------
# Download dependencies
#------------------------------------------

set(glfw_checkout_Dir ${PROJECT_SOURCE_DIR}/deps/glfw)
make_directory(${glfw_checkout_Dir})
include(ExternalProject)

set(glfw_build_dir ${CMAKE_BINARY_DIR}/glfw)
set(glfw_stamp_dir ${glfw_build_dir}/stamp)
set(glfw_tmp_dir ${glfw_build_dir}/tmp)

set(glfw_PREFIX "${glfw_checkout_Dir}")
set(glfw_INSTALL_DIR "${glfw_build_dir}/install")

set(glfw_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${glfw_INSTALL_DIR} -DGLFW_BUILD_DOCS=OFF —DGLFW_BUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
set(glfw_DIR "${glfw_INSTALL_DIR}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(glfw_CMAKE_ARGS ${glfw_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD="c++11" -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY="libc++" -DGLFW_USE_RETINA=ON)
endif()

if (WIN32)
    set(glfw_CMAKE_ARGS ${glfw_CMAKE_ARGS} -DUSE_MSVC_RUNTIME_LIBRARY_DLL=OFF -DGLFW_USE_HYBRID_HPG=ON)
endif()
   
ExternalProject_add(glfw
		PREFIX ${glfw_PREFIX}
		GIT_REPOSITORY https://github.com/glfw/glfw.git
		INSTALL_DIR ${glfw_INSTALL_DIR}
		CMAKE_ARGS ${glfw_CMAKE_ARGS}
		UPDATE_COMMAND ""
		DOWNLOAD_DIR ${glfw_checkout_Dir}
		SOURCE_DIR ${glfw_checkout_Dir}
		TMP_DIR ${glfw_tmp_dir}
		STAMP_DIR ${glfw_stamp_dir}
		BINARY_DIR ${glfw_build_dir}
)
 
#set_property(TARGET glfw PROPERTY FOLDER "Dependencies")

set(GLFW_INCLUDE_DIR ${glfw_INSTALL_DIR}/include CACHE INTERNAL "Directory of GLFW header files")
if(MSVC)
	set(GLFW_LIBRARY ${glfw_INSTALL_DIR}/lib/glfw3.lib CACHE INTERNAL "GLFW lib file")
else()
	set(GLFW_LIBRARY ${glfw_INSTALL_DIR}/lib/libglfw3.a CACHE INTERNAL "GLFW lib file")
endif()	
include_directories(${GLFW_INCLUDE_DIR})


#Assimp
set(assimp_checkout_Dir ${PROJECT_SOURCE_DIR}/deps/assimp)
make_directory(${assimp_checkout_Dir})

set(assimp_build_dir ${CMAKE_BINARY_DIR}/assimp)
set(assimp_stamp_dir ${assimp_build_dir}/stamp)
set(assimp_tmp_dir ${assimp_build_dir}/tmp)

set(assimp_PREFIX "${assimp_checkout_Dir}")
set(assimp_INSTALL_DIR "${assimp_build_dir}/install")

set(assimp_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${assimp_INSTALL_DIR} -DBUILD_SHARED_LIBS=OFF  —DASSIMP_BUILD_TESTS=OFF —DASSIMP_BUILD_ASSIMP_TOOLS=OFF -DCMAKE_BUILD_TYPE=RELEASE -DASSIMP_INSTALL_PDB=OFF)
set(assimp_DIR "${assimp_INSTALL_DIR}")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set( assimp_CMAKE_ARGS ${assimp_CMAKE_ARGS} -DCMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD="c++11")
endif()

if (WIN32)
    set(glfw_CMAKE_ARGS ${assimp_CMAKE_ARGS} -DUSE_MSVC_RUNTIME_LIBRARY_DLL=ON)
endif()
   
ExternalProject_add(assimp
		PREFIX ${assimp_PREFIX}
		GIT_REPOSITORY https://github.com/assimp/assimp.git
		INSTALL_DIR ${assimp_INSTALL_DIR}
		CMAKE_ARGS ${assimp_CMAKE_ARGS}
		UPDATE_COMMAND ""
		DOWNLOAD_DIR ${assimp_checkout_Dir}
		SOURCE_DIR ${assimp_checkout_Dir}
		TMP_DIR ${assimp_tmp_dir}
		STAMP_DIR ${assimp_stamp_dir}
		BINARY_DIR ${assimp_build_dir}
)
 
#set_property(TARGET assimp PROPERTY FOLDER "Dependencies")

set(ASSIMP_INCLUDE_DIR ${assimp_INSTALL_DIR}/include CACHE INTERNAL "Directory of Assimp header files")
if(MSVC)
  # in order to prevent DLL hell, each of the DLLs have to be suffixed with the major version and msvc prefix
  if( MSVC70 OR MSVC71 )
    set(MSVC_PREFIX "vc70")
  elseif( MSVC80 )
    set(MSVC_PREFIX "vc80")
  elseif( MSVC90 )
    set(MSVC_PREFIX "vc90")
  elseif( MSVC10 )
    set(MSVC_PREFIX "vc100")
  elseif( MSVC11 )
    set(MSVC_PREFIX "vc110")
  elseif( MSVC12 )
    set(MSVC_PREFIX "vc120")
  elseif( MSVC14 )
    set(MSVC_PREFIX "vc140")
  else()
    set(MSVC_PREFIX "vc150")
  endif()
  set(ASSIMP_LIBRARY_SUFFIX "-${MSVC_PREFIX}-mt" CACHE STRING "the suffix for the assimp windows library" FORCE)
	set(ASSIMP_LIBRARY ${assimp_INSTALL_DIR}/lib/assimp${ASSIMP_LIBRARY_SUFFIX}.lib CACHE INTERNAL "Assimp lib file")
else()
	set(ASSIMP_LIBRARY ${assimp_INSTALL_DIR}/lib/libassimp.a CACHE INTERNAL "Assimp lib file")
endif()	
include_directories(${ASSIMP_INCLUDE_DIR})

find_package(ZLIB QUIET)
if (NOT ZLIB_FOUND)
	if (MSVC)
		message("Zlib not found. Setting it to zlib built with assimp")
		set(ZLIB_LIBRARY debug ${assimp_INSTALL_DIR}/lib/zlibstaticd.lib)
		set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
		SET(ZLIB_FOUND 1)
	endif()
endif()


#SOIL
set(soil_checkout_dir ${PROJECT_SOURCE_DIR}/deps/SOIL)
make_directory(${soil_checkout_dir})

set(soil_patch_dir ${PROJECT_SOURCE_DIR}/src/soil)

set(soil_build_dir ${CMAKE_BINARY_DIR}/soil)
set(soil_stamp_dir ${soil_build_dir}/stamp)
set(soil_tmp_dir ${soil_build_dir}/tmp)

set(soil_PREFIX "${soil_checkout_dir}")
set(soil_INSTALL_DIR "${soil_build_dir}/install")

set(soil_PATCH ${CMAKE_COMMAND} -E copy ${soil_patch_dir}/CMakeLists.txt <SOURCE_DIR>/CMakeLists.txt)
set(soil_CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${soil_INSTALL_DIR}")
   
ExternalProject_add(SOIL
	PREFIX ${soil_PREFIX}
	URL https://rawgit.com/hherman1/PersonalGraphics/master/SOIL.zip
	INSTALL_DIR ${soil_INSTALL_DIR}
	CMAKE_ARGS ${soil_CMAKE_ARGS}
	PATCH_COMMAND ${soil_PATCH}
	UPDATE_COMMAND ""
	DOWNLOAD_DIR ${soil_checkout_dir}
	SOURCE_DIR ${soil_checkout_dir}
	TMP_DIR ${soil_tmp_dir}
	STAMP_DIR ${soil_stamp_dir}
	BINARY_DIR ${soil_build_dir}
 )

set(SOIL_INCLUDE_DIR ${soil_INSTALL_DIR}/include CACHE INTERNAL "Directory of SOIL header files")

if(MSVC)
	set(SOIL_LIBRARY ${soil_INSTALL_DIR}/lib/SOIL.lib CACHE INTERNAL "SOIL lib file")
else()
	set(SOIL_LIBRARY ${soil_INSTALL_DIR}/lib/libSOIL.a CACHE INTERNAL "SOIL lib file")
endif()	
include_directories(${SOIL_INCLUDE_DIR})


# OpenGL
find_package(OpenGL REQUIRED)
set(OPENGL_LIBRARIES ${OPENGL_gl_LIBRARY})
set(OPENGL_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
include_directories(${OPENGL_INCLUDE_DIRS})

# GLEW
#set(CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/glew)
#set(CMAKE_LIBRARY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/glew/lib/Release/x64)

#find_package(GLEW REQUIRED)
#if (GLEW_FOUND)
#    include_directories(${GLEW_INCLUDE_DIRS})
#	add_definitions(${GLEW_DEFINITIONS})
#	#set(GLEW_DLL ${CMAKE_CURRENT_SOURCE_DIR}/glew/bin/Release/x64/glew32.dll)
#	set (GLEW_LIBRARIES ${CMAKE_LIBRARY_PATH}/glew32s.lib)
#	#set(GLEW_LIBRAR IES ${GLEW_LIBRARIES} ${GLEW_DLL})
#endif()
#message(${GLEW_LIBRARIES})
#message(${GLEW_INCLUDE_DIRS})

############################################################

set (CMAKE_VERBOSE_MAKEFILE TRUE)

############################################################
#OS Specific flags should go here
############################################################

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
 	 # Linux specific code
  	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC") 
	find_package(Threads)
	find_package(X11)
	set(LIBS_ALL ${LIBS_ALL} ${CMAKE_THREAD_LIBS_INIT} rt Xrandr Xxf86vm Xi m dl ${X11_LIBRARIES})
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # Apple-specific code
  set(CMAKE_CXX_FLAGS "-DOSX")
	set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
	#set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
	find_library(COCOA_LIB Cocoa)
	find_library(IOKIT_LIB IOKit)
	find_library(CORE_FOUNDATION_FRAMEWORK CoreFoundation)
    	find_library(CORE_VIDEO_FRAMEWORK CoreVideo)
	set(LIBS_ALL ${LIBS_ALL} ${COCOA_LIB} ${IOKIT_LIB} ${CORE_FOUNDATION_FRAMEWORK} ${CORE_VIDEO_FRAMEWORK})
	message(STATUS "${CORE_VIDEO_FRAMEWORK}")

endif( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

if (WIN32)
  # Windows-specific
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    # Tell MSVC to use main instead of WinMain for Windows subsystem executables
    set_target_properties(${WINDOWS_BINARIES} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif (WIN32)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${CMAKE_CURRENT_SOURCE_DIR}/src/glad/include
)

#Executable directory
set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
SET( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")

#Add executable target
#add_executable(Basic_Graphics ${SOURCEFILES} ${HEADERFILES})




# Organize folder structure for Xcode, Visual Studio, etc.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


#attempting to separate the pieces.
add_library(PersonalGraphics ${ENGINE_SOURCE} ${ENGINE_HEADERS} ${OTHER_HEADERS} ${OTHER_SOURCE})

add_executable(MacKinection ${KINECT_SOURCE} ${KINECT_HEADERS})
target_link_libraries(MacKinection PersonalGraphics ${GLFW_LIBRARY} ${ZLIB_LIBRARIES} ${ASSIMP_LIBRARY} ${SOIL_LIBRARY} ${OPENGL_LIBRARIES} ${LIBS_ALL})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MacKinection)
set_target_properties(MacKinection PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${OUTPUT_DIRECTORY})

add_dependencies(MacKinection glfw assimp SOIL Resources PersonalGraphics)

add_custom_target(Resources ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/resources ${OUTPUT_DIRECTORY}
					   SOURCES ${IMAGES} ${SHADERS} ${MODELS} README.md)

#add_custom_target(RESOURCES SOURCES ${IMAGES} ${SHADERS} ${MODELS})


#copy resource files to build folder so that the executable can find them in the working directory
#add_custom_command(TARGET Basic_Graphics POST_BUILD
 #                  COMMAND ${CMAKE_COMMAND} -E copy_directory
  #                     ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:Basic_Graphics>)


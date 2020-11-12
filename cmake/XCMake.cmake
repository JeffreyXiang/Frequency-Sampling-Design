# Print the content of a variable with title and prefix
function(Xi_printList)
	cmake_parse_arguments("ARG" "" "TITLE;PREFIX" "LIST" ${ARGN})
	if(NOT ${ARG_TITLE} STREQUAL "")
		message(STATUS ${ARG_TITLE})
	endif()
	foreach(str ${ARG_LIST})
		message(STATUS "${ARG_PREFIX}${str}")
	endforeach()
endfunction()

# Add all folders under path containing cmake program to the building system 
function(Xi_addAllSubDirGlob path)
	file(GLOB_RECURSE children LIST_DIRECTORIES true ${path}/*)
	set(dirs "")
	foreach(item ${children})
		if(IS_DIRECTORY ${item} AND EXISTS "${item}/CMakeLists.txt")
			list(APPEND dirs ${item})
		endif()
	endforeach()
	message(STATUS "")
	Xi_printList(TITLE "directories:" PREFIX "- " LIST ${dirs})
	foreach(dir ${dirs})
		add_subdirectory(${dir})
	endforeach()
endfunction()

# Relative path version
function(Xi_addAllSubDirRel path)
	Xi_addAllSubDirGlob(${CMAKE_CURRENT_SOURCE_DIR}/${path})
endfunction()

# Add all folders under 'root/src' containing cmake program to the building system
function(Xi_addAllSubDir)
	Xi_addAllSubDirGlob(${PROJECT_SOURCE_DIR}/src)
endfunction()

# Creat the source tree corresponding to the file system
function(Xi_groupSrcs)
	cmake_parse_arguments("ARG" "" "" "SOURCES" ${ARGN})
	foreach(file ${ARG_SOURCES})
		get_filename_component(filePath "${file}" PATH)
		file(RELATIVE_PATH filePathRel ${CMAKE_CURRENT_SOURCE_DIR} "${filePath}")
		if(MSVC)
			string(REPLACE "/" "\\" filePathRelMSVC "${filePathRel}")
		endif()
		source_group("${filePathRelMSVC}" FILES "${file}")
	endforeach()
endfunction()

# Find all the files under a target and create the source tree
function(Xi_groupAllSrcs)
	file(GLOB_RECURSE srcs ${CMAKE_CURRENT_SOURCE_DIR}/*)
	Xi_groupSrcs(SOURCES ${srcs})
endfunction()

# Get the name of the target from its path and folder name
function(Xi_getTargetNameGlob rst targetPath)
	if(NOT EXISTS "${targetPath}/CMakeLists.txt")
		message(FATAL_ERROR "Target ${targetPath} not exists.")
	endif()
	file(RELATIVE_PATH targetRelPath "${PROJECT_SOURCE_DIR}/src" ${targetPath})
	string(REPLACE "/" "_" targetName "${PROJECT_NAME}/${targetRelPath}")
	set(${rst} ${targetName} PARENT_SCOPE) 
endfunction()

# A relative path version
function(Xi_getTargetNameRel rst targetPath)
	file(RELATIVE_PATH targetRelPath "${PROJECT_SOURCE_DIR}/src" "${PROJECT_SOURCE_DIR}/src/${targetPath}")
	string(REPLACE "/" "_" targetName "${PROJECT_NAME}/${targetRelPath}")
	set(${rst} ${targetName} PARENT_SCOPE) 
endfunction()

# A specific version, call in current target folder
function(Xi_getCurTargetName rst)
	Xi_getTargetNameGlob(targetName ${CMAKE_CURRENT_SOURCE_DIR})
	set(${rst} ${targetName} PARENT_SCOPE) 
endfunction()

# Create a target
function(Xi_addTargetRaw)
	cmake_parse_arguments("ARG" "" "MODE" "SOURCES;LIBS" ${ARGN})

	# target info
	file(RELATIVE_PATH targetRelPath "${PROJECT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/..")
	set(folderPath "${PROJECT_NAME}/${targetRelPath}")
	Xi_getCurTargetName(targetName)
	
	# get all sources
	list(LENGTH ARG_SOURCES sourceNum)
	if(${sourceNum} EQUAL 0)
		file(GLOB_RECURSE ARG_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*)
		Xi_groupSrcs(SOURCES ${ARG_SOURCES})
		list(LENGTH ARG_SOURCES sourceNum)
		if(sourcesNum EQUAL 0)
			message(WARNING "Target [${targetName}] has no source")
			return()
		endif()
	endif()
	
	# print info
	message(STATUS "")
	message(STATUS "---------- New Target ----------")
	message(STATUS "- name: ${targetName}")
	message(STATUS "- folder : ${folderPath}")
	message(STATUS "- mode: ${ARG_MODE}")
	Xi_printList(LIST ${ARG_SOURCES}
		TITLE "- sources:"
		PREFIX "    ")
	list(LENGTH ARG_LIBS libNum)
	if(NOT ${libNum} EQUAL 0)
		Xi_printList(LIST ${ARG_LIBS}
			TITLE  "- libraries:"
			PREFIX "    ")
	endif()
	
	# add target
	if(${ARG_MODE} STREQUAL "EXE")
		add_executable(${targetName} ${ARG_SOURCES})
		if(MSVC)
			set_target_properties(${targetName} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
			set_target_properties(${targetName} PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
		endif()
	elseif(${ARG_MODE} STREQUAL "STATIC")
		add_library(${targetName} STATIC ${ARG_SOURCES})
	elseif(${ARG_MODE} STREQUAL "SHARED")
		add_library(${targetName} SHARED ${ARG_SOURCES})
	else()
		message(FATAL_ERROR "mode [${ARG_MODE}] is not supported")
		return()
	endif()
	
	# folder
	set_target_properties(${targetName} PROPERTIES FOLDER ${folderPath})
	
	target_link_libraries(${targetName} PUBLIC ${ARG_LIBS})
	target_include_directories(${targetName} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

	install(TARGETS ${targetName}
		RUNTIME DESTINATION "bin"
		ARCHIVE DESTINATION "lib"
		LIBRARY DESTINATION "lib")
endfunction()

# Specific version
function(Xi_addTarget)
	cmake_parse_arguments("ARG" "" "MODE" "LIBS" ${ARGN})
	Xi_addTargetRaw(MODE ${ARG_MODE} LIBS ${ARG_LIBS})
endfunction()

# Basic setting
macro(Xi_projectInit)
	# Add a character 'd' to the end of output filename under DEBUG mode
	set(CMAKE_DEBUG_POSTFIX d)

	# set C++ version
	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED True)

	if(MSVC)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /permissive-")
	endif()

	# set output folders (RUNTIME: exe, dll... | LIBRARY: lib | ARCHIVE: lib by dll)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib")

	# Open folder function of VS
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)

	# set include folders
	include_directories (
		${PROJECT_SOURCE_DIR}/include
		${PROJECT_SOURCE_DIR}/include/_deps
		# $ENV{XXX_INCLUDE_PATH}
		# ...
	)

	# set link folders
	link_directories (
		${PROJECT_SOURCE_DIR}/lib
		# $ENV{XXX_LIB_PATH}
		# ...
	)

	include (InstallRequiredSystemLibraries)
	set (CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
	set (CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
	include (CPack)
endmacro()

# call this if using Qt
macro(Xi_QtInit)
	set(CMAKE_INCLUDE_CURRENT_DIR ON)
	set(CMAKE_AUTOMOC ON)
	set(CMAKE_AUTOUIC ON)
	set(CMAKE_AUTORCC ON)
	find_package(Qt5Widgets REQUIRED)
endmacro()

function(Xi_findPackage packageName)
	message(STATUS "")
	message(STATUS "Finding package: ${packageName}")
	find_package(${packageName} REQUIRED)
	if(NOT ${packageName}_FOUND)
		message(FATAL_ERROR "${packageName} not found!")
	else()
		if(DEFINED ${packageName}_DIR)
			message(STATUS "${packageName}_DIR: ${${packageName}_DIR}")
			if(NOT DEFINED ${packageName}_INCLUDE_DIRS)
				get_filename_component(${packageName}_INCLUDE_DIRS "${${packageName}_DIR}/../../../include" ABSOLUTE)
				if(NOT EXISTS "${${packageName}_INCLUDE_DIRS}")
					unset(${packageName}_INCLUDE_DIRS)
				endif()
			endif()
			if(NOT DEFINED ${packageName}_LIBRARY_DIRS)
				get_filename_component(${packageName}_LIBRARY_DIRS "${${packageName}_DIR}/../../../lib" ABSOLUTE)
				if(NOT EXISTS "${${packageName}_LIBRARY_DIRS}")
					unset(${packageName}_LIBRARY_DIRS)
				endif()
			endif()
		endif()
		if(DEFINED ${packageName}_INCLUDE_DIRS)
			message(STATUS "${packageName}_INCLUDE_DIRS: ${${packageName}_INCLUDE_DIRS}")
			include_directories("${${packageName}_INCLUDE_DIRS}")
		endif()
		if(DEFINED ${packageName}_LIBRARY_DIRS)
			message(STATUS "${packageName}_LIBRARY_DIRS: ${${packageName}_LIBRARY_DIRS}")
			link_directories("${${packageName}_LIBRARY_DIRS}")
		endif()
		if(DEFINED ${packageName}_DEFINITIONS)
			message(STATUS "${packageName}_DEFINITIONS: ${${packageName}_DEFINITIONS}")
			add_definitions("${${packageName}_DEFINITIONS}")
		endif()
	endif()
endfunction()

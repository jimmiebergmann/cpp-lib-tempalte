# Function for grouping files. Used for filters in Visual Studio.
function(createSourceGroups files rootPath)
	foreach(_source IN ITEMS ${files} )
		get_filename_component(_source_path "${_source}" PATH)
		file(RELATIVE_PATH _source_path_rel "${rootPath}" "${_source_path}")
		#message("${_source_path} ${_source_path_rel} ${rootPath}")
		string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
		source_group("${_group_path}" FILES "${_source}")
	endforeach()
endfunction(createSourceGroups)


project (proj_template)
cmake_minimum_required(VERSION 2.8)

option(CODE_COVERAGE "Enable coverage reporting" OFF)

# Require C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads)


# Warnings as errors
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()


if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")


# Find files
file(GLOB_RECURSE test_files "${CMAKE_SOURCE_DIR}/test/*.cpp" "${CMAKE_SOURCE_DIR}/test/*.hpp")
file(GLOB_RECURSE lib_src "${CMAKE_SOURCE_DIR}/source/*.cpp")
file(GLOB_RECURSE lib_inc "${CMAKE_SOURCE_DIR}/include/*.hpp" "${CMAKE_SOURCE_DIR}/include/*.inl")
file(GLOB lib_files ${lib_src} ${lib_inc})


# Lib template
include_directories ("${CMAKE_SOURCE_DIR}/include") # Include header files
add_library(lib_template SHARED ${lib_files}) # Create shared library

#IF (WIN32)
#	target_link_libraries(lib_template "ws2_32.lib")
#ENDIF()

createSourceGroups("${lib_src}" "${CMAKE_SOURCE_DIR}/source")
createSourceGroups("${lib_inc}" "${CMAKE_SOURCE_DIR}/include")

target_compile_definitions(lib_template PRIVATE LIB_TEMPLATE_EXPORTS)
install(TARGETS lib_template DESTINATION "${CMAKE_SOURCE_DIR}lib") # Install library
set_target_properties( lib_template
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang") ## Code coverage
  set_target_properties(lib_template PROPERTIES COMPILE_FLAGS "--coverage")
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

install(FILES ${lib_inc} DESTINATION "${CMAKE_SOURCE_DIR}/include") # Install library headers



# Lib static
include_directories ("${CMAKE_SOURCE_DIR}/include")
add_library(lib_template_static STATIC ${lib_files}) # Create shared library

createSourceGroups("${lib_src}" "${CMAKE_SOURCE_DIR}/source")
createSourceGroups("${lib_inc}" "${CMAKE_SOURCE_DIR}/include")

target_compile_definitions(lib_template_static PRIVATE LIB_TEMPLATE_STATIC)
install(TARGETS lib_template_static DESTINATION "${CMAKE_SOURCE_DIR}lib") # Install library
set_target_properties( lib_template_static
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/lib"
)
install(FILES ${lib_inc} DESTINATION "${CMAKE_SOURCE_DIR}/include") # Install library headers


# Test
SET(gtest_force_shared_crt on CACHE BOOL
  "Use shared (DLL) run-time lib even when Google Test is built as static lib.")
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/googletest/googletest
                 ${CMAKE_SOURCE_DIR}/vendor/googletest/googletest
                 EXCLUDE_FROM_ALL) # Add googletest directly to our build.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

include_directories ("${CMAKE_SOURCE_DIR}/include")
link_directories("${CMAKE_SOURCE_DIR}/lib")
add_executable(lib_test ${test_files})
set_target_properties( lib_test
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
)
if(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang") ## Code coverage
  set_target_properties(lib_test PROPERTIES COMPILE_FLAGS "--coverage")
endif(CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

target_link_libraries(lib_test gtest_main lib_template)

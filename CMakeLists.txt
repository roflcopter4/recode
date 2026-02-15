cmake_minimum_required (VERSION 3.20.0)

project (recode LANGUAGES CXX VERSION 0.0.1)

if (MSVC)
    message ("Use the provided solution instead.")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set (CMAKE_C_STANDARD 17)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED TRUE)
set (CMAKE_C_EXTENSIONS ON)
set (CMAKE_CXX_EXTENSIONS ON)

#find_library (UNISTRING_LIBRARY NAMES libunistring;unistring REQUIRED HINTS "${VCPKG_INSTALLED_DIR}/")
find_package(Iconv REQUIRED)
find_package(Unistring REQUIRED)

enable_testing()
add_subdirectory(recode)

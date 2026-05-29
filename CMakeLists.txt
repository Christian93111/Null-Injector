cmake_minimum_required(VERSION 3.20)
project(NullInjector VERSION 0.1.0)

# Ensure MSVC statically links the runtime library (/MT or /MTd) to avoid missing DLLs
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(CMAKE_CXX_STANDARD 17)

set(wxBUILD_SHARED OFF CACHE BOOL "OFF for static library!" FORCE) # force wxWidgets to use static library
set(wxUSE_LIBTIFF OFF CACHE BOOL "Disable TIFF support to fix build errors" FORCE)
set(wxUSE_LIBJPEG builtin CACHE BOOL "Enable JPEG support for icon" FORCE)
mark_as_advanced(wxBUILD_SHARED)

add_executable(NullInjector WIN32)
add_subdirectory(src)

set_target_properties(NullInjector PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")

add_subdirectory(wxWidgets-3.3.2)

target_link_libraries(NullInjector wx::net wx::core wx::base)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
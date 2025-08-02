cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

set(DOCKING_IMGUI_LIB_NAME "imgui")
set(DOCKING_IMGUI_LIB_INCLUDE_DIR "imgui/")

file(GLOB SOURCE_FILES "imgui/*.cpp"  "imgui/*.h" "imgui/*.hpp")
list(APPEND SOURCE_FILES "imgui/backends/imgui_impl_dx12.cpp" "imgui/backends/imgui_impl_dx12.h")
list(APPEND SOURCE_FILES "imgui/backends/imgui_impl_win32.cpp" "imgui/backends/imgui_impl_win32.h")

message("\n[${DOCKING_IMGUI_LIB_NAME}] Begin building Visual Studio filters: ")
foreach(SOURCE_FILE IN ITEMS ${SOURCE_FILES})
    get_filename_component(SOURCE_FILE_PATH "${SOURCE_FILE}" PATH)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP_PATH "${SOURCE_FILE_PATH}")
    string(REPLACE "/" "\\" GROUP_PATH "${GROUP_PATH}")
    message(${SOURCE_FILE})
    source_group("${GROUP_PATH}" FILES "${SOURCE_FILE}")
endforeach()
message("[${DOCKING_IMGUI_LIB_NAME}] End building Visual Studio filters.\n")

add_library(${DOCKING_IMGUI_LIB_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${DOCKING_IMGUI_LIB_NAME} PUBLIC ${DOCKING_IMGUI_LIB_INCLUDE_DIR} ${DOCKING_IMGUI_LIB_INCLUDE_DIR})
target_link_libraries(${DOCKING_IMGUI_NAME} PRIVATE ${DOCKING_IMGUI_LIB_NAME} "d3d12.lib" "dxgi.lib" "dxguid.lib" "d3dcompiler.lib")
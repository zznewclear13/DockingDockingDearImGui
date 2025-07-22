cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

file(GLOB_RECURSE SOURCE_FILES *.cpp* *.h* *.hpp*)

message("\n[${DOCKING_IMGUI_NAME}] Begin building Visual Studio filters: ")
foreach(SOURCE_FILE IN ITEMS ${SOURCE_FILES})
    get_filename_component(SOURCE_FILE_PATH "${SOURCE_FILE}" PATH)
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}" "" GROUP_PATH "${SOURCE_FILE_PATH}")
    string(REPLACE "/" "\\" GROUP_PATH "${GROUP_PATH}")
    message(${SOURCE_FILE})
    source_group("${GROUP_PATH}" FILES "${SOURCE_FILE}")
endforeach()
message("[${DOCKING_IMGUI_NAME}] End building Visual Studio filters.\n")

add_executable(${DOCKING_IMGUI_NAME} ${SOURCE_FILES})
target_include_directories(${DOCKING_IMGUI_NAME} PRIVATE ${DOCKING_IMGUI_INCLUDE_DIR})
target_link_directories(${DOCKING_IMGUI_NAME} PRIVATE ${DOCKING_IMGUI_LIBRARY_DIR})

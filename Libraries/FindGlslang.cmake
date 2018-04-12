set(GLSLANG_INCLUDES "${PROJECT_SOURCE_DIR}/Libraries/glslang/glslang/Include" "${PROJECT_SOURCE_DIR}/Libraries/glslang/SPIRV")
set(SKIP_GLSLANG_INSTALL ON CACHE INTERNAL "Skip installation")
set(BUILD_TESTING OFF CACHE INTERNAL "Enables test builds")
set(ENABLE_GLSLANG_BINARIES OFF CACHE INTERNAL "Builds glslangValidator and spirv-remap")
set(ENABLE_AMD_EXTENSIONS OFF CACHE INTERNAL "Enables support of AMD-specific extensions")
set(ENABLE_NV_EXTENSIONS ON CACHE INTERNAL "Enables support of Nvidia-specific extensions")
set(ENABLE_HLSL OFF CACHE INTERNAL "Enables HLSL input support")
set(ENABLE_OPT OFF CACHE INTERNAL "Enables spirv-opt capability if present")
add_subdirectory(${PROJECT_SOURCE_DIR}/Libraries/glslang)
set(GLSLANG_LIBRARY "glslang" "SPIRV")

set(LIBRARIES_INCLUDES ${LIBRARIES_INCLUDES} ${GLSLANG_INCLUDES})
set(LIBRARIES_LINKS ${LIBRARIES_LINKS} ${GLSLANG_LIBRARY})

if(NOT GLSLANG_LIBRARY)
    message(FATAL_ERROR "Glslang library not found!")
endif()

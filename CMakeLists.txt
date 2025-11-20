cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME task-queue-project)

# Include toolchain file
include("cmake/gcc-arm-none-eabi.cmake")

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# --- Add Third-Party Libraries Here ---

# Add FreeRTOS Kernel
add_library(freertos_kernel STATIC
    # Add FreeRTOS source files
    middleware/freeRTOS/FreeRTOS-Kernel/Source/event_groups.c
    middleware/freeRTOS/FreeRTOS-Kernel/Source/list.c
    middleware/freeRTOS/FreeRTOS-Kernel/Source/queue.c
    middleware/freeRTOS/FreeRTOS-Kernel/Source/stream_buffer.c
    middleware/freeRTOS/FreeRTOS-Kernel/Source/tasks.c
    middleware/freeRTOS/FreeRTOS-Kernel/Source/timers.c

    # Add the portable layer for GCC ARM Cortex-M4F
    middleware/freeRTOS/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/port.c

    # Add the memory management scheme (heap_4 is a common choice)
    middleware/freeRTOS/FreeRTOS-Kernel/portable/MemMang/heap_4.c
)

target_include_directories(freertos_kernel PUBLIC
    middleware/freeRTOS/FreeRTOS-Kernel/include
    middleware/freeRTOS/FreeRTOS-Kernel/portable/GCC/ARM_CM4F
    Core/Inc # For FreeRTOSConfig.h
)



# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    Core/Src/main.c
    Core/Src/task_handler.c
    Core/Src/led_effects.c
    Core/Src/rtc.c
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    freertos_kernel
    # Add user defined libraries
)

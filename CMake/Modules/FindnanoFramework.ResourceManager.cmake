#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#


# set include directories
list(APPEND nanoFramework.ResourceManager_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src/HAL/Include")
list(APPEND nanoFramework.ResourceManager_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src/nanoFramework.ResourceManager")

if(API_nanoFramework.Graphics)
list(APPEND nanoFramework.ResourceManager_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Core")
list(APPEND nanoFramework.ResourceManager_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src/nanoFramework.Graphics/Graphics/Displays")
endif()


# source files
set(nanoFramework.ResourceManager_SRCS

    # source files of the assembly
    nf_system_resourcemanager_nanoFramework_Runtime_Native_ResourceUtility.cpp
    nf_system_resourcemanager_System_Resources_ResourceManager.cpp
    nf_system_resourcemanager.cpp
)

foreach(SRC_FILE ${nanoFramework.ResourceManager_SRCS})

    set(nanoFramework.ResourceManager_SRC_FILE SRC_FILE-NOTFOUND)

    find_file(nanoFramework.ResourceManager_SRC_FILE ${SRC_FILE}
        PATHS

            # path for source files of this module
            ${CMAKE_SOURCE_DIR}/src/nanoFramework.ResourceManager

        CMAKE_FIND_ROOT_PATH_BOTH
    )

    if (BUILD_VERBOSE)
        message("${SRC_FILE} >> ${nanoFramework.ResourceManager_SRC_FILE}")
    endif()

    list(APPEND nanoFramework.ResourceManager_SOURCES ${nanoFramework.ResourceManager_SRC_FILE})
    
endforeach()


include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(nanoFramework.ResourceManager DEFAULT_MSG nanoFramework.ResourceManager_INCLUDE_DIRS nanoFramework.ResourceManager_SOURCES)

#
# Copyright (c) .NET Foundation and Contributors
# See LICENSE file in the project root for full license information.
#

include(FetchContent)
FetchContent_GetProperties(fatfs)

# List of the required FatFs include files.
list(APPEND FATFS_INCLUDE_DIRS ${fatfs_SOURCE_DIR}/source)
list(APPEND FATFS_INCLUDE_DIRS ${TARGET_BASE_LOCATION})

set(FATFS_SRCS

    # fatfs
    ff.c
    ffunicode.c
)

foreach(SRC_FILE ${FATFS_SRCS})

    set(FATFS_SRC_FILE SRC_FILE -NOTFOUND)

    find_file(FATFS_SRC_FILE ${SRC_FILE}
        PATHS 
            ${fatfs_SOURCE_DIR}/source

        CMAKE_FIND_ROOT_PATH_BOTH
    )

    if (BUILD_VERBOSE)
        message("${SRC_FILE} >> ${FATFS_SRC_FILE}")
    endif()


    list(APPEND FATFS_SOURCES ${FATFS_SRC_FILE})

endforeach()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(FATFS DEFAULT_MSG FATFS_INCLUDE_DIRS FATFS_SOURCES)

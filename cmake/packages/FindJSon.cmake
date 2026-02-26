# Package Info.
set(JSON_NAME "JSon")

if(USE_BOOST)
    set(JSON_DESCRIPTION "A C++11 or library for parsing and serializing JSON to and from a DOM container in memory based on Boost.")
else()
    set(JSON_DESCRIPTION "A C++ library for interacting with JSON.")
endif()

# Package option.
option(USE_JSON ${JSON_DESCRIPTION} TRUE)
option(JSON_FETCH_ON_MISSING "Fetch jsoncpp when no local jsoncpp package is available." OFF)
if(USE_JSON)
    add_definitions(-DUSE_JSON)
endif()

set(JSon_FOUND FALSE)

# Prefer a system JSON package first so offline/air-gapped builds still work.
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(JSONCPP QUIET jsoncpp)
    if(NOT JSONCPP_FOUND)
        pkg_check_modules(JSONCPP QUIET jsoncpp_lib)
    endif()
    if(NOT JSONCPP_FOUND)
        pkg_check_modules(JSONCPP QUIET json)
    endif()
endif()

if(JSONCPP_FOUND)
    message(STATUS "Using jsoncpp from pkg-config (${JSONCPP_VERSION})")
    list(APPEND LIB_MODULES ${JSONCPP_LIBRARIES})
    list(APPEND LIB_TARGET_INCLUDE_DIRECTORIES ${JSONCPP_INCLUDE_DIRS})
    list(APPEND LIB_TARGET_LINK_DIRECTORIES ${JSONCPP_LIBRARY_DIRS})
    set(JSon_FOUND TRUE)
    set(JSON_FOUND TRUE)
    return()
endif()

if(NOT USE_JSON)
    return()
endif()

# Fallback: fetch jsoncpp when not available locally.
if(NOT JSON_FETCH_ON_MISSING)
    message(FATAL_ERROR "jsoncpp was not found locally. Install jsoncpp or set -DJSON_FETCH_ON_MISSING=ON to fetch it.")
endif()

# Fallback: fetch jsoncpp when allowed and not available locally.
set(JSON_URL "https://github.com/open-source-parsers/jsoncpp.git")
if(FORCE_UPGRADED_LIBS)
    set(JSON_TAG "master")
else()
    set(JSON_TAG "1.9.5")
endif()
set(JSON_LIB_LIST "jsoncpp_lib" CACHE STRING "List of modules (separated by a semicolon)")

set(FETCHCONTENT_QUIET OFF)
get_filename_component(json_base "${CMAKE_CURRENT_SOURCE_DIR}/${THIRD_PARTY}/${PLATFORM_FOLDER_NAME}/${JSON_NAME}"
    REALPATH BASE_DIR "${CMAKE_BINARY_DIR}")
set(FETCHCONTENT_BASE_DIR ${json_base})
FetchContent_Declare(
    jsoncpp
    GIT_REPOSITORY ${JSON_URL}
    GIT_TAG ${JSON_TAG}
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(jsoncpp)

list(APPEND LIB_TARGET_INCLUDE_DIRECTORIES ${jsoncpp_SOURCE_DIR}/include)
foreach(module IN LISTS JSON_LIB_LIST)
    list(APPEND LIB_MODULES ${module})
endforeach()

set(JSon_FOUND TRUE)
set(JSON_FOUND TRUE)

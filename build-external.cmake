set(root "${CMAKE_CURRENT_LIST_DIR}")
set(self "${CMAKE_CURRENT_LIST_FILE}")
set(build_id)

if (NOT DEFINED EXTERNAL)
    message(FATAL_ERROR "You must define -DEXTERNAL:STRING to specify which external library to build.")
endif ()
if (NOT DEFINED COMPILER)
    message(FATAL_ERROR "You must define -DCOMPILER:PATH to specify which external library to build.")
endif ()
if (NOT DEFINED OPTIONS)
    set(OPTIONS)
endif ()
set(external "${EXTERNAL}")
set(compiler "${COMPILER}")
set(options_flags ${OPTIONS})

set(build_root "build")
set(build_cache_root "${build_root}/cache")
set(
    common_flags
    -r
    -Ur
    -std=c++20
    -ggdb
    -O3
    -fno-pie
)
set(
    diagnostic_flags
    -fdiagnostics-color=always
)
set(
    feature_flags
    -fvisibility=hidden
    -fno-rtti
    -fno-exceptions
    -fno-strict-aliasing
    -nostdlib++
    -nostdinc++
)
file(MAKE_DIRECTORY "${root}/${build_root}")
file(MAKE_DIRECTORY "${root}/${build_cache_root}")
message("Build external ${external}")
string(MD5 external_hash "${external}")
string(MD5 compiler_hash "${compiler}")
string(MD5 options_hash "${options_flags}")
list(APPEND build_id "${external_hash}")
list(APPEND build_id "${compiler_hash}")
list(APPEND build_id "${options_hash}")

message("Discovering for ${external}...")
file(GLOB_RECURSE
    dependencies
    LIST_DIRECTORIES false
    RELATIVE "${root}"
    "${self}"
    "${root}/external/${external}/external.txt"
    "${root}/external/${external}/*.h"
    "${root}/external/${external}/*.hpp"
)
file(GLOB
    sources
    LIST_DIRECTORIES false
    RELATIVE "${root}"
    "${root}/external/${external}/*.cpp"
)
foreach (build_source ${dependencies} ${sources})
    message("- ${build_source}")

    string(MD5 path_hash "${build_source}")
    list(APPEND build_id "${path_hash}")

    file(MD5 "${root}/${build_source}" content_hash)
    string(APPEND build_id "${content_hash}")
endforeach ()
string(MD5 build_id "${build_id}")
string(SUBSTRING "${build_id}" 0 7 build_id)
message("OK build ${build_id}.")

message("Building ${external}...")
set(library "${build_cache_root}/external-${external}.${build_id}.o")
if (NOT EXISTS "${library}")
    execute_process(
        COMMAND "${compiler}" ${common_flags} ${diagnostic_flags} ${feature_flags} ${options_flags} -o "${library}" ${sources}
        WORKING_DIRECTORY "${root}"
        COMMAND_ECHO STDOUT
        COMMAND_ERROR_IS_FATAL ANY
    )
endif ()
message("\nOK external ${library}.\n")


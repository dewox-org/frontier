set(root "${CMAKE_CURRENT_LIST_DIR}")
set(self "${CMAKE_CURRENT_LIST_FILE}")

message("Build Setup:")
if (NOT DEFINED COMPILER)
    set(COMPILER "g++")
endif ()
find_program(compiler NAMES "${COMPILER}")
message("- Compiler: ${compiler} (${COMPILER})")

message("Discovering...")
file(GLOB
    self
    LIST_DIRECTORIES false
    RELATIVE "${root}"
    "${self}"
)
file(GLOB_RECURSE
    dependencies
    LIST_DIRECTORIES false
    RELATIVE "${root}"
    "${root}/*.hpp"
    "${root}/*.inl"
)
file(GLOB_RECURSE
    sources
    LIST_DIRECTORIES false
    RELATIVE "${root}"
    "${root}/*.cpp"
)
set(build_id)
foreach (build_source "${self}" ${dependencies} ${sources})
    message("- ${build_source}")

    string(MD5 path_hash "${build_source}")
    list(APPEND build_id "${path_hash}")

    file(MD5 "${root}/${build_source}" content_hash)
    string(APPEND build_id "${content_hash}")
endforeach ()
string(MD5 build_id "${build_id}")
string(SUBSTRING "${build_id}" 0 7 build_id)
message("OK build ${build_id}.")

message("Building...")
set(build_root "build")
set(build_object_root "${build_root}/object")
set(executable "${build_object_root}/${build_id}.exe")
if (NOT EXISTS "${executable}")
    file(MAKE_DIRECTORY "${root}/${build_root}")
    file(MAKE_DIRECTORY "${root}/${build_object_root}")
    execute_process(
        COMMAND "${compiler}" -std=c++20 -O3 -Wall -Wextra -Werror=return-type -Wno-unused-parameter -fvisibility=hidden -fdiagnostics-color=always -o "${executable}" ${sources}
        WORKING_DIRECTORY "${root}"
        COMMAND_ECHO STDOUT
        COMMAND_ERROR_IS_FATAL ANY
    )
    file(COPY_FILE "${root}/${executable}" "${root}/${build_root}/dewox" ONLY_IF_DIFFERENT)
endif ()


set(root "${CMAKE_CURRENT_LIST_DIR}")
set(self "${CMAKE_CURRENT_LIST_FILE}")

message("Build Setup:")
if (NOT DEFINED COMPILER)
    set(COMPILER "g++")
endif ()
set(build_root "build")
set(build_cache_root "${build_root}/cache")
set(
    common_flags
    -std=c++20
    -O3
    -static
    -fno-pie
)
set(
    diagnostic_flags
    -fdiagnostics-color=always
    -Wall
    -Wextra
    -Werror=return-type
    -Werror=unused-result
    -Wno-unused-parameter
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
find_program(compiler NAMES "${COMPILER}")
message("- Build: ${build_root}")
message("- Build Cache: ${build_cache_root}")
message("- Compiler: ${compiler} (${COMPILER})")

message("Discovering...")
file(GLOB_RECURSE
    tests
    LIST_DIRECTORIES false
    RELATIVE "${root}"
    "${root}/source/test-*.cpp"
)
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
    "${root}/source/*.hpp"
    "${root}/source/*.inl"
)
file(GLOB_RECURSE
    sources
    LIST_DIRECTORIES false
    RELATIVE "${root}"
    "${root}/source/*.cpp"
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

message("Generating sites...")
set(sites "${build_cache_root}/sites.${build_id}.cpp")
set(site_tests_declaration)
set(site_tests_reference)
string(APPEND
    site_tests_declaration
    "namespace dewox::inline test\n"
    "{\n"
)
foreach (test ${tests})
    if ("${test}" MATCHES "/(test-[a-z0-9-]+)[.]cpp$")
        set(test_name "${CMAKE_MATCH_1}")
        string(REPLACE "-" "_" test_id "${test_name}")
        message("- ${test}: ${test_id}")
        string(APPEND site_tests_declaration "    extern \"C\" auto dewox_site_${test_id}(Test* test) -> void;\n")
        string(APPEND site_tests_reference "        ::dewox::object::create(&Site::into, \"${test_name}\"_s, (void*) &::dewox::test::dewox_site_${test_id}),\n")
    else ()
        message("! ${test}: invalid test filename, ignored.")
    endif ()
endforeach ()
string(APPEND
    site_tests_declaration
    "}\n"
)
file(WRITE
    "${root}/${sites}"
    "#include \"${root}/source/site.hpp\"\n"
    "#include \"${root}/source/test.hpp\"\n"
    "#include \"${root}/source/object.hpp\"\n"
    "\n"
    "${site_tests_declaration}"
    "\n"
    "namespace dewox::inline site\n"
    "{\n"
    "    Site dewox_sites[]{\n"
    "${site_tests_reference}"
    "        ::dewox::object::create(&Site::into, {}, {}),\n"
    "    };\n"
    "}\n"
    "\n"
)

message("Building...")
set(executable "${build_cache_root}/dewox.${build_id}.exe")
if (NOT EXISTS "${executable}")
    execute_process(
        COMMAND "${compiler}" ${common_flags} ${diagnostic_flags} ${feature_flags} -o "${executable}" ${sources} ${sites}
        WORKING_DIRECTORY "${root}"
        COMMAND_ECHO STDOUT
        COMMAND_ERROR_IS_FATAL ANY
    )
endif ()
file(COPY_FILE "${root}/${executable}" "${root}/${build_root}/dewox" ONLY_IF_DIFFERENT)


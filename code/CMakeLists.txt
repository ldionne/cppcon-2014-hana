cmake_minimum_required(VERSION 3.0)
find_package(Boost)

#=============================================================================
# Setup compiler flags
#=============================================================================
include(CheckCXXCompilerFlag)
macro(append_flag lst testname flag)
    check_cxx_compiler_flag(${flag} ${testname})
    if (${testname})
        list(APPEND ${lst} ${flag})
    endif()
endmacro()

append_flag(CXX_FLAGS HAS_W_FLAG                         -W)
append_flag(CXX_FLAGS HAS_WALL_FLAG                      -Wall)
append_flag(CXX_FLAGS HAS_WEXTRA_FLAG                    -Wextra)
append_flag(CXX_FLAGS HAS_WNO_LONG_LONG_FLAG             -Wno-long-long)
append_flag(CXX_FLAGS HAS_WNO_UNUSED_LOCAL_TYPEDEFS_FLAG -Wno-unused-local-typedefs)
append_flag(CXX_FLAGS HAS_WNO_UNUSED_PARAMETER_FLAG      -Wno-unused-parameter)
append_flag(CXX_FLAGS HAS_WWRITE_STRINGS_FLAG            -Wwrite-strings)
append_flag(CXX_FLAGS HAS_STDCXX1Y_FLAG                  -std=c++1y)
append_flag(CXX_FLAGS HAS_PEDANTIC_FLAG                  -pedantic)

add_definitions(${CXX_FLAGS})
include_directories(hana/include)


#=============================================================================
# Setup targets
#=============================================================================
enable_testing()

add_executable(lambda_tuple lambda_tuple.cpp)
add_test(lambda_tuple lambda_tuple)

add_executable(concepts concepts.cpp)
add_test(concepts concepts)

add_executable(expression_templates expression_templates.cpp)
add_test(expression_templates expression_templates)

add_executable(integral integral.cpp)
add_test(integral integral)

add_executable(type_computations type_computations.cpp)
add_test(type_computations type_computations)

add_executable(record record.cpp)
add_test(record record)

if (${Boost_FOUND})
    add_executable(type_value_unification type_value_unification.cpp)
    add_test(type_value_unification type_value_unification)
else()
    message(STATUS "The Boost headers were not found; the `type_value_unification` target won't be available.")
endif()

# We make sure everything possible is inlined by using -O3
set_source_files_properties(loop_unrolling.cpp PROPERTIES COMPILE_FLAGS -O3)
add_executable(loop_unrolling loop_unrolling.cpp loop_unrolling_link.cpp)
add_test(loop_unrolling loop_unrolling)

add_subdirectory(benchmark)

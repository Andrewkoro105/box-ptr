cmake_minimum_required(VERSION 3.11)
project(box-ptr)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

option(${PROJECT_NAME}_ENABLE_INSTALL "Enable install rule" ON)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

add_executable(Box_Ptr_ex main.cpp)
target_link_libraries(Box_Ptr_ex PUBLIC ${PROJECT_NAME})

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.13.0
        FIND_PACKAGE_ARGS NAMES GTest
)

FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE test "tests/*.cpp")

add_executable(${PROJECT_NAME}_tests ${test})

target_link_libraries(${PROJECT_NAME}_tests PRIVATE gtest gtest_main ${PROJECT_NAME})

if (${PROJECT_NAME}_ENABLE_INSTALL)
        install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include DESTINATION ${CMAKE_INSTALL_PREFIX})

        install(TARGETS ${PROJECT_NAME}
                EXPORT ${PROJECT_NAME}
                LIBRARY DESTINATION lib
                PUBLIC_HEADER DESTINATION include/${PROJECT_NAME}
                )

        install(EXPORT ${PROJECT_NAME}
                FILE ${PROJECT_NAME}Config.cmake
                NAMESPACE ${PROJECT_NAME}::
                DESTINATION lib/cmake/${PROJECT_NAME}
                )
endif ()
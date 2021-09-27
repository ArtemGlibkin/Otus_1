cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project(ip_filter VERSION ${PROJECT_VESRION})

add_executable(ip_filter src/main.cpp)
add_library(ip_filter_lib STATIC src/IPV4Pool.cpp)
add_executable(ip_filter_test tests/IPV4PoolTest.cpp)

set_target_properties(ip_filter PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

target_include_directories(ip_filter
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ip_filter ip_filter_lib)

target_include_directories(ip_filter_lib
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(ip_filter_test PRIVATE ${GTEST_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ip_filter_test ${GTEST_LIBRARY} ${GTEST_MAIN_LIBRARY} Threads::Threads ip_filter_lib)

if(MSVC)
    target_compile_options(ip_filter_test PRIVATE /MT)
	target_compile_options(ip_filter_lib PRIVATE /MT)
	target_compile_options(ip_filter PRIVATE /MT)
endif()

install(TARGETS ip_filter RUNTIME DESTINATION bin)
set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT artem-glibkin@yandex.ru)

include(CPack)

enable_testing()
gtest_add_tests(TARGET ip_filter_test
				TEST_LIST Sorting Filter FilterAny Final 
)
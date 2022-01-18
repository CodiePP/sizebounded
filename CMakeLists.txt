cmake_minimum_required(VERSION 3.5)

project(BoundedSize)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_definitions( -DDEBUG )

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

# boost
#set(BOOST_ROOT "/opt/boost_1_61_0")
add_definitions( -DBOOST_ALL_DYN_LINK )
#set(Boost_ADDITIONAL_VERSIONS "1.49")
set(Boost_DEBUG OFF)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.55.0 REQUIRED COMPONENTS system chrono thread date_time unit_test_framework )
LINK_DIRECTORIES ( ${Boost_LIBRARY_DIRS} )
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})


file(GLOB SRCS *.cpp)

include_directories (${PROJECT_SOURCE_DIR}/..)

add_executable ( ${PROJECT_NAME}
    ${SRCS}
)

target_link_libraries ( ${PROJECT_NAME}
    ${Boost_LIBRARIES}
)


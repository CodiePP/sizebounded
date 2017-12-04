cmake_minimum_required(VERSION 2.8)

project(BoundedSize)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -Wall")

add_definitions( -DDEBUG )

# boost
#set(BOOST_ROOT "/opt/boost_1_61_0")
add_definitions( -DBOOST_ALL_DYN_LINK )
#set(Boost_ADDITIONAL_VERSIONS "1.49")
set(Boost_DEBUG OFF)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.61.0 REQUIRED COMPONENTS system chrono thread date_time unit_test_framework )
LINK_DIRECTORIES ( ${Boost_LIBRARY_DIRS} )
INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS})


file(GLOB SRCS *.cpp)

add_executable ( ${PROJECT_NAME}
    ${SRCS}
)

target_link_libraries ( ${PROJECT_NAME}
    ${Boost_LIBRARIES}
)


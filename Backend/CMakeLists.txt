cmake_minimum_required(VERSION 3.0.2)
project(main VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(libraries/fcgi2/include)

include_directories(libraries/json-develop/include)

link_directories(/etc/local/lib)

add_executable( main 
                main.cpp
                Back_end_http_server.cpp
                Back_end_http_server.h  )

target_link_libraries(main fcgi fcgi++) 

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})


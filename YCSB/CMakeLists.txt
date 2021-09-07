cmake_minimum_required(VERSION 3.8)
set(This ycsb)

set(Sources 
    ycsbc.cc
)

add_executable(${This} ${Sources})
set_target_properties(${This} PROPERTIES
    FOLDER Tests
)

add_subdirectory(core)
add_subdirectory(db)

target_link_libraries(${This} PUBLIC
    ycsbCore
    ycsbDB
    Kv
    gRPC::grpc++
    Protocol
)

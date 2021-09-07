cmake_minimum_required(VERSION 3.8)
set(This ycsbDB)

set(Headers
    include/lib/mem_alloc.h
    include/lib/stl_hashtable.h
    include/lib/string.h
    include/lib/string_hashtable.h
    include/db/basic_db.h
    include/db/db_factory.h
    include/db/eraft_db.h
)

set(Sources
    db_factory.cc
    eraft_db.cc
)

add_library(${This} STATIC ${Sources} ${Headers})
set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)
target_link_libraries(${This} PUBLIC
    ycsbCore
    Kv
    gRPC::grpc++
    Protocol
)

target_include_directories(${This} PUBLIC include)

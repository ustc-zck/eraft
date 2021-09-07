cmake_minimum_required(VERSION 3.8)
set(This ycsbCore)

set(Headers 
    include/core/client.h
    include/core/const_generator.h
    include/core/core_workload.h
    include/core/counter_generator.h
    include/core/db.h
    include/core/discrete_generator.h
    include/core/generator.h
    include/core/properties.h
    include/core/scrambled_zipfian_generator.h
    include/core/skewed_latest_generator.h
    include/core/timer.h
    include/core/uniform_generator.h
    include/core/utils.h
    include/core/zipfian_generator.h
)

set(Sources
    core_workload.cc
)

add_library(${This} STATIC ${Sources} ${Headers})
set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)

target_include_directories(${This} PUBLIC include)
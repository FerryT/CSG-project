cmake_minimum_required (VERSION 2.6)
project (StructuralSampler)

file(GLOB sample_SRC
    "StructuralSampler/src/*.cpp"
)

add_definitions("-std=gnu++0x")

include_directories(../metis/include)
include_directories(StructuralSampler/headers)

LINK_DIRECTORIES(../metis/lib)

#add_library(metis STATIC IMPORTED GLOBAL)
add_executable(Cluster ${sample_SRC})

TARGET_LINK_LIBRARIES(Cluster metis)

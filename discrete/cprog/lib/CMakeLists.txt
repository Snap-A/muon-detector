set(PDG_HDRS "${Discrete_SOURCE_DIR}/include/pdg/pdg.h")
set(SPHERE_HDRS "${Discrete_SOURCE_DIR}/include/sphere/sphere.h")
set(GEOMETRY_HDRS "${Discrete_SOURCE_DIR}/include/geometry/geometry.h")
set(VECTOR_HDRS "${Discrete_SOURCE_DIR}/include/vector/vector.h")

add_library(pdg pdg.c ${PDG_HDRS})
add_library(sphere sphere.c ${SPHERE_HDRS})
add_library(geometry geometry.c ${GEOMETRY_HDRS})
add_library(vector vector.c ${VECTOR_HDRS})

target_include_directories(pdg PUBLIC ../include)
target_include_directories(sphere PUBLIC ../include)
target_include_directories(geometry PUBLIC ../include)
target_include_directories(vector PUBLIC ../include)

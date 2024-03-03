set(GUN_HDRS "${MonteCarlo_SOURCE_DIR}/include/gun/gun.h"
  "${MonteCarlo_SOURCE_DIR}/include/gun/gun_range.h"
  "${MonteCarlo_SOURCE_DIR}/include/gun/gun_iso.h"
  "${MonteCarlo_SOURCE_DIR}/include/gun/gun_pdg.h"
  "${MonteCarlo_SOURCE_DIR}/include/gun/gun_decay.h")
set(PDF_HDRS "${MonteCarlo_SOURCE_DIR}/include/pdf/pdf.h")
set(PDG_HDRS "${MonteCarlo_SOURCE_DIR}/include/pdg/pdg.h")
set(SPHERE_HDRS "${MonteCarlo_SOURCE_DIR}/include/sphere/sphere.h")
set(GEOMETRY_HDRS "${MonteCarlo_SOURCE_DIR}/include/geometry/geometry.h")
set(VECTOR_HDRS "${MonteCarlo_SOURCE_DIR}/include/vector/vector.h")

add_library(gun gun.c gun_range.c gun_decay.c gun_iso.c gun_pdg.c ${GUN_HDRS})
add_library(pdf pdf.c ${PDF_HDRS})
add_library(pdg pdg.c ${PDG_HDRS})
add_library(sphere sphere.c ${SPHERE_HDRS})
add_library(geometry geometry.c ${GEOMETRY_HDRS})
add_library(vector vector.c ${VECTOR_HDRS})

target_include_directories(gun PUBLIC ../include)
target_include_directories(pdf PUBLIC ../include)
target_include_directories(pdg PUBLIC ../include)
target_include_directories(sphere PUBLIC ../include)
target_include_directories(geometry PUBLIC ../include)
target_include_directories(vector PUBLIC ../include)

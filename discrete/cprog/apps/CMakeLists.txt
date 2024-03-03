find_library(MATH_LIBRARY m)

add_executable(single single.c)

add_executable(tele tele.c)

target_link_libraries(single vector sphere pdg ${MATH_LIBRARY})
target_link_libraries(tele vector sphere pdg ${MATH_LIBRARY})

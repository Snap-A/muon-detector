include(CTest)

add_executable(test_vec test_vec.c)
add_executable(test_geo test_geo.c)

target_link_libraries(test_vec vector ${MATH_LIBRARY} ${CMOCKA_LIBRARIES})
target_link_libraries(test_geo geometry sphere vector pdg ${MATH_LIBRARY} ${CMOCKA_LIBRARIES})

add_test (NAME VectorTest COMMAND test_vec)
add_test (NAME GeometryTest COMMAND test_geo)

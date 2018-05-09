add_executable(ntobject use_ntobject.cpp)
add_executable(table use_table.cpp)

target_link_libraries(ntobject ${common_libs})
target_link_libraries(table ${common_libs})

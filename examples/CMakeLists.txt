add_executable(ntobject use_ntobject.cpp)
add_executable(table use_table.cpp)
add_executable(full full_use.cpp)
add_executable(test_transaction test_transactions.cpp)

target_link_libraries(ntobject ${common_libs})
target_link_libraries(table ${common_libs})
target_link_libraries(full ${common_libs})
target_link_libraries(test_transaction ${common_libs})

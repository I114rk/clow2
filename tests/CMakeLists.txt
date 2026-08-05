add_executable(clow_tests
    test_json.cpp
    test_configuration.cpp
)

target_include_directories(clow_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(clow_tests PRIVATE clow_core)

target_compile_features(clow_tests PRIVATE cxx_std_20)

enable_testing()
add_test(NAME clow_tests COMMAND clow_tests)

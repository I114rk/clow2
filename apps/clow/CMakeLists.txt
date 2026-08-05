add_executable(clow
    main.cpp
    command_parser.cpp
    command_processor.cpp
)

target_include_directories(clow PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(clow PRIVATE clow_core)

target_compile_features(clow PRIVATE cxx_std_20)

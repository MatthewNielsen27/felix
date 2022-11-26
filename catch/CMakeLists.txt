find_package(Catch2 3 REQUIRED)

add_executable(test_felix
    catch_String.cpp
)

target_link_libraries(test_felix
    PUBLIC
        felix
    PRIVATE
        Catch2::Catch2WithMain
)

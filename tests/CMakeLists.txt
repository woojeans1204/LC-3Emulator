
# tests/CMakeLists.txt
file(GLOB TEST_SRC_FILES *.cpp)

# Executable for tests
add_executable(RunTests ${TEST_SRC_FILES})

# Link with main project
target_link_libraries(RunTests LC3AssemblerSimulator)

# Define a custom target to run tests
add_custom_target(run_tests
    COMMAND RunTests
    DEPENDS RunTests
)
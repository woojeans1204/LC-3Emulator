# tests/CMakeLists.txt
# Define a test executable for SymbolTable
add_executable(TestSymbolTable TestSymbolTable.cpp)
target_link_libraries(TestSymbolTable PRIVATE LC3AssemblerSimulator)

# Define a test executable for Parser
add_executable(TestParser TestParser.cpp)
target_link_libraries(TestParser PRIVATE LC3AssemblerSimulator)
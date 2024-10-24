# tests/CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(LC3AssemblerTests)

# 테스트 소스 파일 모두 검색 (확장자는 cpp로 가정)
file(GLOB TEST_SOURCES "*.cpp")

# 각 테스트 파일에 대해 add_executable 수행
foreach(test_file ${TEST_SOURCES})
    # 파일 이름에서 확장자 제거하여 실행 파일 이름으로 사용
    get_filename_component(test_name ${test_file} NAME_WE)
    
    # 실행 파일 추가
    add_executable(${test_name} ${test_file})
    
    # 실행 파일에 LC3AssemblerSimulator 라이브러리 링크
    target_link_libraries(${test_name} PRIVATE LC3AssemblerSimulator)
    
    # 필요에 따라 추가적인 설정 (예: 컴파일 옵션) 가능
    # target_compile_options(${test_name} PRIVATE -Wall -Wextra -pedantic)
endforeach()

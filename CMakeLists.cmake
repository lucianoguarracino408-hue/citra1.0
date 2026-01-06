CMakeLists

cat > $BASE/cpp/CMakeLists.txt << 'EOF'
cmake_minimum_required(VERSION 3.14)
project(citra_robotics CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(robot_server
    src/main.cpp
    src/robot_core.cpp
    src/security.h
)

target_include_directories(robot_server PRIVATE src external)
target_link_libraries(robot_server Threads::Threads OpenSSL::SSL OpenSSL::Crypto)
EOF
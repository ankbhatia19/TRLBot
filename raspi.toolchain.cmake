set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)


set(CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++")
set(CMAKE_ASM_COMPILER "aarch64-linux-gnu-as")
find_program(CMAKE_AR NAMES "aarch64-linux-gnu-ar")
find_program(CMAKE_RANLIB NAMES "aarch64-linux-gnu-ranlib")

add_compile_options("-mcpu=cortex-a72")
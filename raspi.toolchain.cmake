set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)


set(CMAKE_C_COMPILER "aarch64-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "aarch64-linux-gnu-g++")
set(CMAKE_ASM_COMPILER "aarch64-linux-gnu-as")

# Below call is necessary to avoid non-RT problem.
SET(CMAKE_LIBRARY_ARCHITECTURE aarch64-linux-gnu)
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE aarch64)
SET(CPACK_RPM_PACKAGE_ARCHITECTURE aarch64)


find_program(CMAKE_AR NAMES "aarch64-linux-gnu-ar")
find_program(CMAKE_RANLIB NAMES "aarch64-linux-gnu-ranlib")

add_compile_options("-mcpu=cortex-a72")
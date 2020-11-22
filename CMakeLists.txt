#
# ftl library
#
# @author Natesh Narain <nnaraindev@gmail.com>
#

cmake_minimum_required(VERSION 3.0)

# TODO: Might need to use external projects for multiple toolchains / unit tests
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/toolchains/cmake-avr-toolchain/avr-gcc.toolchain.cmake")
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}")

project(ftl)

add_subdirectory(examples)

# TODO: Unit testing

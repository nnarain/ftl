
project(mcp9600)

find_package(libfirmware COMPONENTS avr_i2c)

add_definitions(-DF_CPU=16000000UL)
add_avr_executable(${PROJECT_NAME} "atmega2560"
    main.cpp
    ${LIBFIRMWARE_SOURCES}
)

target_link_libraries(${PROJECT_NAME}-atmega2560.elf libfirmware)

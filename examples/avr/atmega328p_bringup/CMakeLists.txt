
project(atmega328p_bringup)

find_package(ftl COMPONENTS avr_i2c)

add_definitions(-DF_CPU=16000000UL)
add_avr_executable(${PROJECT_NAME} "atmega328p"
    main.cpp
    ${FTL_SOURCES}
)

target_include_directories(${PROJECT_NAME}-atmega328p.elf PUBLIC
    ${FTL_INCLUDE_DIR}
)

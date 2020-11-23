project(mcp9808)

find_package(ftl COMPONENTS avr_i2c)

add_definitions(-DF_CPU=16000000UL)
add_avr_executable(${PROJECT_NAME} "atmega2560"
    main.cpp
    ${FTL_SOURCES}
)

target_include_directories(${PROJECT_NAME}-atmega2560.elf PUBLIC
    ${FTL_INCLUDE_DIR}
)

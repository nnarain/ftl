
project(ssd1306_fonts)

find_package(ftl COMPONENTS avr_i2c)

add_definitions(-DF_CPU=16000000UL)

set(target_name "${PROJECT_NAME}-${FTL_PLATFORM}")

add_avr_executable(${PROJECT_NAME}-${FTL_PLATFORM} ${FTL_PLATFORM}
    main.cpp
    ${FTL_SOURCES}
)

target_include_directories(${target_name}-${FTL_PLATFORM}.elf PUBLIC
    ${FTL_INCLUDE_DIR}
)

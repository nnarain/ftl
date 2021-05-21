set(FTL_PLATFORMS
    "atmega328p"
# FIXME: Should probably build out an explicit matrix of supported examples for each given platform.
#    "atmega2560"
#    "atmega32u4"
)

set(FTL_EXAMPLES
    "motor_shield"
    "serial_logger"
    "i2c_scanner"
    "mcp9600"
    "mcp9808"
    "ssd1306"
    "ssd1306_fonts"
    "pca9685"
    "pca9685_servo"
    "hcsr04"
)

# Generic
foreach(FTL_PLATFORM ${FTL_PLATFORMS})
    foreach(example ${FTL_EXAMPLES})
        add_subdirectory(${example} ${FTL_PLATFORM}-${example})
    endforeach(example ${FTL_EXAMPLES})
endforeach(FTL_PLATFORM ${FTL_PLATFORMS})

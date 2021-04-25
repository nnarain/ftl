set(FTL_PLATFORMS
    "atmega328p"
    "atmega2560"
    "atmega32u4"
)

set(FTL_EXAMPLES
    "serial_logger"
    "i2c_scanner"
    "mcp9600"
    "mcp9808"
    "ssd1306"
    "ssd1306_fonts"
    "pca9685"
)

# Generic
foreach(FTL_PLATFORM ${FTL_PLATFORMS})
    foreach(example ${FTL_EXAMPLES})
        add_subdirectory(${example} ${FTL_PLATFORM}-${example})
    endforeach(example ${FTL_EXAMPLES})
endforeach(FTL_PLATFORM ${FTL_PLATFORMS})

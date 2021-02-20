# Device bringup
add_subdirectory(atmega32u4_bringup)

# Temperature sensors
add_subdirectory(mcp9600)
add_subdirectory(mcp9808)
# Displays
add_subdirectory(ssd1306)
add_subdirectory(ssd1306_fonts)

# Generic
add_subdirectory(i2c_scanner)

#
# Configuration for libfirmware
#
# @author Natesh Narain <nnaraindev@gmail.com>
#

# This library is primarily header only.
# However, some hardware abstraction requires source files (such as implementing ISRs).
# Specify components to include the required support library. Sources are returned as `LIBFIRMWARE_SOURCES`

set(LIBFIRMWARE_SOURCES)

set(LIBFIRMWARE_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

add_library(libfirmware INTERFACE)
target_include_directories(libfirmware INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

foreach(component ${libfirmware_FIND_COMPONENTS})
    # Include the components config script
    include(${CMAKE_CURRENT_LIST_DIR}/${component}-config.cmake)
    # Get component sources
    string(TOUPPER ${component} component)
    list(APPEND LIBFIRMWARE_SOURCES ${${component}_SOURCES})
endforeach()

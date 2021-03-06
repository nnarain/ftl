#
# Configuration for ftl
#
# @author Natesh Narain <nnaraindev@gmail.com>
#

# This library is primarily header only.
# However, some hardware abstraction requires source files (such as implementing ISRs).
# Specify components to include the required support library. Sources are returned as `FTL_SOURCES`

set(FTL_SOURCES)

set(FTL_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include;${CMAKE_CURRENT_LIST_DIR}/include/ftl/lib")

foreach(component ${ftl_FIND_COMPONENTS})
    # Include the components config script
    include(${CMAKE_CURRENT_LIST_DIR}/${component}-config.cmake)
    # Get component sources
    string(TOUPPER ${component} component)
    list(APPEND FTL_SOURCES ${${component}_SOURCES})
endforeach()

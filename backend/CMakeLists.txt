
#**************************************************************************************************
# Include *****************************************************************************************

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

#**************************************************************************************************
# Set variable ************************************************************************************
SET(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)


#**************************************************************************************************
# Make configuration ******************************************************************************
add_executable(koth-snake-server ${SOURCES})
set_property(TARGET koth-snake-server PROPERTY CXX_STANDARD 17)
target_link_libraries(koth-snake-server -pthread -lstdc++fs ${USOCKETS_LIB} ZLIB::ZLIB fmt::fmt)

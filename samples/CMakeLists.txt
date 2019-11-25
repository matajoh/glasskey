if( WIN32 )
  set( SAMPLES_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG> )
else()
  set( SAMPLES_FOLDER ${CMAKE_CURRENT_BINARY_DIR} )
endif()

set( GLASSKEY_INCLUDE_FOLDERS 
    ${GLUT_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR} 
)
include_directories( ${GLASSKEY_INCLUDE_FOLDERS} )

add_executable( hello_world hello_world.cpp )
target_link_libraries( hello_world ${GLUT_LIBRARIES})
target_compile_definitions(hello_world PUBLIC FREEGLUT_STATIC)
install( TARGETS hello_world DESTINATION samples )

install( FILES hello_world.cpp DESTINATION samples )
install( FILES CMakeLists.txt.in DESTINATION samples RENAME CMakeLists.txt )

set( SOURCES
  color.cpp
  frame.cpp
)

set( HEADERS
  color.h
  frame.h
)

add_definitions( -DGLASSKEY_VERSION=${GLASSKEY_VERSION} )

set( GLASSKEY_INCLUDE_FOLDERS 
${GLUT_INCLUDE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories( ${GLASSKEY_INCLUDE_FOLDERS} )

add_library( glasskey STATIC ${SOURCES} ${HEADERS} )

set( GLASSKEY_LINK_LIBRARIES 
  ${GLUT_LIBRARIES}
)

target_link_libraries( glasskey ${GLASSKEY_LINK_LIBRARIES} )
target_compile_definitions(glasskey PUBLIC FREEGLUT_STATIC)

install(TARGETS glasskey 
  RUNTIME DESTINATION "bin"
  ARCHIVE DESTINATION "lib"
  LIBRARY DESTINATION "lib"
)
install(FILES ${HEADERS} DESTINATION "include" )

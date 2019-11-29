set( SOURCES
  color.cpp
  text_grid.cpp
  glasskey.cpp
  rect.cpp
)

set( HEADERS
  color.h
  glasskey.h
  rect.h
)

add_definitions( -DGLASSKEY_VERSION=${GLASSKEY_VERSION} )

set( GLASSKEY_INCLUDE_FOLDERS 
${GLUT_INCLUDE_DIR}
${CMAKE_CURRENT_SOURCE_DIR}
)

include_directories( ${GLASSKEY_INCLUDE_FOLDERS} )

add_library( glasskey STATIC ${SOURCES} ${HEADERS} )



set( GLUT_STATIC_LIBRARIES ${GLUT_LIBRARIES})
list( TRANSFORM GLUT_STATIC_LIBRARIES REPLACE "lib/freeglut" "lib/freeglut_static" )
message(${GLUT_STATIC_LIBRARIES})
set( GLASSKEY_LINK_LIBRARIES 
  ${GLUT_STATIC_LIBRARIES}
)

target_link_libraries( glasskey ${GLASSKEY_LINK_LIBRARIES} )
target_compile_definitions(glasskey PUBLIC FREEGLUT_STATIC)

install(TARGETS glasskey 
  RUNTIME DESTINATION "bin"
  ARCHIVE DESTINATION "lib"
  LIBRARY DESTINATION "lib"
)
install(FILES ${HEADERS} DESTINATION "include" )


if( BUILD_PYTHON )
  set( GKPY_INCLUDE_FOLDERS
    ${GLASSKEY_INCLUDE_FOLDERS}
  )

  include_directories( ${GKPY_INCLUDE_FOLDERS} )

  pybind11_add_module(_glasskey python.cpp  ${SOURCES} ${HEADERS})
  target_link_libraries( _glasskey PRIVATE ${GLASSKEY_LINK_LIBRARIES} )
  target_compile_definitions(_glasskey PUBLIC FREEGLUT_STATIC)
  endif()
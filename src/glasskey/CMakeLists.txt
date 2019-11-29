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

include_directories( ${GLASSKEY_INCLUDE_FOLDERS} )

add_library( glasskey STATIC ${SOURCES} ${HEADERS} )

target_include_directories(glasskey PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries( glasskey PUBLIC FreeGLUT::freeglut_static)

install(TARGETS glasskey 
  RUNTIME DESTINATION "bin"
  ARCHIVE DESTINATION "lib"
  LIBRARY DESTINATION "lib"
)
install(FILES ${HEADERS} DESTINATION "include" )


if( BUILD_PYTHON )
  pybind11_add_module(_pyglasskey python.cpp  ${SOURCES} ${HEADERS})
  target_link_libraries( _pyglasskey PUBLIC FreeGLUT::freeglut_static)
endif()

if( BUILD_JULIA )
  add_library( _jlglasskey SHARED julia.cpp ${SOURCES} ${HEADERS})
  target_link_libraries( _jlglasskey PUBLIC FreeGLUT::freeglut_static JlCxx::cxxwrap_julia JlCxx::cxxwrap_julia_stl)
endif()
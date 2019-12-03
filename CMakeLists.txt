cmake_minimum_required( VERSION 3.8 )

# -------------------- Version --------------------------------

file( STRINGS "VERSION" GLASSKEY_VERSION_FILE )

string( REPLACE "." ";" GLASSKEY_VERSION_LIST ${GLASSKEY_VERSION_FILE} )

list( GET GLASSKEY_VERSION_LIST 0 GLASSKEY_VERSION_MAJOR )

list( GET GLASSKEY_VERSION_LIST 1 GLASSKEY_VERSION_MINOR )

list( GET GLASSKEY_VERSION_LIST 2 GLASSKEY_VERSION_REVISION )

set( GLASSKEY_VERSION ${GLASSKEY_VERSION_MAJOR}.${GLASSKEY_VERSION_MINOR}.${GLASSKEY_VERSION_REVISION} )

# -------------------- Options --------------------------------

project( glasskey VERSION ${GLASSKEY_VERSION} LANGUAGES CXX )

set( BUILD_DOCUMENTATION_DESC "Specifies whether to build the documentation for the API and XML" )
set( GLASSKEY_BUILD_DOCUMENTATION ON  CACHE BOOL ${BUILD_DOCUMENTATION_DESC} )

set( BUILD_PYTHON_DESC "Specifies whether to build the python module")
set( GLASSKEY_BUILD_PYTHON ON CACHE BOOL ${BUILD_PYTHON_DESC} )

set( BUILD_SHARED_LIB_DESC "Specifies whether to build the shared library")
set( GLASSKEY_BUILD_SHARED_LIB ON CACHE BOOL ${BUILD_SHARED_LIB_DESC})

set( BUILD_STATIC_LIB_DESC "Specifies whether to build the static library")
set( GLASSKEY_BUILD_STATIC_LIB ON CACHE BOOL ${BUILD_STATIC_LIB_DESC})

set( WARNINGS_AS_ERRORS OFF )

set (CMAKE_CXX_STANDARD 17)

IF(MSVC)
    SET( CMAKE_DEBUG_POSTFIX "d" )
ELSE()
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
ENDIF(MSVC)

##############################################
# Declare dependencies
find_package( FreeGLUT REQUIRED )

if( GLASSKEY_BUILD_DOCUMENTATION )
  find_package( Doxygen REQUIRED )
endif()

if( GLASSKEY_BUILD_PYTHON )
  if(${CMAKE_VERSION} VERSION_LESS "3.12.0")
    find_package(PythonInterp)
    if(PYTHONINTERP_FOUND)
      add_test(NAME PythonTests COMMAND ${PYTHON_EXECUTABLE} -m pytest test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
  else()
    find_package(Python3 COMPONENTS Interpreter)
    if(Python3_Interpreter_FOUND)
      add_test(NAME PythonTests COMMAND Python3::Interpreter -m pytest test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()
  endif()
  find_package( pybind11 REQUIRED )
endif()

set( SOURCES
  src/glasskey/color.cpp
  src/glasskey/text_grid.cpp
  src/glasskey/glasskey.cpp
  src/glasskey/rect.cpp
)

##############################################
# Create target and set properties

if( GLASSKEY_BUILD_SHARED_LIB )
  add_library( glasskey SHARED ${SOURCES} )

  target_include_directories(glasskey
    PUBLIC
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

  target_link_libraries( glasskey PUBLIC FreeGLUT::freeglut)
endif()

if( GLASSKEY_BUILD_STATIC_LIB )
  add_library( glasskey_static STATIC ${SOURCES} )

  target_include_directories(glasskey_static
    PUBLIC
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

  target_link_libraries( glasskey_static PUBLIC FreeGLUT::freeglut_static)
endif()

if( GLASSKEY_BUILD_PYTHON )
  pybind11_add_module(_pyglasskey src/python_binding.cpp  ${SOURCES} ${HEADERS})
  target_include_directories(_pyglasskey
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/include
  )
  target_link_libraries( _pyglasskey
    PUBLIC
      FreeGLUT::freeglut_static
  )
endif()

##############################################
# Installation instructions

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/glasskey)

if( GLASSKEY_BUILD_SHARED_LIB )
  install(TARGETS glasskey
    EXPORT glasskey-targets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
endif()

IF( GLASSKEY_BUILD_STATIC_LIB )
    INSTALL(TARGETS glasskey_static
      EXPORT glasskey-targets
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}      
    )
ENDIF()

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

#Export the targets to a script
install(EXPORT glasskey-targets
  FILE
    glasskeyTargets.cmake
  NAMESPACE
    glasskey::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

#Create a ConfigVersion.cmake file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/glasskeyConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/glasskeyConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/glasskeyConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

#Install the config, configversion and custom find modules
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/glasskeyConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/glasskeyConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

##############################################
## Exporting from the build tree
export(EXPORT glasskey-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/glasskeyTargets.cmake NAMESPACE glasskey::)

#Register package in user's package registry
export(PACKAGE glasskey)


##############################################
## Add subdirectories

add_subdirectory(samples)


##############################################
## Packaging

set(CPACK_PACKAGE_VENDOR "Matthew Johnson")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "An OpenGL library for ASCII display")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set( CPACK_GENERATOR "ZIP" )
set( CPACK_SOURCE_GENERATOR "ZIP" )
include(CPack)
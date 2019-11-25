cmake_minimum_required( VERSION 3.8 )

project( glasskey )

# -------------------- Version --------------------------------

file( STRINGS "VERSION" GLASSKEY_VERSION_FILE )

string( REPLACE "." ";" GLASSKEY_VERSION_LIST ${GLASSKEY_VERSION_FILE} )

list( GET GLASSKEY_VERSION_LIST 0 GLASSKEY_VERSION_MAJOR )

list( GET GLASSKEY_VERSION_LIST 1 GLASSKEY_VERSION_MINOR )

list( GET GLASSKEY_VERSION_LIST 2 GLASSKEY_VERSION_REVISION )

set( GLASSKEY_VERSION ${GLASSKEY_VERSION_MAJOR}.${GLASSKEY_VERSION_MINOR}.${GLASSKEY_VERSION_REVISION} )

message("Configure GLASSKEY_VERSION at ${GLASSKEY_VERSION}")

# -------------------- Options --------------------------------

set( BUILD_DOCUMENTATION_DESC "Specifies whether to build the documentation for the API and XML" )

set( BUILD_DOCUMENTATION ON  CACHE BOOL ${BUILD_DOCUMENTATION_DESC} )

set( WARNINGS_AS_ERRORS OFF )

# ------------------- Detect the system -----------------------

if( APPLE )
  set( SYSTEM_NAME "Mac" )
elseif( UNIX )
  set( SYSTEM_NAME "Linux" )
  execute_process(COMMAND lsb_release -is OUTPUT_VARIABLE LSB_ID)
  execute_process(COMMAND lsb_release -rs OUTPUT_VARIABLE LSB_RELEASE)
  if( LSB_ID AND LSB_RELEASE )
    string(STRIP "${LSB_ID}" LSB_ID)
    string(STRIP "${LSB_RELEASE}" LSB_RELEASE)
    set( SYSTEM_NAME "${SYSTEM_NAME}-${LSB_ID}-${LSB_RELEASE}")
  elseif( EXISTS "/etc/debian_version")
    file( READ /etc/debian_version DEBIAN_VERSION )
    set( SYSTEM_NAME "${SYSTEM_NAME}-${DEBIAN_VERSION}")
  elseif( EXISTS "/etc/os-release")
    execute_process(COMMAND "sed" "-ne" "s/^ID=\"\\?\\([a-z]\\+\\)\"\\?$/\\1/p" "/etc/os-release" OUTPUT_VARIABLE OS_RELEASE_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
    execute_process(COMMAND "sed" "-ne" "s/^VERSION_ID=\"\\?\\([0-9\\.]\\+\\)\"\\?$/\\1/p" "/etc/os-release" OUTPUT_VARIABLE OS_RELEASE_VERSION_ID OUTPUT_STRIP_TRAILING_WHITESPACE)
    set( SYSTEM_NAME "${SYSTEM_NAME}-${OS_RELEASE_ID}-${OS_RELEASE_VERSION_ID}")
  elseif( EXISTS "/etc/redhat-release")
    set( SYSTEM_NAME "${SYSTEM_NAME}-Redhat")
  endif()
elseif( WIN32 )
  set( SYSTEM_NAME "Windows" )
  set( GLUT_ROOT_PATH "C:\\Program Files\\freeglut" CACHE PATH "Path to GLUT root directory")
  set( OPENGL_LIBRARY_DIR "C:\\Program Files\\freeglut\\lib" CACHE PATH "Path to OpenGL lib directory")
endif()
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
  set( SYSTEM_NAME "${SYSTEM_NAME}-64bit" )
else()
  set( SYSTEM_NAME "${SYSTEM_NAME}-32bit" )
endif()

# -------------------- Find packages --------------------------

set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake )

find_package( GLUT REQUIRED )

if( BUILD_DOCUMENTATION )
  find_package( Doxygen REQUIRED )
endif()


# -------------------- Build settings -----------------------------

# use C++11
set (CMAKE_CXX_STANDARD 11)
 
if( MSVC )
  if( WARNINGS_AS_ERRORS )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX" )
  endif()
  # set the Windows target version to WinXP or later
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0501" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS" )
else()
  set( GLASSKEY_CSHARP_DIR ${CMAKE_BINARY_DIR}/src/CSharpWrapper )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
  if( WARNINGS_AS_ERRORS )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )
  else()
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations" ) 
  endif()  
endif()

if( APPLE )
  add_definitions( -DAPPLE )
  set( CMAKE_MACOSX_RPATH ON )
endif()

# -------------------- Walk the subdirectories --------------------

add_subdirectory( src )
add_subdirectory( test )
add_subdirectory( samples )
if( BUILD_DOCUMENTATION )
  add_subdirectory( doc )
endif()

# -------------------- Package ------------------------------------

set( CPACK_SYSTEM_NAME ${SYSTEM_NAME} )
set( CPACK_PACKAGE_VERSION "${GLASSKEY_VERSION}" )
set( CPACK_GENERATOR "ZIP" )
set( CPACK_SOURCE_GENERATOR "ZIP" )
include( CPack )

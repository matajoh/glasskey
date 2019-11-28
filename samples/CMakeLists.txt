SET( SAMPLES
  hello_world
  bouncing_ball
  multi_window
)

if( WIN32 )
  set( SAMPLES_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG> )
else()
  set( SAMPLES_FOLDER ${CMAKE_CURRENT_BINARY_DIR} )
endif()

set( GLASSKEY_INCLUDE_FOLDERS 
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
    ${CMAKE_CURRENT_SOURCE_DIR} 
)
include_directories( ${GLASSKEY_INCLUDE_FOLDERS} )

foreach(sample ${SAMPLES})
  add_executable( ${sample} ${sample}.cpp )
  target_link_libraries( ${sample} glasskey)
  target_compile_definitions(${sample} PUBLIC FREEGLUT_STATIC)
  install( TARGETS ${sample} DESTINATION samples )
  
  install( FILES ${sample}.cpp DESTINATION samples )
endforeach(sample)

install( FILES CMakeLists.txt.in DESTINATION samples RENAME CMakeLists.txt )

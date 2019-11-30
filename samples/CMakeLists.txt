SET( SAMPLES
  hello_world
  bouncing_balls
  multi_window
)

foreach(sample ${SAMPLES})
  add_executable( ${sample} cpp/${sample}.cpp )
  target_include_directories( ${sample}
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/../../src
      ${CMAKE_CURRENT_SOURCE_DIR}/../../include
  )
  target_link_libraries( ${sample} glasskey_static)
  install( TARGETS ${sample} DESTINATION samples/bin )
  
  install( FILES cpp/${sample}.cpp DESTINATION samples/cpp )
endforeach(sample)

install( FILES CMakeLists.txt.in DESTINATION samples RENAME CMakeLists.txt )

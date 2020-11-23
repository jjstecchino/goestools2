add_executable(compute_sync_words compute_sync_words.cpp)
add_sanitizers(compute_sync_words)
target_link_libraries(compute_sync_words correct_static m stdc++)

add_library(packetizer
  correlator.cpp
  derandomizer.cpp
  packetizer.cpp
  reader.cpp
  reed_solomon.cpp
  )
target_link_libraries(packetizer correct_static)

add_executable(packetdump packetdump.cpp)
add_sanitizers(packetdump)
target_link_libraries(packetdump packetizer m stdc++)

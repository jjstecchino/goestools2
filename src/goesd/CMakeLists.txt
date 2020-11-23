find_package(PkgConfig)

add_library(publisher
  packet_publisher.cpp
  publisher.cpp
  sample_publisher.cpp
  soft_bit_publisher.cpp
  stats_publisher.cpp
  )
target_link_libraries(publisher nanomsg)

pkg_check_modules(AIRSPY libairspy)
if(NOT AIRSPY_FOUND)
  message(WARNING "Unable to find libairspy")
else()
  add_library(airspy_source airspy_source.cpp)
  target_link_libraries(airspy_source ${AIRSPY_LIBRARIES} publisher stdc++)
endif()

pkg_check_modules(RTLSDR librtlsdr)
if(NOT RTLSDR_FOUND)
  message(WARNING "Unable to find librtlsdr")
else()
  add_library(rtlsdr_source rtlsdr_source.cpp)
  if((RTLSDR_VERSION VERSION_EQUAL 0.5.4) OR
     (RTLSDR_VERSION VERSION_GREATER 0.5.4))
    target_compile_definitions(rtlsdr_source PRIVATE RTLSDR_HAS_BIAS_TEE)
  endif()
  target_link_libraries(rtlsdr_source ${RTLSDR_LIBRARIES} publisher stdc++)
endif()

add_library(nanomsg_source nanomsg_source.cpp)
target_link_libraries(nanomsg_source nanomsg publisher stdc++)

add_library(agc agc.cpp)
target_link_libraries(agc publisher m stdc++)

add_library(rrc rrc.cpp)
target_link_libraries(rrc publisher stdc++)

add_library(costas costas.cpp)
target_link_libraries(costas publisher stdc++)

add_library(clock_recovery clock_recovery.cpp)
target_link_libraries(clock_recovery publisher stdc++)

add_library(quantize quantize.cpp)
target_link_libraries(quantize publisher stdc++)

add_executable(goesd goesd.cpp config.cpp options.cpp decoder.cpp demodulator.cpp monitor.cpp datagram_socket.cpp source.cpp)
install(TARGETS goesd COMPONENT goestools RUNTIME DESTINATION bin)
target_include_directories(goesd PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(goesd util)
target_link_libraries(goesd nlohmann_json)
target_link_libraries(goesd packetizer pthread)
target_link_libraries(goesd agc)
target_link_libraries(goesd rrc)
target_link_libraries(goesd costas)
target_link_libraries(goesd clock_recovery)
target_link_libraries(goesd quantize)
target_link_libraries(goesd nanomsg_source)
target_link_libraries(goesd version)
if(AIRSPY_FOUND)
  target_compile_definitions(goesd PUBLIC -DBUILD_AIRSPY)
  target_link_libraries(goesd airspy_source)
endif()
if(RTLSDR_FOUND)
  target_compile_definitions(goesd PUBLIC -DBUILD_RTLSDR)
  target_link_libraries(goesd rtlsdr_source)
endif()

add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark pthread)
target_link_libraries(benchmark agc)
target_link_libraries(benchmark rrc)
target_link_libraries(benchmark costas)
target_link_libraries(benchmark clock_recovery)

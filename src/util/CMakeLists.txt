add_library(util fs.cpp string.cpp time.cpp)
target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

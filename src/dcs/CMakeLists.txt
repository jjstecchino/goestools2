add_library(dcs dcs.cpp)

add_executable(dcsdump dcsdump.cpp)
add_sanitizers(dcsdump)
target_link_libraries(dcsdump lrit dcs m stdc++)

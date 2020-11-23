add_library(lrit lrit.cpp file.cpp json.cpp)
target_link_libraries(lrit util nlohmann_json)

add_executable(lritdump lritdump.cpp)
add_sanitizers(lritdump)
target_link_libraries(lritdump lrit m stdc++)

add_executable(areadump areadump.cpp)
add_sanitizers(areadump)
target_link_libraries(areadump lrit dir m stdc++)

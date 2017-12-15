add_executable(workshop1.1 main.cpp)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(Freetype REQUIRED)
find_package(JPEG REQUIRED)
find_package(SFML 2 COMPONENTS window graphics system REQUIRED)

target_include_directories(workshop1.1 PRIVATE ${SFML_INCLUDE_DIR})
target_compile_features(workshop1.1 PUBLIC cxx_std_17)
target_compile_definitions(workshop1.1 PRIVATE SFML_STATIC)

target_link_libraries(workshop1.1 ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
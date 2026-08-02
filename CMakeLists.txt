cmake_minimum_required(VERSION 3.16)

project(AerospaceShieldingSim)

find_package(Geant4 REQUIRED ui_all vis_all)

include(${Geant4_USE_FILE})

file(GLOB sources CONFIGURE_DEPENDS *.cc *.cpp)
file(GLOB headers CONFIGURE_DEPENDS *.hh *.h)

add_executable(sim ${sources} ${headers})
target_link_libraries(sim PRIVATE ${Geant4_LIBRARIES})
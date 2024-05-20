cmake_minimum_required(VERSION 3.16)
project(ChessEngines LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(ChessEngines 
    src/main.cpp
    src/Board.cpp
    src/GameBoard.cpp
    src/Piece.cpp
    src/SimpleEngines.cpp
    src/MinMaxEngine.cpp
    src/MinMaxNoPruning.cpp
)

# Copy all images over
file(GLOB IMAGES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.png")
foreach(IMAGE ${IMAGES})
    get_filename_component(IMAGE_NAME ${IMAGE} NAME)
    configure_file(${IMAGE} ${CMAKE_CURRENT_BINARY_DIR}/${IMAGE_NAME} COPYONLY)
endforeach()

# Copy fonts over
set(FONT "${CMAKE_CURRENT_SOURCE_DIR}/src/Nunito-VariableFont_wght.ttf")
get_filename_component(FONT_NAME ${FONT} NAME)
configure_file(${FONT} ${CMAKE_CURRENT_BINARY_DIR}/${FONT_NAME} COPYONLY)
    
target_link_libraries(ChessEngines PRIVATE sfml-graphics)
target_compile_features(ChessEngines PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET ChessEngines
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
        VERBATIM)
endif()

install(TARGETS ChessEngines)

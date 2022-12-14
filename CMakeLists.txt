# OPTIMISATION FLAGS : FIXME
# In cmake, we define variables with set(VAR Value [Value]*)
# set(MYVAR "val1") assigns value val1 to MYVAR
# set(MYVAR "val1" "val2") creates a list

set(OPT_FLAGS
    "-Ofast -march=native"
)

# END OPTIMIZATION FLAGS
# SOURCES : FIXME
# Put all your sources here EXCEPT YOUR MAIN.CC
set(SRC_ENGINE
    src/board/board.cc
    src/board/move.cc
    src/game_tracer/game-tracer.cc
    src/pgn_parser/pgn-exception.cc
    src/pgn_parser/pgn-move.cc
    src/pgn_parser/pgn-parser.cc
    src/pieces/position.cc
    src/uci/uci-time.cc
    src/uci/uci.cc
    src/perft/perft.cc
    src/ai/AI.cc
    src/ai/random.cc
    src/ai/negamax.cc
    src/ai/alpha-beta.cc
    src/ai/evaluators/sum.cc
    src/ai/evaluators/simplified.cc
)

# Directories containing headers must be specified in the include_directories()
include_directories(
    "src/board"
    "src/game_tracer"
    "src/listener"
    "src/misc"
    "src/pgn_parser"
    "src/pieces"
    "src/uci"
    "src/perft"
    "src/ai"
    "src/ai/evaluators"
)

# Put your test files here
set(SRC_GTEST
    ""
)

# END SOURCES


# SETUP
cmake_minimum_required(VERSION 3.12)
project(Chess)

# BUILD OPTIONS
set(CMAKE_CXX_STANDARD 17)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic -std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPT_FLAGS}")
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g3 -fsanitize=address")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Ofast -march=native -fsanitize=address")

# LIBRARIES
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost REQUIRED COMPONENTS system program_options)
set(LIBRARIES Boost::system Boost::program_options ${CMAKE_DL_LIBS})

# CHESSENGINE
add_executable(chessengine)
target_sources(chessengine PRIVATE "src/main.cc" ${SRC_ENGINE})
target_link_libraries(chessengine PRIVATE ${LIBRARIES} -ldl)


# TESTS
# Check if GTest is installed
find_package(GTest)

# If it is, create a dedicated target "tests"
if(GTest_FOUND)
  include(GoogleTest)

  add_executable(tests)
  target_sources(tests PRIVATE
    ${SRC_GTEST}
    ${SRC_ENGINE}
  )
  target_compile_options(tests PRIVATE -Wall -Werror -Wextra)
  target_link_libraries(tests PRIVATE GTest::GTest ${LIBRARIES})
endif()

# STATIC TARGET
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(chessengine-static)
    target_compile_definitions(chessengine-static PRIVATE -D_STATIC)
    target_sources(chessengine-static PRIVATE "src/main.cc" ${SRC_ENGINE})
    target_link_libraries(chessengine-static PRIVATE ${LIBRARIES} -static)
endif()
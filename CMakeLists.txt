cmake_minimum_required(VERSION 3.0)

project(Chess)

include_directories(include src executables include/pieces src/pieces include/AI src/AI)

# Include the h files
set(CHESS_H
   include/board.h
   include/direction.h
   include/move.h
   include/position.h
   include/piece.h
   include/pieces/bishop.h
   include/pieces/king.h
   include/pieces/knight.h
   include/pieces/pawn.h
   include/pieces/queen.h
   include/pieces/rook.h)
set(AI_H
   include/AI/computer.h
   include/AI/depthcomputer.h
   include/AI/randomcomputer.h)
# Cpp source files
set(CHESS_CPP
   src/board.cpp
   src/direction.cpp
   src/move.cpp
   src/position.cpp
   src/piece.cpp
   src/pieces/bishop.cpp
   src/pieces/king.cpp
   src/pieces/knight.cpp
   src/pieces/pawn.cpp
   src/pieces/queen.cpp
   src/pieces/rook.cpp)
set(AI_CPP
   src/AI/computer.cpp
   src/AI/depthcomputer.cpp
   src/AI/randomcomputer.cpp)


add_library(chess ${CHESS_H} ${CHESS_CPP})
add_library(AI ${AI_H} ${AI_CPP})

add_executable(test executables/test.cpp ${CHESS_CPP} ${AI_CPP})

target_link_libraries(test chess AI)

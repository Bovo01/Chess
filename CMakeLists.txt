cmake_minimum_required(VERSION 3.0)

project(ProgettoScacchiera)

include_directories(include src executables include/pieces src/pieces)

add_library(position include/position.h src/position.cpp)
add_library(direction include/direction.h src/direction.cpp)
add_library(board include/board.h src/board.cpp)
add_library(piece include/piece.h src/piece.cpp)
add_library(move include/move.h src/move.cpp)
# Aggiungo tutti i pezzi uno ad uno
add_library(king include/pieces/king.h src/pieces/king.cpp)
add_library(queen include/pieces/queen.h src/pieces/queen.cpp)
add_library(rook include/pieces/rook.h src/pieces/rook.cpp)
add_library(bishop include/pieces/bishop.h src/pieces/bishop.cpp)
add_library(knight include/pieces/knight.h src/pieces/knight.cpp)
add_library(pawn include/pieces/pawn.h src/pieces/pawn.cpp)
# add_library(computer include/computer.h src/computer.cpp)


add_executable(scacchiera executables/scacchiera.cpp src/position.cpp src/piece.cpp src/board.cpp )
add_executable(test executables/test.cpp src/position.cpp src/piece.cpp src/board.cpp src/move.cpp
         src/pieces/king.cpp src/pieces/queen.cpp src/pieces/rook.cpp src/pieces/bishop.cpp src/pieces/knight.cpp src/pieces/pawn.cpp)

target_link_libraries(scacchiera position direction piece king queen rook bishop knight pawn board )
target_link_libraries(test position direction move piece king queen rook bishop knight pawn board )
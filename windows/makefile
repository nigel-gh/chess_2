# Directories
SRC_DIR = ../src
OBJ_DIR = obj
PIECES_DIR = $(SRC_DIR)/Pieces
PLAYERS_DIR = $(SRC_DIR)/Players
SMART_PLAYERS_DIR = $(PLAYERS_DIR)/SmartPlayers

# Files
OBJS = $(OBJ_DIR)/main.o \
       $(OBJ_DIR)/Player.o \
       $(OBJ_DIR)/Game.o \
       $(OBJ_DIR)/DummyPlayer.o \
       $(OBJ_DIR)/RandomPlayer.o \
       $(OBJ_DIR)/BruteForcePlayer.o \
       $(OBJ_DIR)/AlphaBetaPlayer.o \
       $(OBJ_DIR)/Board.o \
       $(OBJ_DIR)/Piece.o \
       $(OBJ_DIR)/Move.o \
       $(OBJ_DIR)/ZobristHash.o \
       $(OBJ_DIR)/Rook.o \
       $(OBJ_DIR)/King.o \
       $(OBJ_DIR)/Bishop.o \
       $(OBJ_DIR)/Knight.o \
       $(OBJ_DIR)/Queen.o \
       $(OBJ_DIR)/Pawn.o \
       $(OBJ_DIR)/TypesAndEnums.o

SOURCE = $(SRC_DIR)/main.cpp \
         $(PLAYERS_DIR)/Player.cpp \
         $(SRC_DIR)/Game.cpp \
         $(PLAYERS_DIR)/DummyPlayer.cpp \
         $(PLAYERS_DIR)/RandomPlayer.cpp \
         $(SMART_PLAYERS_DIR)/BruteForcePlayer.cpp \
         $(SMART_PLAYERS_DIR)/AlphaBetaPlayer.cpp \
         $(SRC_DIR)/Board.cpp \
         $(PIECES_DIR)/Piece.cpp \
         $(SRC_DIR)/Move.cpp \
         $(SRC_DIR)/ZobristHash.cpp \
         $(PIECES_DIR)/Rook.cpp \
         $(PIECES_DIR)/King.cpp \
         $(PIECES_DIR)/Bishop.cpp \
         $(PIECES_DIR)/Knight.cpp \
         $(PIECES_DIR)/Queen.cpp \
         $(PIECES_DIR)/Pawn.cpp \
         $(SRC_DIR)/TypesAndEnums.cpp \

OUT     = chessgame
CC      = g++
FLAGS   = -g -c -Wall -std=c++2a
LFLAGS  =

all: $(OBJ_DIR) $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) $< -o $@

$(OBJ_DIR)/%.o:    $(PIECES_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(PLAYERS_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) $< -o $@

$(OBJ_DIR)/%.o: $(SMART_PLAYERS_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) $< -o $@

clean:
	rmdir /s /q $(OBJ_DIR)

.PHONY: all clean
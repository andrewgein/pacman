UNAME_S := $(shell uname -s)

EXE = pacman 
SRC = $(wildcard src/*.cpp)
LIBSRC = $(wildcard lib/*.c)

OUTPATH = bin/release/
OUTFILE = $(OUTPATH)$(EXE)
OBJPATH = obj/release/
OBJ = $(addprefix $(OBJPATH),$(notdir $(SRC:.cpp=.o)))
LIBOBJ = $(addprefix $(OBJPATH),$(notdir $(LIBSRC:.c=.o)))

DEBUG_OUTPATH = bin/debug/
DEBUG_OUTFILE = $(DEBUG_OUTPATH)$(EXE)
DEBUG_OBJPATH = obj/debug/
DEBUG_OBJ = $(addprefix $(DEBUG_OBJPATH),$(notdir $(SRC:.cpp=.o)))
LIBOBJ = $(addprefix $(OBJPATH),$(notdir $(LIBSRC:.c=.o)))


ifeq ($(UNAME_S),Darwin)
CXX = clang++
CC = clang
CXXFLAGS = -DNDEBUG
CCFLAGS = -DNDEBUG
STDFLAG = -std=c++11
debug : CCFLAGS = -g
debug : CXXFLAGS = -g
INCLUDE_FLAGS := -Wno-unused-command-line-argument -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -I/Library/Frameworks/SDL2_image.framework/Headers -F/Library/Frameworks -framework SDL2_image -I/Library/Frameworks/SDL2_ttf.framework/Headers -F/Library/Frameworks -framework SDL2_ttf -I/Library/Frameworks/SDL2_mixer.framework/Headers -F/Library/Frameworks -framework SDL2_mixer
else ifeq ($(UNAME_S),Linux)
CXX = g++
CC = gcc 
CXXFLAGS = -DNDEBUG
CCFLAGS = -DNDEBUG
STDFLAG = -std=c++11
debug : CCFLAGS = -g
debug : CXXFLAGS = -g
INCLUDE_FLAGS = -I/usr/include/SDL2/ -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf 
endif

INCLUDE_FLAGS += -I./lib -I./include

all: $(OUTFILE)
	ln -sf $(OUTFILE) $(EXE)

debug: $(DEBUG_OUTFILE)
	ln -sf $(DEBUG_OUTFILE) $(EXE)

$(OUTFILE): $(OBJ) $(LIBOBJ)
	mkdir -p $(OUTPATH)
	$(CXX) $(STDFLAG) $(CXXFLAGS) $(INCLUDE_FLAGS) $(OBJ) $(LIBOBJ) -o $(OUTFILE)

$(DEBUG_OUTFILE): $(DEBUG_OBJ) $(LIBOBJ)
	mkdir -p $(DEBUG_OUTPATH)
	mkdir -p $(OUTPATH)
	$(CXX) $(STDFLAG) $(CXXFLAGS) $(INCLUDE_FLAGS) $(DEBUG_OBJ) $(LIBOBJ) -o $(DEBUG_OUTFILE)

$(OBJPATH)%.o: src/%.cpp | obj
	$(CXX) $(STDFLAG) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(DEBUG_OBJPATH)%.o: src/%.cpp | obj
	$(CXX) $(STDFLAG) $(CXXFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(OBJPATH)%.o: lib/%.c | obj
	$(CC) $(CCFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(DEBUG_OBJPATH)%.o: lib/%.c | obj
	$(CC) $(CCFLAGS) $(INCLUDE_FLAGS) -c $< -o $@

obj:
	mkdir -p $(OBJPATH)
	mkdir -p $(DEBUG_OBJPATH)

clean:
	rm -f *.a *.o *.out game.dat pacman
	rm -rf obj bin
	rm -f .DS_Store lib/.DS_Store src/.DS_Store resources/.DS_Store resources/images/.DS_Store
	rm -rf *.dSYM
	rm -rf .cache

.PHONY: debug all

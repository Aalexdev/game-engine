# project
VERSION = 0.1.0

# compiler
CXX = g++
STD_VERSION = c++17
LIBSFLAGS = -l mingw32 -l engine -l yaml-cpp
CFLAGS =
DEFINES = -DVERSION='"$(VERSION)"' -D ENGINE_ASSERTS -D ENGINE_PROFILE
INCLUDE = include/ -I "../game engine/include/"

# directories
BIN = out
SRC = src
OBJ = .obj
LIB = libs/
EXEC = editor

# source files
SRCS = $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/**/*.cpp) $(wildcard $(SRC)/**/**/*.cpp)
OBJS := $(patsubst %.cpp, $(OBJ)/%.o, $(notdir $(SRCS)))

all: $(EXEC)

release: CFLAGS = -Wall -O2 -D NDEBUG
release: clean
release: $(EXEC)

dbg: CFLAGS = -g3
dbg: all

dbgRebuild: CFLAGS = -g3
dbgRebuild: rebuild

rebuild: clean
rebuild: $(EXEC)

clean:
	@del $(OBJ)\*.o

$(EXEC) : $(OBJS)
	$(CXX) -std=$(STD_VERSION) $(OBJ)/*.o -I $(INCLUDE) -L $(LIB) -o $(BIN)\$(EXEC) $(CFLAGS) $(DEFINES) $(LIBSFLAGS)

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CXX) -std=$(STD_VERSION) -o $@ -c $< -I $(INCLUDE) -L $(LIB) $(DEFINES) $(CFLAGS)

$(OBJ)/%.o : $(SRC)/*/%.cpp
	$(CXX) -std=$(STD_VERSION) -o $@ -c $< -I $(INCLUDE) -L $(LIB) $(DEFINES) $(CFLAGS)

$(OBJ)/%.o : $(SRC)/*/*/%.cpp
	$(CXX) -std=$(STD_VERSION) -o $@ -c $< -I $(INCLUDE) -L $(LIB) $(DEFINES) $(CFLAGS)

info:
	@echo -----------------------------------------------------
	@echo info :                
	@echo 	compiler                     : $(CXX)
	@echo 	compiler standart version    : $(STD_VERSION)
	@echo 	flags                        : $(CFLAGS)
	@echo 	defines                      : $(DEFINES)
	@echo 	executable                   : $(EXEC)
	@echo 	libs directory               : $(LIB)
	@echo 	binary directory             : $(BIN)
	@echo 	source code directory        : $(SRC)
	@echo 	compiled object directory    : $(OBJ)
	@echo 	include directory            : $(INCLUDE)
	@echo -----------------------------------------------------
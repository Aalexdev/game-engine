# project
VERSION = 0.1.0

# compiler
CXX = g++
STD_VERSION = c++17
LIBSFLAGS = -lmingw32
CFLAGS =
DEFINES = -D VERSION='"$(VERSION)"' -D HORREUM_ASSERTS -D HORREUM_PROFILE -D HORREUM_ALLOC_LOG
INCLUDE = include/

# directories
BIN = out
SRC = src
OBJ = .obj
LIB = libs
EXEC = libhorreum

# source files
SRCS = $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/**/*.cpp) $(wildcard $(SRC)/**/**/*.cpp)
OBJS := $(patsubst %.cpp, $(OBJ)/%.o, $(notdir $(SRCS)))

all: $(EXEC)

test:
	g++ -o test .\test.cpp -L.\out\ -lhorreum

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
	ar crs $(BIN)/$(EXEC).a $(OBJ)/*.o

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
	@echo 	lib name                     : $(EXEC)
	@echo 	libs directory               : $(LIB)
	@echo 	binary directory             : $(BIN)
	@echo 	source code directory        : $(SRC)
	@echo 	compiled object directory    : $(OBJ)
	@echo 	include directory            : $(INCLUDE)
	@echo -----------------------------------------------------
# project
VERSION = 0.1.0

# compiler
CXX = g++
STD_VERSION = c++17
LIBSFLAGS = -lmingw32
CFLAGS =
DEFINES = 
INCLUDE = ../../include/libs/

# directories
BIN = out
OBJ = .obj
OUT = libGlad
LIB = ../../libs/

# source files
SRCS = $(wildcard *.c) $(wildcard **/*.c) $(wildcard **/**/*.c)
OBJS := $(patsubst %.c, $(OBJ)/%.o, $(notdir $(SRCS)))

all: $(OUT)

release: CFLAGS = -Wall -O2 -D NDEBUG
release: $(OUT) 

dbg: CFLAGS = -g3
dbg: all

dbgRebuild: CFLAGS = -g3
dbgRebuild: rebuild

rebuild: clean
rebuild: $(OUT)

clean:
	@del $(OBJ)\*.o

$(OUT) : $(OBJS)
	ar rcs $(LIB)$(OUT).a $(OBJ)/*.o

$(OBJ)/%.o : %.c
	$(CXX) -std=$(STD_VERSION) -o $@ -c $< -I $(INCLUDE) $(DEFINES) $(CFLAGS)

$(OBJ)/%.o : */%.c
	$(CXX) -std=$(STD_VERSION) -o $@ -c $< -I $(INCLUDE) $(DEFINES) $(CFLAGS)

$(OBJ)/%.o : */*/%.c
	$(CXX) -std=$(STD_VERSION) -o $@ -c $< -I $(INCLUDE) $(DEFINES) $(CFLAGS)
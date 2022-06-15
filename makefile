# project
VERSION = 0.1.0

# compiler
CXX = g++
STD_VERSION = c++17
LIBSFLAGS = -lmingw32 -lglfw3 -lSDL2main -lSDL2 -mwindows -Wl,--dynamicbase -Wl,--nxcompat -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid -lvulkan-1 -lyaml-cpp -lglad -lImGui -lImGuiFileDialog -lImGuizmo -lbox2d
CFLAGS =
DEFINES = -DVERSION='"$(VERSION)"' -D ENGINE_BUILD_DLL -D ENGINE_ASSERTS -D ENGINE_PROFILE
INCLUDE = include/

# directories
BIN = out
SRC = src
OBJ = .obj
LIB = libs
DLL = engine

# source files
SRCS = $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/**/*.cpp) $(wildcard $(SRC)/**/**/*.cpp)
OBJS := $(patsubst %.cpp, $(OBJ)/%.o, $(notdir $(SRCS)))

all: $(DLL)

test:
	$(CXX) -std=$(STD_VERSION) -I $(INCLUDE) tests/*.cpp -o out/test.exe -L out/ -l engine

release: CFLAGS = -Wall -O2 -D NDEBUG
release: clean
release: $(DLL)

dbg: CFLAGS = -g3
dbg: all

dbgRebuild: CFLAGS = -g3
dbgRebuild: rebuild

rebuild: clean
rebuild: $(DLL)

clean:
	@del $(OBJ)\*.o


$(DLL) : $(OBJS)
	$(CXX) -std=$(STD_VERSION) -shared $(OBJ)/*.o -I $(INCLUDE) -L $(LIB) -o $(BIN)\$(DLL).dll $(CFLAGS) $(DEFINES) $(LIBSFLAGS) -Wl,--out-implib,$(BIN)\$(DLL).lib

$(OBJ)/%.o : $(SRC)/%.cpp
	$(CXX) -std=$(STD_VERSION) -shared -o $@ -c $< -I $(INCLUDE) -L $(LIB) $(DEFINES) $(CFLAGS)

$(OBJ)/%.o : $(SRC)/*/%.cpp
	$(CXX) -std=$(STD_VERSION) -shared -o $@ -c $< -I $(INCLUDE) -L $(LIB) $(DEFINES) $(CFLAGS)

$(OBJ)/%.o : $(SRC)/*/*/%.cpp
	$(CXX) -std=$(STD_VERSION) -shared -o $@ -c $< -I $(INCLUDE) -L $(LIB) $(DEFINES) $(CFLAGS)

info:
	@echo -----------------------------------------------------
	@echo info :                
	@echo 	compiler                     : $(CXX)
	@echo 	compiler standart version    : $(STD_VERSION)
	@echo 	flags                        : $(CFLAGS)
	@echo 	defines                      : $(DEFINES)
	@echo 	lib name                     : $(DLL)
	@echo 	libs directory               : $(LIB)
	@echo 	binary directory             : $(BIN)
	@echo 	source code directory        : $(SRC)
	@echo 	compiled object directory    : $(OBJ)
	@echo 	include directory            : $(INCLUDE)
	@echo -----------------------------------------------------
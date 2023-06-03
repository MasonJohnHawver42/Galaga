PLATFORM ?= LINUX

# -limgui -lrlImGui
# -I./include/imgui -I./include/rlImGui
# --preload-file data --embed-file imgui.ini

ifeq ($(PLATFORM), LINUX)
	CC = g++
	OUTPUT = main
	LIBS = -L./lib/linux -lraylib -llua -lGL -lm -lpthread -ldl -lrt -lX11
	FLAGS = --std=c++17 -Wall -g
	MESS = $(OUTPUT)
endif

ifeq ($(PLATFORM), WEB)
	CC = emcc
	OUTPUT = index.html
	LIBS = -L./lib/web -lraylib -llua -lGL -lm -lpthread -ldl -lrt -lX11
	FLAGS = --std=c++17 -Os -Wall -s USE_GLFW=3 -s ASYNCIFY -s TOTAL_MEMORY=67108864 -s FORCE_FILESYSTEM=1 -s ASSERTIONS=1 --shell-file shell.html --preload-file assets -DPLATFORM_WEB
	MESS = $(OUTPUT) index.js index.wasm index.data
endif

SOURCE = main.cpp 
INCLUDE = -I./include/ -I./include/raylib -I./include/lua

$(OUTPUT): $(SOURCE)
	$(CC) -o $(OUTPUT) main.cpp $(INCLUDE) $(LIBS) $(FLAGS)

clean:
	rm $(MESS)
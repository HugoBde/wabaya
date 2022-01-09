CC = clang++
LIBS = 
INCLUDES = 
FLAGS = -Wall -Wextra -std=c++17
BIN = bin/main
OBJS = obj/main.o obj/utils.o obj/emulator.o obj/memory.o obj/cpu.o

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $^ -o $@ $(FLAGS) $(LIBS)

obj/main.o: src/main.cpp src/emulator.h
obj/utils.o: src/utils.cpp src/utils.h
obj/emulator.o: src/emulator.cpp src/emulator.h src/utils.h src/cpu.h src/memory.h src/display.h src/soundsystem.h
obj/memory.o: src/memory.cpp src/memory.h src/utils.h
obj/cpu.o: src/cpu.cpp src/cpu.h src/memory.h

$(OBJS):
	$(CC) $< -o $@ -c $(FLAGS) $(INCLUDES)

clean:
	rm bin/* obj/*
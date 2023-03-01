SRC = $(wildcard src/*.c)
OBJ = $(filter-out $(MOBJ),$(patsubst src/%.c,obj/%.o,$(SRC)))
BIN = $(filter-out $(MBIN),$(patsubst obj/%.o,bin/%,$(OBJ)))

.PHONY: all

all    : $(BIN) init
$(OBJ) : $(SRC)
$(BIN) : $(OBJ)

obj/%.o: src/%.c
	gcc -g -c $< -o $@
bin/%: obj/%.o
	gcc -o $@ $<

init:
	mkdir -p bin obj

demo: all
	cp bin/get demo/get


clean:
	rm -f obj/* bin/*
	rm -f *.EXE *.OBJ
	rm -f src/*.EXE src/*.OBJ src/*.O
	rm -f src/*.exe src/*.obj src/*.o

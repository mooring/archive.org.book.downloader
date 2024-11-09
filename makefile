SRC = $(wildcard src/*.c)
OBJ = $(filter-out $(MOBJ),$(patsubst src/%.c,obj/%.o,$(SRC)))
BIN = $(filter-out $(MBIN),$(patsubst obj/%.o,demo/%,$(OBJ)))


.PHONY: all

all    : $(BIN) init
$(OBJ) : $(SRC)
$(BIN) : $(OBJ)

obj/%.o: src/%.c
	gcc -g -c $< -o $@
demo/%: obj/%.o
	gcc -o $@ $<

init:
	mkdir -p demo obj

clean:
	rm -f obj/* demo/get
	rm -f *.EXE *.OBJ
	rm -f src/*.EXE src/*.OBJ src/*.O
	rm -f src/*.exe src/*.obj src/*.o

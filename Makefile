# Single compilation unit makefile
CC=cc
SDLINC = `sdl2-config --cflags`
SDLLIB = `sdl2-config --libs`
INCS = -I. ${SDLINC}
LIBS = -lm ${SDLLIB}
CFLAGS = -std=c11 -ggdb -Wall ${INCS} ${LIBS}

.PHONY: all options clean

all: options clean sp

options:
	@echo sp build options:
	@echo "CC       = ${CC}"
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"

clean:
	@echo cleaning
	@rm -f sp

sp: ${OBJ}
	${CC} -o $@ $@.c ${CFLAGS}

.ONESHELL:
emcc:
	@cp data wasm/
	@cd wasm
	@source ~/workspace/emsdk/emsdk_env.sh
	@emcc ../sp.c -O3 -s WASM=1 -s USE_SDL=2 -s MODULARIZE=1 --preload-file data -o sp.js
	@rm data

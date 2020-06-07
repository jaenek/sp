NAME = sp

PKGS = sdl2 SDL2_ttf
CFLAGS = -Wall -Wextra -pedantic -I. -std=c11 -ggdb
LIBS = $(shell pkg-config --libs $(PKGS)) -lm

OBJ = lib/SDL_FontCache.o src/${NAME}.o

.PHONY: all options clean

all: clean options ${NAME}

options:
	@echo ${NAME} build options:
	@echo "CC     = ${CC}"
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LIBS   = ${LIBS}"

clean:
	@echo cleaning
	@rm -vf ${NAME} src/${NAME}.o

${NAME}: ${OBJ}
	${CC} ${CFLAGS} -o $@ $^ ${LIBS}

src/%.o: src/%.c
lib/%.o: lib/%.c
	${CC} ${CFLAGS} -c -o $@ $< ${LIBS}

.ONESHELL:
emcc:
	@cp data wasm/
	@cd wasm
	@source ~/workspace/emsdk/emsdk_env.sh
	@emcc ../sp.c -O3 -s WASM=1 -s USE_SDL=2 -s MODULARIZE=1 --preload-file data -o sp.js
	@rm data

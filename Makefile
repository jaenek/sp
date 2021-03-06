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


EMCCFLAGS = -O3 -s WASM=1 -s MODULARIZE=1 -s ALLOW_MEMORY_GROWTH=1
EMCCLIBS = -s USE_SDL=2 -s USE_SDL_TTF=2
EMCCDATA = --preload-file data --embed-file font
.ONESHELL:
emcc:
	@source ~/workspace/emsdk/emsdk_env.sh
	@emcc -c lib/SDL_FontCache.c ${EMCCFLAGS} ${EMCCLIBS}
	@emcc src/sp.c SDL_FontCache.o ${EMCCFLAGS} ${EMCCLIBS} ${EMCCDATA} -o wasm/sp.js
	@rm SDL_FontCache.o

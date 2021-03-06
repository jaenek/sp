// Single Compile Unit
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#if EMSCRIPTEN
#include <emscripten.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../lib/SDL_FontCache.h"

#include "sp_config.c"
#include "sp_input.c"
#include "sp_plot.c"
#include "sp_main.c"

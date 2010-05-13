#ifdef __PSP
#include <psptypes.h>
#include "PSP_graphics.h"

Color* g_vram_base = (Color*) (0x40000000 | 0x04000000);
#endif // __PSP

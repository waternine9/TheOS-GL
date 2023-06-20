
#define PROBE_VALUE(v) do { asm("cli"); asm("hlt" :: "a" (v)); } while(0)


#define PROBE_VALUE(v) asm("cli\nhlt" :: "a" (v))

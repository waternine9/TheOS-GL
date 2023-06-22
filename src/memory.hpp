#ifndef H_TOS_MEMORY
#define H_TOS_MEMORY

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" 
{
#endif // __cplusplus

void  memcpy(void *Destination_, const void *Source_, size_t N);
void  memset(void *Destination_, uint8_t Val, size_t N);
void* malloc(size_t Bytes);
void  free(void *Buf);
void* memmove(void *dest, const void *src, size_t n);
int strlen(const char *s);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // H_TOS_MEMORY
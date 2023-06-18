#include <cstdint>
#include <cstddef>

inline static void
kmemset(void *destination_param, int value, size_t count)
{
    uint8_t *destination = (uint8_t*)destination_param;

    for (size_t i = 0; i < count; i++)
    {
        destination[i] = value;
    }
}

inline static void
kmemcpy(void *destination_param, void *source_param, size_t count)
{
    uint8_t *destination = (uint8_t*)destination_param;
    uint8_t *source = (uint8_t*)source_param;

    for (size_t i = 0; i < count; i++)
    {
        destination[i] = source[i];
    }
}

void
kalloc_init();

void* 
kmalloc(size_t n);

void
free(void *pointer);

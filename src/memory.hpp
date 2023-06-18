#include <cstdint>
#include <cstddef>

inline void
kmemset(void *destination_param, int value, size_t count)
{
    uint8_t *destination = (uint8_t*)destination_param;

    for (int i = 0; i < count; i++) {
        destination[i] = value;
    }
}

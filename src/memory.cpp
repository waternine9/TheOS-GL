#include "memory.hpp"
#define STARTING_LOCATION 0x1000000
#define ONE_GIGABIT 1073741824
#define MEMORY_LIMIT ONE_GIGABIT

typedef struct list_node {
    bool taken;
    struct list_node *prev;
    struct list_node *next;

    // NOTE: Size excluding the node size.
    size_t size; 
} ListNode;

typedef struct {
    ListNode *free_list_start;
    ListNode *free_list_end;
} FreeListAllocator;

FreeListAllocator FREE_LIST;

void
kalloc_init()
{
    // - Set up first node.
    ListNode *first_node = (ListNode*)STARTING_LOCATION;

    // - Initialize first node.
    first_node->taken = false;
    first_node->prev = nullptr;
    first_node->next = nullptr;
    first_node->size = ONE_GIGABIT;

    // - Set up pointers.
    FREE_LIST.free_list_start = first_node;
    FREE_LIST.free_list_end = first_node;
}

void* 
kmalloc(size_t n)
{
    return(NULL);
}

void
free(void *pointer)
{
    return;
}

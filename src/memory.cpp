//
// TODO
// - Defragmentation of nodes..
// - Core protection. (Freeing invalid addresses)
// 

#include "memory.hpp"
#include "debug.hpp"

#define STARTING_LOCATION 0x2000000
#define ONE_GIGABIT 1073741824
#define MEMORY_LIMIT ONE_GIGABIT
#define ALIGNMENT 32
#define SIZE_ALIGNMENT 4096
#define TAKEN 1
#define NOT_TAKEN 0


typedef struct list_node {
    size_t taken;
    struct list_node *prev;
    struct list_node *next;

    // NOTE: Size excluding the node size.
    size_t size; 
} __attribute__((__aligned__(ALIGNMENT))) ListNode;

typedef struct {
    ListNode *free_list_start;
    ListNode *free_list_end;
} FreeListAllocator;


FreeListAllocator FREE_LIST;

static void
fl_detach_node(ListNode *node) 
{
    ListNode *prev = node->prev;
    if (prev) {
        prev->next = node->next;
    }

    ListNode *next = node->next;
    if (next) {
        next->prev = node->prev;
    }

    node->prev = nullptr;
    node->next = nullptr;
}

static void
fl_claim_node(FreeListAllocator *free_list, ListNode *node)
{
    // - Check if any node is in internal free list structures.
    if (free_list->free_list_start == node) {
        free_list->free_list_start = node->next;
    }

    if (free_list->free_list_end == node) {
        free_list->free_list_end = node->prev;
    }

    // - Detach the node from other nodes in the free list.
    fl_detach_node(node);

    // - Claim the node.
    node->taken = TAKEN;
}

static void
fl_release_node(FreeListAllocator *free_list, ListNode *node)
{
    // - Connect to free list's internal structures.
    node->prev = free_list->free_list_end;
    free_list->free_list_end->next = node;
    free_list->free_list_end = node;

    // - Release the node.
    node->taken = NOT_TAKEN;
}

static ListNode*
fl_find_best_fitting_node(FreeListAllocator *free_list, size_t size)
{
    ListNode *closest_node = free_list->free_list_start;
    size_t closest_node_size = closest_node->size;
    
    for (ListNode *current_node = closest_node; current_node != nullptr; current_node = current_node->next)
    {
        if (current_node->size >= size && current_node->size < closest_node_size)
        {
            closest_node = current_node;
            closest_node_size = current_node->size;
        }
    }

    return closest_node;
}

static size_t
align_size(size_t size, size_t alignment)
{
    return (size/alignment + 1)*alignment;
}

static void*
get_node_data(ListNode *node)
{
    return (void*)(node+1);
}

static ListNode*
fl_fragment_node(ListNode *node, size_t size)
{
    // - Check if there's enough memory in the node.
    if (node->size <= (size + sizeof(ListNode))) {
        return nullptr;
    }

    // - Allocate the new node.
    uint8_t *data = (uint8_t*)get_node_data(node);
    ListNode *new_node = (ListNode*)(data + size);

    // - Create the new node.
    new_node->taken = NOT_TAKEN;
    new_node->prev = nullptr;
    new_node->next = nullptr;
    new_node->size = node->size - size - sizeof(ListNode);

    // - Resize the original node.
    node->size = size;

    // - Give new node to the user.
    return new_node;
}

static void*
fl_allocate_memory(FreeListAllocator *free_list, size_t size)
{
    // - Find best fitting node.
    ListNode *closest_node = fl_find_best_fitting_node(free_list, size);

    // - Split the node into a smaller size.
    ListNode *fragmented_node = fl_fragment_node(closest_node, align_size(size, SIZE_ALIGNMENT));
    if (fragmented_node != nullptr)
    {
        fl_release_node(free_list, fragmented_node);
    }
    
    // - Claim the closest node.
    fl_claim_node(free_list, closest_node);

    // - Return the data after the node.
    return get_node_data(closest_node);
}

static void
fl_deallocate_memory(FreeListAllocator *free_list, void *ptr)
{
    // - Get list node from data pointer.
    ListNode *node = (ListNode*)(ptr) - 1;

    // - Release the node.
    fl_release_node(free_list, node);
}

void
kalloc_init()
{
    // - Set up first node.
    ListNode *first_node = (ListNode*)STARTING_LOCATION;

    // - Initialize first node.
    first_node->taken = NOT_TAKEN;
    first_node->prev = nullptr;
    first_node->next = nullptr;
    first_node->size = ONE_GIGABIT - sizeof(ListNode);

    // - Set up pointers.
    FREE_LIST.free_list_start = first_node;
    FREE_LIST.free_list_end = first_node;
}

void* 
kmalloc(size_t n)
{
    return(fl_allocate_memory(&FREE_LIST, n));
}

void
kfree(void *pointer)
{
    fl_deallocate_memory(&FREE_LIST, pointer);
}

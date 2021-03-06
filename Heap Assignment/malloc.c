/*
Gabriel de Sa
UTA ID: 1001676832
Heap Assignment Fall 2020
*/
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ALIGN4(s) (((((s)-1) >> 2) << 2) + 4)
#define BLOCK_DATA(b) ((b) + 1)
#define BLOCK_HEADER(ptr) ((struct _block *)(ptr)-1)

static int atexit_registered = 0;
static int num_mallocs = 0;
static int num_frees = 0;
static int num_reuses = 0;
static int num_grows = 0;
static int num_splits = 0;
static int num_coalesces = 0;
static int num_blocks = 0;
static int num_requested = 0;
static int max_heap = 0;

struct _block {
  size_t size;         /* Size of the allocated _block of memory in bytes */
  struct _block *prev; /* Pointer to the previous _block of allcated memory   */
  struct _block *next; /* Pointer to the next _block of allcated memory   */
  bool free;           /* Is this _block free?                     */
  char padding[3];
};

struct _block *heapList = NULL; /* Free list to track the _blocks available */
/*
 *  \brief printStatistics
 *
 *  \param none
 *
 *  Prints the heap statistics upon process exit.  Registered
 *  via atexit()
 *
 *  \return none
 */

void printStatistics(void) {
  struct _block *n = heapList;
  while (n) {
    num_blocks++;
    max_heap += n->size;
    n = n->next;
  }
  printf("\nheap management statistics\n");
  printf("mallocs:\t%d\n", num_mallocs);
  printf("frees:\t\t%d\n", num_frees);
  printf("reuses:\t\t%d\n", num_reuses);
  printf("grows:\t\t%d\n", num_grows);
  printf("splits:\t\t%d\n", num_splits);
  printf("coalesces:\t%d\n", num_coalesces);
  printf("blocks:\t\t%d\n", num_blocks);
  printf("requested:\t%d\n", num_requested);
  printf("max heap:\t%d\n", max_heap);
}

/*
 * \brief findFreeBlock
 *
 * \param last pointer to the linked list of free _blocks
 * \param size size of the _block needed in bytes
 *
 * \return a _block that fits the request or NULL if no free _block matches
 *
 * \TODO Implement Next Fit
 * \TODO Implement Best Fit
 * \TODO Implement Worst Fit
 */
struct _block *findFreeBlock(struct _block **last, size_t size) {
  struct _block *curr = heapList;

#if defined FIT && FIT == 0
  /* First fit */
  while (curr && !(curr->free && curr->size >= size)) {
    *last = curr;
    curr = curr->next;
  }
#endif

#if defined BEST && BEST == 0
  struct _block *lastBest = NULL;
  int bestSize = 1000000, currSize = 0;
  while (curr) {
    *last = curr;
    if (curr->free == true) {
      currSize = curr->size - size;
      if (bestSize >= currSize) {
        bestSize = currSize;
        lastBest = curr;
      }
    }
    curr = curr->next;
  }
  if (lastBest) {
    curr = lastBest;
  }

#endif

#if defined WORST && WORST == 0
  struct _block *lastWorst = NULL;
  int worstSize = 0;
  while (curr) {
    *last = curr;
    if (curr->free && (worstSize <= curr->size)) {
      worstSize = curr->size;
      lastWorst = curr;
    }
    curr = curr->next;
  }
  if (lastWorst) {
    curr = lastWorst;
  }
#endif

#if defined NEXT && NEXT == 0
  /* Next Fit */
  if (*last != NULL) {
    curr = *last;
    curr = curr->next;
  }
  while (curr && !(curr->free && curr->size >= size)) {
    *last = curr;
    curr = curr->next;
  }
#endif

  return curr;
}

/*
 * \brief growheap
 *
 * Given a requested size of memory, use sbrk() to dynamically
 * increase the data segment of the calling process.  Updates
 * the free list with the newly allocated memory.
 *
 * \param last tail of the free _block list
 * \param size size in bytes to request from the OS
 *
 * \return returns the newly allocated _block of NULL if failed
 */
struct _block *growHeap(struct _block *last, size_t size) {
  /* Request more space from OS */
  struct _block *curr = (struct _block *)sbrk(0);
  struct _block *prev = (struct _block *)sbrk(sizeof(struct _block) + size);

  assert(curr == prev);

  /* OS allocation failed */
  if (curr == (struct _block *)-1) {
    return NULL;
  }

  num_grows++;
  /* Update heapList if not set */
  if (heapList == NULL) {
    heapList = curr;
  }

  /* Attach new _block to prev _block */
  if (last) {
    last->next = curr;
  }

  /* Update _block metadata */
  curr->size = size;
  curr->next = NULL;
  curr->free = false;
  return curr;
}

/*
 * \brief malloc
 *
 * finds a free _block of heap memory for the calling process.
 * if there is no free _block that satisfies the request then grows the
 * heap and returns a new _block
 *
 * \param size size of the requested memory in bytes
 *
 * \return returns the requested memory allocation to the calling process
 * or NULL if failed
 */
void *malloc(size_t size) {
  if (atexit_registered == 0) {
    atexit_registered = 1;
    atexit(printStatistics);
  }

  /* Align to multiple of 4 */
  size = ALIGN4(size);

  /* Handle 0 size */
  if (size == 0) {
    return NULL;
  }

  /* Look for free _block */
  struct _block *last = heapList;
  struct _block *next = findFreeBlock(&last, size);

  /* Could not find free _block, so grow heap */
  if (next == NULL) {
    next = growHeap(last, size);
  } else {
    num_reuses++;
  }


  /* Split free _block if possible */
  if (next->size > size) {
    if ((next->size - size) > sizeof(struct _block)) {
      int s = next->size;
      struct _block *_next = next->next;
      uint8_t *ptr = (uint8_t *)next;
      next->next = (struct _block *)(ptr + size + sizeof(struct _block));
      next->next->free = true;
      next->next->size = s - size - sizeof(struct _block);
      next->next->next = _next;
      num_splits++;
    }
  }
  /* Could not find free _block or grow heap, so just return NULL */
  if (next == NULL) {
    return NULL;
  }

  /* Mark _block as in use */
  next->free = false;
  num_mallocs++;
  num_requested += size;
  /* Return data address associated with _block */
  return BLOCK_DATA(next);
}

/*
 * \brief free
 *
 * frees the memory _block pointed to by pointer. if the _block is adjacent
 * to another _block then coalesces (combines) them
 *
 * \param ptr the heap memory to free
 *
 * \return none
 */
void free(void *ptr) {
  num_frees++;
  if (ptr == NULL) {
    return;
  }
  /* Make _block as free */
  struct _block *curr = BLOCK_HEADER(ptr);
  assert(curr->free == 0);
  curr->free = true;

  /* TODO: Coalesce free _blocks if needed */
  struct _block *i = heapList;
  while (i != 0x0 && i->next != 0x0) {
    if (i->free == true && i->next->free == true) {
      i->size += (i->next->size + sizeof(i->next));
      i->next = i->next->next;
      num_coalesces++;
    }
    i = i->next;
  }
}

/*
  \brief calloc

  calloc allocates memory _block of an nmemb size and sets it to zero.

  \param nmemb that has the number of elements to allocate.
  \param size of the requested memory in bytes

  \return Address of the beginning of the list. NULL if it failed.
*/
void *calloc(size_t nmemb, size_t size) {
  struct _block *ptr = malloc(size * nmemb);
  memset(ptr, 0, size * nmemb);
  return ptr;
}

/*
  \brief realloc

  realloc takes dynamic memory and expands it to a new requested size.

  \param ptr a ptr to an existing allocated piece of memory
  \param size of the requested memory to expand in bytes

  \return Address of the beginning of the usuable data. NULL if it failed.
*/
void *realloc(void *ptr, size_t size) {
  struct _block *ret = malloc(size);
  memcpy(ret, ptr, size);
  free(ptr);
  return ptr;
}

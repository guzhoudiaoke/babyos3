
/*
 *	babyos/lib/malloc.cc
 *
 *  Copyright (C) <2020>  <Ruyi Liu>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */


/*
 *  2020-03-07		created
 */



#include "mm.h"
#include "string.h"
#include "unistd.h"
#include "stdio.h"


/* What is the correct alignment? */
#define ALIGNMENT 16


/* Basic constants and macros */ 
static const uint64 WSIZE = 8;
static const uint64 DSIZE = 16;
static const uint64 CHUNKSIZE = 1<<12;


/* Get max of two value */
static inline uint64 max(uint64 a, uint64 b)
{
    return a > b ? a : b;
}
  
/* Pack a size and allocated bit into a word */ 
static inline uint64 pack(uint64 size, bool prev_alloc, bool alloc)
{
    uint64 ret = size;
    if (alloc) {
        ret |= 1;
    }
    if (prev_alloc) {
        ret |= 2;
    }
    return ret;
}

/* Read a word at address p */
static inline uint64 get(void *p)
{
    return *((uint64 *)p);
}

/* write a word at address p */
static inline void put(void *p, uint64 val)
{
    *((uint64 *)p) = val;
}


/* Read the size fields from address p */ 
static inline uint64 get_size(void *p)
{
    return get(p) & (~0x7);
}

/* Read the allocated fields from address p */ 
static inline bool get_alloc(void *p)
{
    return get(p) & 0x1;
}


/* Read the prev allocated fields from address p */ 
static inline bool get_prev_alloc(void *p)
{
    return get(p) & 0x2;
}

/* Given block ptr bp, compute address of its header */ 
static inline void * hdrp(void *bp)
{
    return ((char *) (bp) - WSIZE);
}

/* Given block ptr bp, compute address of its footer */ 
static inline void * ftrp(void *bp)
{
    return ((char *)(bp) + get_size(hdrp(bp)) - DSIZE);
}

/* Given block ptr bp, compute address of next blocks */ 
static inline void * next_blkp(void *bp)
{
    return ((char *)(bp) + get_size(((char *)(bp) - WSIZE)));
}

/* Given block ptr bp, compute address of previous blocks */ 
static inline void * prev_blkp(void *bp)
{
    return ((char *)(bp) - get_size(((char *)(bp) - DSIZE)));
}

/* Given block ptr bp, compute address of previous free block */ 
static inline void *get_prev_link(void *bp)
{
    return (void *)(*(uint64 *)bp);
}

/* Given block ptr bp, compute address of next free block */ 
static inline void *get_next_link(void *bp)
{
    return (void *)(*(uint64 *)(((char *)bp + WSIZE)));
}

/* Given block ptr bp, set address of previous free block */ 
static inline void set_prev_link(void *bp, void *prev)
{
    *((uint64 *)bp) = (uint64)prev;}

/* Given block ptr bp, set address of next free block */ 
static inline void set_next_link(void *bp, void *next)
{
    *((uint64 *)((char *)bp + WSIZE)) = (uint64)next;
}


static void* heap_listp;
const int free_list_num=12;
static void* free_list_heads[12];
static void* free_list_tails[12];

/* Get index of free list by size */
static inline int get_free_list_head_id(uint64 asize)
{
    if (asize <= 32) {
        return 0;
    }
    if (asize <= 64) {
        return 1;
    }
    if (asize <= 128) {
        return 2;
    }
    if (asize <= 256) {
        return 3;
    }
    if (asize <= 512) {
        return 4;
    }
    if (asize <= 1024) {
        return 5;
    }
    if (asize <= 2048) {
        return 6;
    }
    if (asize <= 4096) {
        return 7;
    }
    if (asize <= 8192) {
        return 8;
    }
    if (asize <= 8192*2) {
        return 9;
    }
    if (asize <= 8192*4) {
        return 10;
    }
    return 11;
}

/* insert a block to tail of free list */
static void free_list_insert(void *p)
{
    /* get size of this block */
    uint64 size = get_size(hdrp(p));

    /* get free list index by size */
    int id = get_free_list_head_id(size);

    /* clear prev and next link of p */
    set_prev_link(p, NULL);
    set_next_link(p, NULL);

    /* add p to tail of list */
    if (free_list_tails[id] == NULL) {
        free_list_heads[id] = free_list_tails[id] = p;
    }
    else {
        set_prev_link(p, free_list_tails[id]);
        set_next_link(free_list_tails[id], p);
        free_list_tails[id] = p;
    }
}


#if 0
/* insert a block to head of free list */
static void free_list_insert3(void *p)
{
    /* get size of this block */
    uint64 size = get_size(hdrp(p));

    /* get free list index by size */
    int id = get_free_list_head_id(size);

    /* clear prev and next link of p */
    set_prev_link(p, NULL);
    set_next_link(p, NULL);

    /* add p to head of list */
    set_next_link(p, free_list_heads[id]);
    if (free_list_heads[id] != NULL) {
        set_prev_link(free_list_heads[id], p);
    }
    free_list_heads[id] = p;
}

/* insert a block into free list */
static void free_list_insert2(void *p)
{
    /* get size of this block */
    uint64 size = get_size(hdrp(p));

    /* get free list index by size */
    int id = get_free_list_head_id(size);

    /* clear prev and next link of p */
    set_prev_link(p, NULL);
    set_next_link(p, NULL);

    /* get free list head */
    void *pre = NULL;
    void *cur = free_list_heads[id];
    while (cur != NULL && size > get_size(hdrp(cur))) {
        pre = cur;
        cur = get_next_link(cur);
    }

    if (pre == NULL) {
        /* insert to head */
        if (free_list_heads[id] == NULL) {
            free_list_heads[id] = p;
        }
        else {
            /* add p to head of list */
            set_next_link(p, free_list_heads[id]);
            set_prev_link(free_list_heads[id], p);
            free_list_heads[id] = p;
        }
    }
    else {
        set_next_link(pre, p);
        set_prev_link(p, pre);
        if (cur != NULL) {
            set_prev_link(cur, p);
            set_next_link(p, cur);
        }
    }

}
#endif

/* remove a block from free list */
static void free_list_remove(void *p)
{
    /* get size of this block */
    uint64 size = get_size(hdrp(p));

    /* get free list index by size */
    int id = get_free_list_head_id(size);

    /* get free list head */
    if (free_list_heads[id] == p) {
        /* p is head of free list */
        free_list_heads[id] = get_next_link(free_list_heads[id]);
        if (free_list_heads[id] != NULL) {
            set_prev_link(free_list_heads[id], NULL);
        }
        else {
            free_list_tails[id] = NULL;
        }
    }
    else if (free_list_tails[id] == p) {
        free_list_tails[id] = get_prev_link(free_list_tails[id]);
        set_next_link(free_list_tails[id], NULL);
    }
    else {
        /* p is middle node of free list */
        void *prev = get_prev_link(p);
        void *next = get_next_link(p);
        if (prev != NULL) {
            set_next_link(prev, next);
        }
        if (next != NULL) {
            set_prev_link(next, prev);
        }
    }

    /* clear prev and next link of p */
    set_prev_link(p, NULL);
    set_next_link(p, NULL);
}

#if 0
/* remove a block from free list */
static void free_list_remove2(void *p)
{
    /* get size of this block */
    uint64 size = get_size(hdrp(p));

    /* get free list index by size */
    int id = get_free_list_head_id(size);

    /* get free list head */
    if (free_list_heads[id] == p) {
        /* p is head of free list */
        free_list_heads[id] = get_next_link(free_list_heads[id]);
        if (free_list_heads[id] != NULL) {
            set_prev_link(free_list_heads[id], NULL);
        }
    }
    else {
        /* p is middle node of free list */
        void *prev = get_prev_link(p);
        void *next = get_next_link(p);
        if (prev != NULL) {
            set_next_link(prev, next);
        }
        if (next != NULL) {
            set_prev_link(next, prev);
        }
    }

    /* clear prev and next link of p */
    set_prev_link(p, NULL);
    set_next_link(p, NULL);
}

/* rounds up to the nearest multiple of ALIGNMENT */
static uint64 align(uint64 x)
{
    return ALIGNMENT * ((x+ALIGNMENT-1)/ALIGNMENT);
}
#endif

/* merge a block to heap */
static void *coalesce(void *bp)
{
    /* get prev and next block of bp */
    //void *prev = prev_blkp(bp);
    void *prev = NULL;
    void *next = next_blkp(bp);

    /* get prev allocated and next allocated */
    //uint64 prev_alloc = get_alloc(ftrp(prev));
    uint64 prev_alloc = get_prev_alloc(hdrp(bp));
    uint64 next_alloc = get_alloc(hdrp(next));
    if (!prev_alloc) {
        prev = prev_blkp(bp);
    }

    /* get size of bp */
    uint64 size = get_size(hdrp(bp));

    if (prev_alloc && next_alloc) { 
        /* Case 1, both prev and next block have allocated */
        return bp;
    }
    else if (prev_alloc && !next_alloc) { 
        /* Case 2, prev block allocated, next block not allocated */

        /* remove next block from free list */
        free_list_remove(next);

        /* add size of next block to bp, merge bp and next block */
        size += get_size(hdrp(next_blkp(bp)));
        put(hdrp(bp), pack(size, 1, 0));
        put(ftrp(bp), pack(size, 1, 0));
    }
    else if (!prev_alloc && next_alloc) { 
        /* Case 3, prev block not allocated, next block allocated */

        /* remove prev block from free list */
        free_list_remove(prev);

        /* add size of prev block to bp, merge bp and prev block */
        size += get_size(hdrp(prev_blkp(bp)));
        put(ftrp(bp), pack(size, get_prev_alloc(hdrp(prev)), 0));
        put(hdrp(prev_blkp(bp)), pack(size, get_prev_alloc(hdrp(prev)), 0));
        bp = prev_blkp(bp);
    } 
    else { 
        /* Case 4, both prev block and next block not allocated */

        /* remove prev block and next block from free list */
        free_list_remove(prev);
        free_list_remove(next);

        /* add size of prev and next block to bp, merge these three block */
        size += get_size(hdrp(prev_blkp(bp))) + get_size(ftrp(next_blkp(bp))); 
        put(hdrp(prev_blkp(bp)), pack(size, get_prev_alloc(hdrp(prev)), 0)); 
        put(ftrp(next_blkp(bp)), pack(size, get_prev_alloc(hdrp(prev)), 0)); 
        bp = prev_blkp(bp); 
    }

    return bp; 
}

/*
 * extend the heap by words
 */
static void *extend_heap(uint64 words)
{
    char *bp;
    uint64 size;

    /* calc size to extend heap */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if ((long)(bp = (char *) sbrk(size)) == -1) {
        return NULL;
    }

    /* Initialize free block header/footer and the epilogue header */
    put(hdrp(bp), pack(size, get_prev_alloc(hdrp(bp)), 0)); /* Free block header */
    put(ftrp(bp), pack(size, get_prev_alloc(hdrp(bp)), 0)); /* Free block footer */
    put(hdrp(next_blkp(bp)), pack(0, 0, 1)); /* New epilogue header */

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}


/*
 * Initialize: returns false on error, true on success.
 */
bool malloc_init(void)
{
    /* Create the initial empty heap */
    if ((heap_listp = sbrk(4*WSIZE)) == (void *)-1){
        return false;
    }

    put(heap_listp, 0); /* Alignment padding */
    put((char *) heap_listp + (1*WSIZE), pack(DSIZE, 1, 1)); /* Prologue header */
    put((char *) heap_listp + (2*WSIZE), pack(DSIZE, 1, 1)); /* Prologue footer */
    put((char *) heap_listp + (3*WSIZE), pack(0, 1, 1)); /* Epilogue header */
    heap_listp = (char *)heap_listp + (2*WSIZE); 

    /* set all free list head and tail to NULL */
    for (int i = 0; i < free_list_num; i++){
        free_list_heads[i] = NULL;
        free_list_tails[i] = NULL;
    }

    /* Extend the empty heap with a free block of CHUMSIZE bytes */
    void * p = extend_heap(CHUNKSIZE/WSIZE);

    if (p == NULL) {
        return false; 
    }

    /* add p to free list */
    free_list_insert(p);

    return true;
}

/*
 * find a fit block from heap
 */
static void *find_fit(uint64 asize)
{
    int id = get_free_list_head_id(asize);

    for (; id < free_list_num; id++) {
        void *free_list_head = free_list_heads[id];
        void *p = free_list_head;
        while (p != NULL) {
            if (asize <= get_size(hdrp(p))) {
                return p;
            }
            p = get_next_link(p);
        }
    }
    return NULL;
}

/* try to place the left block */
static void place(void *bp, uint64 asize) 
{ 
    uint64 csize = get_size(hdrp(bp)); 
    if ((csize - asize) >= (2*DSIZE)) { 
        /* the left block have more than 2 DSIZE, add to heap */
        put(hdrp(bp), pack(asize, get_prev_alloc(hdrp(bp)), 1)); 
        put(ftrp(bp), pack(asize, get_prev_alloc(hdrp(bp)), 1)); 
        void *next = next_blkp(bp); 
        put(hdrp(next), pack(csize-asize, 1, 0)); 
        if (!get_alloc(hdrp(next))) {
            put(ftrp(next), pack(csize-asize, 1, 0)); 
        }

        /* add to free list */
        free_list_insert(next);
    }
    else { 
        /* have no enough space */
        put(hdrp(bp), pack(csize, get_prev_alloc(hdrp(bp)), 1)); 
        put(ftrp(bp), pack(csize, get_prev_alloc(hdrp(bp)), 1));

        void *next = next_blkp(bp); 
        put(hdrp(next), pack(get_size(hdrp(next)), 1, get_alloc(hdrp(next)))); 
        if (!get_alloc(hdrp(next))) {
            put(ftrp(next), pack(get_size(hdrp(next)), 1, get_alloc(hdrp(next)))); 
        }
    }
}


/* Given block ptr bp, compute address of previous free block */ 
static inline void *get_next_SMALL(void *bp)
{
    return (void *)(*(uint64 *)bp);
}

/* Given block ptr bp, set address of previous free block */ 
static inline void set_next_SMALL(void *bp, void *next)
{
    *((uint64 *)bp) = (uint64)next;
}

/*
 * malloc
 */
void* malloc(uint64 size)
{
    static bool inited = false;
    if (!inited) {
        malloc_init();
        inited = true;
    }

    /* IMPLEMENT THIS */
    uint64 asize; /* Adjusted block size */
    uint64 extendsize; /* Amount to extend heap if no fit */
    char *bp;
    /* Ignore spurious requests */
    if (size == 0) {
        return NULL;
    }

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= DSIZE) {
        asize = 2*DSIZE;
    }
    else {
        //asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);
        asize = DSIZE * ((size + (WSIZE) + (DSIZE-1)) / DSIZE);
    }

    /* Search the free list for a fit */
    if ((bp = (char *) find_fit(asize)) != NULL) {
        free_list_remove(bp);
        place(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = max(asize,CHUNKSIZE);
    if ((bp = (char *) extend_heap(extendsize/WSIZE)) == NULL) {
        return NULL;
    }

    place(bp, asize);

    return bp;
}



/*
 * free
 */
void free(void* ptr)
{
    /* get size of ptr */
    uint64 size = get_size(hdrp(ptr));

    /* set size and not allocated flag to header and footer */
    put(hdrp(ptr), pack(size, get_prev_alloc(hdrp(ptr)), 0));
    put(ftrp(ptr), pack(size, get_prev_alloc(hdrp(ptr)), 0));

    void *next = next_blkp(ptr); 
    put(hdrp(next), pack(get_size(hdrp(next)), 1, get_alloc(hdrp(next)))); 
    if (!get_alloc(hdrp(next))) {
        put(ftrp(next), pack(get_size(hdrp(next)), 1, get_alloc(hdrp(next)))); 
    }

    /* coalesce to heap */
    void * p = coalesce(ptr);
    free_list_insert(p);
}

/*
 * realloc
 */
void* realloc(void* oldptr, uint64 size)
{
    /* old ptr is NULL, just malloc a new block of memory */
    if (oldptr == NULL) {
        return malloc(size);
    }

    /* old size is 0, just free old ptr */
    if (size == 0) {
        free(oldptr);
        return NULL;
    }

    uint64 oldsize = get_size(hdrp(oldptr));
    size = (size+2*DSIZE-1)/DSIZE*DSIZE;
    if (oldsize >= size) {
        return oldptr;
    }


    /* malloc a new block of memory */
    void *newptr = malloc(size);
    if (newptr == NULL) {
        return NULL;
    }

    /* if size of old ptr bigger than new size, just copy size of new size */
    if (size < oldsize) {
        oldsize = size;
    }

    /* copy data from old ptr to new ptr */
    memcpy(newptr, oldptr, oldsize);

    /* free old ptr */
    free(oldptr);

    /* return new ptr */
    return newptr;
}

/*
 * calloc
 * This function is not tested by mdriver, and has been implemented for you.
 */
void* calloc(uint64 nmemb, uint64 size)
{
    void* ptr;
    size *= nmemb;
    ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

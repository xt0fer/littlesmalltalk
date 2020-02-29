/*
    memory management for the Little Smalltalk system
    Uses a variation on the Baker two-space algorithm

    Written by Tim Budd, Oregon State University, 1994
    Comments to budd@cs.orst.edu
    All rights reserved, no guarantees given whatsoever.
    May be freely redistributed if not for profit.
*/

/*
    The fundamental data type is the object.
    The first field in an object is a size, the low order two
        bits being used to maintain:
            * binary flag, used if data is binary
            * indirection flag, used if object has been relocated
    The next data field is the class
    The following fields are either objects, or character values


    KRH - the following is no longer correct.

    The initial image is loaded into static memory space --
    space which is never garbage collected
    This improves speed, as these items are not moved during GC.

    Now we use a simple two-space algorithm without a static space.
    This is not as efficient for GC as the minimal generational
    implementation used before, but is a lot nicer for image writing.
*/

#pragma once

#include <sys/types.h>
#include <stdio.h>
#include <stdint.h>

/* ints must be at least 32-bit in size! */

struct object {
    uintptr_t header;
    struct object *class;
    struct object *data[];
};

/*
    byte objects are used to represent strings and symbols
        bytes per word must be correct
*/

struct byteObject {
    uintptr_t header;
    struct object *class;
    uint8_t bytes[];
};

/*
 * Used for GC.  Exposed here to keep all the
 * object definitions in one place.
 */

struct mobject {
    uintptr_t header;
    struct mobject *data[];
};


#define BytesPerWord ((int)(sizeof (intptr_t)))
#define bytePtr(x) (((struct byteObject *) x)->bytes)
#define WORDSUP(ptr, amt) ((struct object *)(((char *)(ptr)) + ((amt) * BytesPerWord)))
#define WORDSDOWN(ptr, amt) WORDSUP(ptr, 0 - (amt))

#define TO_WORDS(isz) (((isz) + BytesPerWord - 1)/BytesPerWord)

/*
 * SmallInt objects are used to represent short integers.  They are
 * encoded as 31 bits, signed, with the low bit set to 1.  This
 * distinguishes them from all other objects, which are longword
 * aligned and are proper C memory pointers.
 */
#include <limits.h>

#define IS_SMALLINT(x) ((((intptr_t)(x)) & 0x01) != 0)
#define FITS_SMALLINT(x) ((((intptr_t)(x)) >= INT_MIN/2) && \
                          (((intptr_t)(x)) <= INT_MAX/2))
#define CLASS(x) (IS_SMALLINT(x) ? SmallIntClass : ((x)->class))
#define integerValue(x) ((int)(((intptr_t)(x)) >> 1))
#define newInteger(x) ((struct object *)((((intptr_t)(x)) * 2) | 0x01))

/*
 * The "size" field is the top 30 bits; the bottom two are flags
 */
#define SIZE(op) ((uint32_t)(((struct object *)(op))->header) >> 2)
#define SET_SIZE(op, val) (((struct object *)(op))->header = (uintptr_t)((uint32_t)(val) << 2))

/* handle the other flags in the header. */
#define FLAG_GCDONE (0x01)
#define IS_GCDONE(o) (((struct object *)(o))->header & (uintptr_t)FLAG_GCDONE)
#define SET_GCDONE(o) (((struct object *)(o))->header |= (uintptr_t)FLAG_GCDONE)

#define FLAG_BIN (0x02)
#define IS_BINOBJ(o) (((struct object *)(o))->header & (uintptr_t)FLAG_BIN)
#define SET_BINOBJ(o) (((struct object *)(o))->header |= (uintptr_t)FLAG_BIN)

#define NOT_NIL(o) ((o) && ((o) != nilObject))

/*
    memoryBase holds the pointer to the current space,
    memoryPointer is the pointer into this space.
    To allocate, decrement memoryPointer by the correct amount.
    If the result is less than memoryBase, then garbage collection
    must take place
*/

extern struct object *memoryPointer, *memoryBase;

/*
    roots for the memory space
    these are traced down during memory management
    rootStack is the dynamic stack
    staticRoots are values in static memory that point to
    dynamic values
*/
# define ROOTSTACKLIMIT 2000
extern struct object *rootStack[];
extern int rootTop;
extern void addStaticRoot(struct object **);

/* image reading/writing */
extern int fileIn(FILE *fp);
extern int fileOut(FILE *fp);

/*
    entry points
*/

extern void gcinit(int, int);
extern struct object *gcollect(int);
extern struct object *staticAllocate(int);
extern struct object *staticIAllocate(int);
extern struct object *gcialloc(int);
extern void exchangeObjects(struct object *, struct object *, int size);
extern int symstrcomp(struct object *left, const char *right);
extern int strsymcomp(const char *left, struct object *right);
extern int isDynamicMemory(struct object *);
extern int fileOut(FILE *fp);

#define gcalloc(sz) (((intptr_t)(memoryPointer = WORDSDOWN(memoryPointer, (sz) + 2)) < \
                      (intptr_t)memoryBase) ? gcollect(sz) : \
                     (SET_SIZE(memoryPointer, (sz)), memoryPointer))

#ifndef gcalloc
extern struct object *gcalloc(int);
#endif


/* these define the flags used for writing and reading images.
The bytes per word or size is usually stored in the lower bits */
# define LST_ERROR_TYPE     (0)

/* normal objects */
# define LST_OBJ_TYPE       (1<<5)

/* positive small integers */
# define LST_PINT_TYPE      (2<<5)

/* negative small integers */
# define LST_NINT_TYPE      (3<<5)

/* byte arrays */
# define LST_BARRAY_TYPE    (4<<5)

/* previously dumped object */
# define LST_POBJ_TYPE      (5<<5)

/* nil object */
# define LST_NIL_TYPE       (6<<5)

# define LST_SMALL_TAG_LIMIT    0x0F
# define LST_LARGE_TAG_FLAG 0x10
# define LST_TAG_SIZE_MASK  0x1F
# define LST_TAG_VALUE_MASK     LST_SMALL_TAG_LIMIT
# define LST_TAG_TYPE_MASK  0xE0


/* image information */
struct image_header {
    char magic[4];
    uint32_t version;
} __attribute__((packed));

#define IMAGE_VERSION_0 (0)
#define IMAGE_VERSION_1 (1)
#define IMAGE_VERSION_2 (2)

extern int64_t gc_count;
extern int64_t gc_total_time;
extern int64_t gc_max_time;
extern int64_t gc_total_mem_copied;
extern int64_t gc_mem_max_copied;

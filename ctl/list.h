/* List containers are implemented as doubly-linked lists */

#ifndef T
#error "Template type T undefined for <ctl/list.h>"
#endif

#include <ctl/ctl.h>
#include <stdarg.h>

#define A JOIN(list, T)
#define B JOIN(A, node)
#define I JOIN(A, it)

typedef struct B
{
    struct B* next;
    struct B* prev;
    T value;
} B;

typedef struct A
{
    void (*free)(T*);
    T (*copy)(T*);
    B* head;
    B* tail;
    size_t size;
} A;

typedef struct I
{
    CTL_COMMONFIELDS_ITER;
    A* container;
    B* begin;
} I;

static inline T*
JOIN(A, front)(A* self)
{
    return &self->head->value;
}

static inline T*
JOIN(A, back)(A* self)
{
    return &self->tail->value;
}

static inline B*
JOIN(A, begin)(A* self)
{
    return self->head;
}

static inline B*
JOIN(A, end)(A* self)
{
    (void) self;
    return NULL;
}

static inline void
JOIN(I, step)(I* self)
{
    if(self->next == self->end)
        self->done = 1;
    else
    {
        self->node = self->next;
        self->ref = &self->node->value;
        self->next = self->node->next;
    }
}

static inline I
JOIN(I, range)(A* container, B* begin, B* end)
{
    (void) container;
    static I zero;
    I self = zero;
    if(begin)
    {
        self.step = JOIN(I, step);
        self.begin = begin;
        self.end = end;
        self.next = begin->next;
        self.node = begin;
        self.ref = &begin->value;
        self.container = container;
    }
    else
        self.done = 1;
    return self;
}

#include <ctl/_share.h>

static inline A
JOIN(A, init)(void)
{
    static A zero;
    A self = zero;
#ifdef POD
#undef POD
    self.copy = JOIN(A, implicit_copy);
#else
    self.free = JOIN(T, free);
    self.copy = JOIN(T, copy);
#endif
    return self;
}

static inline B*
JOIN(B, init)(T value)
{
    B* self = (B*) malloc(sizeof(B));
    self->prev = self->next = NULL;
    self->value = value;
    return self;
}

static inline void
JOIN(A, disconnect)(A* self, B* node)
{
#if defined(_ASSERT_H) && !defined(NDEBUG)
    assert (self->size);
#endif
    if(node == self->tail) self->tail = self->tail->prev;
    if(node == self->head) self->head = self->head->next;
    if(node->prev) node->prev->next = node->next;
    if(node->next) node->next->prev = node->prev;
    node->prev = node->next = NULL;
    self->size--;
}

static inline void
JOIN(A, connect)(A* self, B* position, B* node, int before)
{
    if(JOIN(A, empty)(self))
    {
        self->head = self->tail = node;
        self->size++;
    }
    else
    if (self->size < JOIN(A, max_size)())
    {
        if(before)
        {
            node->next = position;
            node->prev = position->prev;
            if(position->prev)
                position->prev->next = node;
            position->prev = node;
            if(position == self->head)
                self->head = node;
        }
        else
        {
            node->prev = position;
            node->next = position->next;
            if(position->next)
                position->next->prev = node;
            position->next = node;
            if(position == self->tail)
                self->tail = node;
        }
        self->size++;
    }
    /* error handling? silent ignore or stderr or assert or customizable.
    else
        assert (0 || "list size exceeded");
        fprintf (stderr, "list size exceeded");
    */
}

static inline void
JOIN(A, push_back)(A* self, T value)
{
    B* node = JOIN(B, init)(value);
    JOIN(A, connect)(self, self->tail, node, 0);
}

static inline void
JOIN(A, push_front)(A* self, T value)
{
    B* node = JOIN(B, init)(value);
    JOIN(A, connect)(self, self->head, node, 1);
}

static inline void
JOIN(A, transfer)(A* self, A* other, B* position, B* node, int before)
{
    JOIN(A, disconnect)(other, node);
    JOIN(A, connect)(self, position, node, before);
}

// FIXME I* position
static inline void
JOIN(A, erase)(A* self, B* node)
{
    JOIN(A, disconnect)(self, node);
    if(self->free)
        self->free(&node->value);
    free(node);
}

static inline void
JOIN(A, pop_back)(A* self)
{
    JOIN(A, erase)(self, self->tail);
}

static inline void
JOIN(A, pop_front)(A* self)
{
    JOIN(A, erase)(self, self->head);
}

// FIXME I* position
static inline void
JOIN(A, insert)(A* self, B* position, T value)
{
    B* node = JOIN(B, init)(value);
    JOIN(A, connect)(self, position, node, 1);
}

static inline void
JOIN(A, clear)(A* self)
{
    while(!JOIN(A, empty)(self))
        JOIN(A, pop_back)(self);
}

static inline void
JOIN(A, free)(A* self)
{
    JOIN(A, clear)(self);
    *self = JOIN(A, init)();
}

static inline void
JOIN(A, resize)(A* self, size_t size, T value)
{
    // TODO max_size?
    if(size != self->size)
        for(size_t i = 0; size != self->size; i++)
            (size < self->size)
                ? JOIN(A, pop_back)(self)
                : JOIN(A, push_back)(self, self->copy(&value));
    if(self->free)
        self->free(&value);
}

static inline A
JOIN(A, copy)(A* self)
{
    A other = JOIN(A, init)();
    for(B* node = self->head; node; node = node->next)
        JOIN(A, push_back)(&other, self->copy(&node->value));
    return other;
}

static inline void
JOIN(A, assign)(A* self, size_t size, T value)
{
    JOIN(A, resize)(self, size, self->copy(&value));
    size_t i = 0;
    foreach(A, self, it)
    {
        if(self->free)
            self->free(it.ref);
        *it.ref = self->copy(&value);
        i++;
    }
    if(self->free)
        self->free(&value);
}

static inline void
JOIN(A, reverse)(A* self)
{
    foreach(A, self, it)
    {
        B* next = it.node->next;
        B* prev = it.node->prev;
        it.node->prev = next;
        it.node->next = prev;
    }
    B* tail = self->tail;
    B* head = self->head;
    self->tail = head;
    self->head = tail;
}

#ifdef DEBUG

static inline I
JOIN(I, iter)(A* self, B *node)
{
    I it = JOIN(I, each)(self);
    it.begin = node;
    it.node = node;
    it.ref = &node->value;
    it.next = node->next;
    return it;
}

static inline size_t
JOIN(A, remove)(A* self, T value){
    size_t erases = 0;
    foreach(A, self, it)
        if(memcmp(it.ref, &value, sizeof(T)) == 0)
        {
            JOIN(A, erase)(self, it.node);
            erases += 1;
        }
    return erases;
}

static inline I
JOIN(A, emplace)(A* self, I* pos, int numvalues, ...) {
    va_list sp;
    va_start(sp, numvalues);
    for (int i=0; i < numvalues; i++)
    {
        T value = va_arg(sp, T);
        B* node = JOIN(B, init)(value);
        JOIN(A, connect)(self, pos->node, node, 1);
    }
    va_end(sp);
    return JOIN(I, iter)(self, pos->next);
}

static inline I*
JOIN(A, insert_count)(A* self, I* pos, size_t count, T value)
{
    B* node = JOIN(B, init)(value);
    for (size_t i=0; i < count; i++)
        JOIN(A, connect)(self, pos->node, node, 1);
    if (count)
        pos->step(pos);
    return pos;
}

static inline I*
JOIN(A, insert_range)(A* self, I* pos, I* first, I* last)
{
    A* other = first->container;
    foreach(A, other, it)
    {
        B* node = JOIN(B, init)(*it.ref);
        JOIN(A, connect)(self, it.node, node, 1);
    }
    if (first != last)
        pos->step(pos);
    return pos;
}

#endif

static inline size_t
JOIN(A, remove_if)(A* self, int _equal(T*))
{
    size_t erases = 0;
    foreach(A, self, it)
        if(_equal(it.ref))
        {
            JOIN(A, erase)(self, it.node);
            erases++;
        }
    return erases;
}

// FIXME I* position
static inline void
JOIN(A, splice)(A* self, B* pos, A* other)
{
    if(self->size == 0 && pos == NULL)
        JOIN(A, swap)(self, other);
    else
        foreach(A, other, it)
            JOIN(A, transfer)(self, other, pos, it.node, 1);
}

#if 0
static inline void
JOIN(A, splice)(A* self, I* pos, A* other) {}

static inline void
JOIN(A, splice_it)(A* self, I* pos, A* other, I* other_pos) {}

static inline void
JOIN(A, splice_range)(A* self, I* pos, A* other, I* other_first, I* other_last) {}
#endif

static inline void
JOIN(A, merge)(A* self, A* other, int _compare(T*, T*))
{
    if(JOIN(A, empty)(self))
        JOIN(A, swap)(self, other);
    else
    {
        for(B* node = self->head; node; node = node->next)
            while(!JOIN(A, empty)(other) && _compare(&node->value, &other->head->value))
                JOIN(A, transfer)(self, other, node, other->head, 1);
        // Remainder.
        while(!JOIN(A, empty)(other))
            JOIN(A, transfer)(self, other, self->tail, other->head, 0);
    }
}

static inline void
JOIN(A, sort)(A* self, int _compare(T*, T*))
{
    if(self->size > 1)
    {
        A carry = JOIN(A, init)();
        A temp[64];
        for(size_t i = 0; i < len(temp); i++)
            temp[i] = JOIN(A, init)();
        A* fill = temp;
        A* counter = NULL;
        do
        {
            JOIN(A, transfer)(&carry, self, carry.head, self->head, 1);
            for(counter = temp; counter != fill && !JOIN(A, empty)(counter); counter++)
            {
                JOIN(A, merge)(counter, &carry, _compare);
                JOIN(A, swap)(&carry, counter);
            }
            JOIN(A, swap)(&carry, counter);
            if(counter == fill)
                fill++;
        }
        while(!JOIN(A, empty)(self));
        for(counter = temp + 1; counter != fill; counter++)
            JOIN(A, merge)(counter, counter - 1, _compare);
        JOIN(A, swap)(self, fill - 1);
    }
}

static inline void
JOIN(A, unique)(A* self, int _equal(T*, T*))
{
    foreach(A, self, it)
        if(it.next && _equal(it.ref, &it.next->value))
            JOIN(A, erase)(self, it.node);
}

static inline B*
JOIN(A, find)(A* self, T key, int _equal(T*, T*))
{
    foreach(A, self, it)
        if(_equal(it.ref, &key))
            return it.node;
    return NULL;
}

#undef T
#undef A
#undef B
#undef I

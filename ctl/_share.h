static inline int
JOIN(A, empty)(A* self)
{
    return self->size == 0;
}

static inline size_t
JOIN(A, size)(A* self)
{
    return self->size;
}

static inline size_t
JOIN(A, max_size)()
{
    return 4294967296 / sizeof(T); // 32bit at most. avoid DDOS
}

static inline I
JOIN(I, each)(A* a)
{
    return JOIN(A, empty)(a)
         ? JOIN(I, range)(a, NULL, NULL)
         : JOIN(I, range)(a, JOIN(A, begin)(a), JOIN(A, end)(a));
}

static inline T
JOIN(A, implicit_copy)(T* self)
{
    return *self;
}

static inline int
JOIN(A, equal)(A* self, A* other, int _equal(T*, T*))
{
    if(self->size != other->size)
        return 0;
    I a = JOIN(I, each)(self);
    I b = JOIN(I, each)(other);
    while(!a.done && !b.done)
    {
        if(!_equal(a.ref, b.ref))
            return 0;
        a.step(&a);
        b.step(&b);
    }
    return 1;
}

static inline void
JOIN(A, swap)(A* self, A* other)
{
    A temp = *self;
    *self = *other;
    *other = temp;
}

#include "../test.h"
#include "digi.hh"

#define T digi
#include <ctl/queue.h>

#include <queue>
#include <algorithm>

#define CHECK(_x, _y) {                                           \
    while(_x.size > 0) {                                          \
        assert(_x.size == _y.size());                             \
        assert(queue_digi_empty(&_x) == _y.empty());                \
        assert(*_y.front().value == *queue_digi_front(&_x)->value); \
        assert(*_y.back().value == *queue_digi_back(&_x)->value);   \
        _y.pop();                                                 \
        queue_digi_pop(&_x);                                        \
    }                                                             \
}

int
main(void)
{
    INIT_SRAND;
    const size_t loops = TEST_RAND(TEST_MAX_LOOPS);
    for(size_t loop = 0; loop < loops; loop++)
    {
        size_t size = TEST_RAND(TEST_MAX_SIZE);
        queue_digi a = queue_digi_init();
        std::queue<DIGI> b;
        for(size_t pushes = 0; pushes < size; pushes++)
        {
            const int value = TEST_RAND(INT_MAX);
            queue_digi_push(&a, digi_init(value));
            b.push(DIGI{value});
        }
        enum
        {
            TEST_PUSH,
            TEST_POP,
            TEST_SWAP,
            TEST_TOTAL,
        };
        int which = TEST_RAND(TEST_TOTAL);
        switch(which)
        {
            case TEST_PUSH:
            {
                const int value = TEST_RAND(INT_MAX);
                b.push(DIGI{value});
                queue_digi_push(&a, digi_init(value));
                CHECK(a, b);
                break;
            }
            case TEST_POP:
            {
                if(a.size > 0)
                {
                    b.pop();
                    queue_digi_pop(&a);
                    CHECK(a, b);
                }
                break;
            }
            case TEST_SWAP:
            {
                queue_digi aa = queue_digi_copy(&a);
                queue_digi aaa = queue_digi_init();
                std::queue<DIGI> bb = b;
                std::queue<DIGI> bbb;
                queue_digi_swap(&aaa, &aa);
                std::swap(bb, bbb);
                CHECK(aaa, bbb);
                queue_digi_free(&aaa);
                CHECK(a, b);
                break;
            }
        }
        CHECK(a, b);
        queue_digi_free(&a);
    }
    TEST_PASS(__FILE__);
}

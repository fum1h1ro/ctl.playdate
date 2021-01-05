#include "../test.h"
#include "strint.hh"

#define USE_INTERNAL_VERIFY
#define T strint
#include <ctl/map.h>

#include <map>
#include <algorithm>

#define CHECK(_x, _y) {                                      \
    assert(_x.size == _y.size());                            \
    std::map<std::string,int>::iterator _iter = _y.begin();  \
    foreach(map_strint, &_x, _it) {                          \
        assert(_it.ref->value == _iter->second);             \
        _iter++;                                             \
    }                                                        \
    map_strint_it _it = map_strint_it_each(&_x);             \
    for(auto& _d : _y) {                                     \
        assert(_it.ref->value == _d.second);                 \
        _it.step(&_it);                                      \
    }                                                        \
}

static char *
new_rand_str() {
    char *c_char = (char *)calloc(36, 1);
    snprintf(c_char, 36, "%c%d", 48 + (rand() % 74), rand());
    return c_char;
}

static void
setup_sets(map_strint* a, std::map<std::string,int>& b)
{
    size_t iters = TEST_RAND(TEST_MAX_SIZE);
    *a = map_strint_init(strint_compare);
    for(size_t inserts = 0; inserts < iters; inserts++)
    {
        char *key = new_rand_str();
        const int vb = TEST_RAND(TEST_MAX_SIZE);
        map_strint_insert(a, strint_init(str_init(key),vb));
        b.insert(STRINT{key,vb}); // or std::pair<std::string,int>(key,vb));
    }
}

int
main(void)
{
    INIT_SRAND;
    const size_t loops = TEST_RAND(TEST_MAX_LOOPS);
    for(size_t loop = 0; loop < loops; loop++)
    {
        map_strint a;
        std::map<std::string,int> b;
        setup_sets(&a, b);
        enum
        {
            TEST_INSERT,
            TEST_INSERT_OR_ASSIGN,
            //TEST_EMPLACE,
            //TEST_EXTRACT,
            //TEST_MERGE,
            //TEST_CONTAINS,
            //TEST_ERASE_IF,
            //TEST_EQUAL_RANGE,
            TEST_ERASE,
            TEST_CLEAR,
            TEST_SWAP,
            TEST_COUNT,
            TEST_FIND,
            TEST_COPY,
            TEST_EQUAL,
            TEST_UNION,
            TEST_INTERSECTION,
            TEST_SYMMETRIC_DIFFERENCE,
            TEST_DIFFERENCE,
            TEST_TOTAL,
        };
        int which = TEST_RAND(TEST_TOTAL);
        switch(which)
        {
            case TEST_INSERT:
            {
                char *key = new_rand_str();
                const int vb = TEST_RAND(TEST_MAX_SIZE);
                map_strint_insert(&a, strint_init(str_init(key),vb));
                b.insert(STRINT{key,vb});
                CHECK(a, b);
                break;
            }
            case TEST_INSERT_OR_ASSIGN:
            {
                char *key = new_rand_str();
                const int vb = TEST_RAND(TEST_MAX_SIZE);
                auto p = STRINT{key,vb};
                map_strint_insert_or_assign(&a, strint_init(str_init(key),vb));
                b.insert_or_assign(key, std::move(p));
                CHECK(a, b);
                break;
            }
            case TEST_ERASE:
            {
                const size_t erases = TEST_RAND(TEST_MAX_SIZE) / 4;
                for(size_t i = 0; i < erases; i++)
                    if(a.size > 0)
                    {
                        char *key = new_rand_str();
                        const int value = TEST_RAND(TEST_MAX_SIZE);
                        strint kd = strint_init(str_init(key),value);
                        map_strint_erase(&a, kd);
                        b.erase(key);
                        CHECK(a, b);
                        strint_free(&kd);
                    }
                CHECK(a, b);
                break;
            }
            case TEST_SWAP:
            {
                map_strint aa = map_strint_copy(&a);
                map_strint aaa = map_strint_init(strint_compare);
                std::map<std::string,int> bb = b;
                std::map<std::string,int> bbb;
                map_strint_swap(&aaa, &aa);
                std::swap(bb, bbb);
                CHECK(aaa, bbb);
                map_strint_free(&aaa);
                CHECK(a, b);
                break;
            }
            case TEST_COUNT:
            {
                char *key = new_rand_str();
                int value = TEST_RAND(TEST_MAX_SIZE);
                strint kd = strint_init(str_init(key), value);
                int aa = map_strint_count(&a, kd);
                int bb = b.count(key);
                assert(aa == bb);
                CHECK(a, b);
                strint_free(&kd);
                break;
            }
            case TEST_FIND:
            {
                char *key = new_rand_str();
                const int value = TEST_RAND(TEST_MAX_SIZE);
                strint kd = strint_init(str_init(key), value);
                map_strint_node* aa = map_strint_find(&a, kd);
                auto bb = b.find(key);
                if(bb == b.end())
                    assert(map_strint_end(&a) == aa);
                else
                    assert(bb->second == aa->key.value);
                CHECK(a, b);
                strint_free(&kd);
                break;
            }
            case TEST_CLEAR:
            {
                b.clear();
                map_strint_clear(&a);
                CHECK(a, b);
                break;
            }
#if 0
            case TEST_CONTAINS: // C++20.
            {
                char *key = new_rand_str();
                const int value = TEST_RAND(TEST_MAX_SIZE);
                strint kd = strint_init(str_init(key), value);
                int aa = map_strint_contains(&a, kd);
                int bb = b.contains(STRINT{key,value});
                assert(aa == bb);
                CHECK(a, b);
                break;
            }
            case TEST_EMPLACE:
            case TEST_EXTRACT:
            case TEST_MERGE:
            case TEST_CONTAINS:
            case TEST_ERASE_IF:
            {
                size_t a_erases = map_strint_erase_if(&a, strint_is_odd);
                size_t b_erases = b.erase_if(STRINT_is_odd);
                assert(a_erases == b_erases);
                CHECK(a, b);
                break;
            }
            case TEST_EQUAL_RANGE:
                break;
#endif
            case TEST_COPY:
              { // C++20
                map_strint aa = map_strint_copy(&a);
                std::map<std::string,int> bb = b;
                CHECK(aa, bb);
                map_strint_free(&aa);
                CHECK(a, b);
                break;
            }
            case TEST_EQUAL:
            {
                map_strint aa = map_strint_copy(&a);
                std::map<std::string,int> bb = b;
                assert(map_strint_equal(&a, &aa, strint_match));
                assert(b == bb);
                map_strint_free(&aa);
                CHECK(a, b);
                break;
            }
            case TEST_UNION:
            {
                map_strint aa;
                std::map<std::string,int> bb;
                setup_sets(&aa, bb);
                map_strint aaa = map_strint_union(&a, &aa);
                std::map<std::string,int> bbb;
                std::set_union(b.begin(), b.end(), bb.begin(), bb.end(),
                               std::inserter(bbb, bbb.begin()));
                CHECK(a, b);
                CHECK(aa, bb);
                CHECK(aaa, bbb);
                map_strint_free(&aa);
                map_strint_free(&aaa);
                break;
            }
            case TEST_INTERSECTION:
            {
                map_strint aa;
                std::map<std::string,int> bb;
                setup_sets(&aa, bb);
                map_strint aaa = map_strint_intersection(&a, &aa);
                std::map<std::string,int> bbb;
                std::set_intersection(b.begin(), b.end(), bb.begin(), bb.end(),
                                      std::inserter(bbb, bbb.begin()));
                CHECK(a, b);
                CHECK(aa, bb);
                CHECK(aaa, bbb);
                map_strint_free(&aa);
                map_strint_free(&aaa);
                break;
            }
            case TEST_SYMMETRIC_DIFFERENCE:
            {
                map_strint aa;
                std::map<std::string,int> bb;
                setup_sets(&aa, bb);
                map_strint aaa = map_strint_symmetric_difference(&a, &aa);
                std::map<std::string,int> bbb;
                std::set_symmetric_difference(b.begin(), b.end(), bb.begin(), bb.end(),
                                              std::inserter(bbb, bbb.begin()));
                CHECK(a, b);
                CHECK(aa, bb);
                CHECK(aaa, bbb);
                map_strint_free(&aa);
                map_strint_free(&aaa);
                break;
            }
            case TEST_DIFFERENCE:
            {
                map_strint aa;
                std::map<std::string,int> bb;
                setup_sets(&aa, bb);
                map_strint aaa = map_strint_difference(&a, &aa);
                std::map<std::string,int> bbb;
                std::set_difference(b.begin(), b.end(), bb.begin(), bb.end(),
                                    std::inserter(bbb, bbb.begin()));
                CHECK(a, b);
                CHECK(aa, bb);
                CHECK(aaa, bbb);
                map_strint_free(&aa);
                map_strint_free(&aaa);
                break;
            }
        }
        CHECK(a, b);
        map_strint_free(&a);
    }
    TEST_PASS(__FILE__);
}
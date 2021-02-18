#include "../test.h"

/* generate lists out of various containers */
// pick one to be the target container
// pick another to be the 2nd range

#define POD
#define T int
#include <ctl/list.h>
#define POD
#define T int
#define N 25
#include <ctl/array.h>
#define POD
#define T int
#include <ctl/deque.h>
#define POD
#define T int
#include <ctl/set.h>
#define POD
#define T int
#include <ctl/vector.h>
#define POD
#define T int
#include <ctl/unordered_set.h>

typedef enum types_t
{
    CTL_VECTOR,
    CTL_ARRAY,
    CTL_DEQUE,
    CTL_LIST,
    CTL_SET,
    CTL_USET /* 5 */
} types_t;
static const types_t types[] = {CTL_VECTOR, CTL_ARRAY, CTL_DEQUE, CTL_LIST, CTL_SET, CTL_USET};

#include <vector>
#include <array>
#include <deque>
#include <list>
#include <set>
#include <unordered_set>
#include <algorithm>

template <size_t N> using arrint = std::array<int,N>;

types_t pick_type(void) {
    const int LEN = len(types);
    return types[TEST_RAND(LEN)];
}

void print_vec(vec_int *a)
{
    printf("vec: ");
    foreach (vec_int, a, it) printf("%d, ", *it.ref);
    printf("\n");
}
void print_arr25(arr25_int *a)
{
    printf("arr25: ");
    foreach (arr25_int, a, it) printf("%d, ", it.ref ? *it.ref : -1);
    printf("\n");
}
void print_deq(deq_int *a)
{
    printf("deq: ");
    foreach (deq_int, a, it) printf("%d, ", *it.ref);
    printf("\n");
}
void print_list(list_int *a)
{
    printf("list: ");
    list_foreach_ref(list_int, a, it) printf("%d, ", *it.ref);
    printf("\n");
}
void print_set(set_int *a)
{
    printf("set: ");
    foreach (set_int, a, it) printf("%d, ", *it.ref);
    printf("\n");
}
void print_uset(uset_int *a)
{
    printf("uset: ");
    foreach (uset_int, a, it) printf("%d, ", *it.ref);
    printf("\n");
}

void print_list_stl(std::list<int> &b)
{
    for (auto &d : b)
        printf("%d, ", d);
    printf("\n----\n");
}

#ifdef DEBUG
#define TEST_MAX_VALUE 15
#ifndef LONG
#undef TEST_MAX_SIZE
#define TEST_MAX_SIZE 10
#endif
#else
#define print_vec(x)
#define print_arr25(x)
#define print_deq(x)
#define print_list(x)
#define print_set(x)
#define print_uset(x)
#define TEST_MAX_VALUE 1500
#endif

// from or to uset is unordered, all C++ set algorithms on uset are considered broken.
#define CHECK(ty1, ty2, cppty, _x, _y)                                                                                 \
    {                                                                                                                  \
        if (strcmp(#ty1, "uset") && strcmp(#ty2, "uset"))                                                              \
        {                                                                                                              \
            if (_x.size != _y.size())                                                                                  \
                printf("CTL size %zu != STL %zu\n", _x.size, _y.size());                                               \
            assert(_x.size == _y.size());                                                                              \
            assert(ty1##_int_empty(&_x) == _y.empty());                                                                \
            ty1##_int_it _it1 = ty1##_int_begin(&_x);                                                                  \
            int i = 0;                                                                                                 \
            LOG("=> %s: ", #cppty);                                                                                    \
            for (auto &_d : _y)                                                                                        \
            {                                                                                                          \
                LOG("%d, ", _d);                                                                                       \
                i++;                                                                                                   \
                if (*_it1.ref != _d)                                                                                   \
                    printf("[%d]: CTL %d != STL %d\n", i, *_it1.ref, _d);                                              \
                /*assert(*_it1.ref == _d);*/                                                                           \
                ty1##_int_it_next(&_it1);                                                                              \
            }                                                                                                          \
            LOG("\n");                                                                                                 \
            std::cppty::iterator _iter = _y.begin();                                                                   \
            foreach (ty1##_int, &_x, _it2)                                                                             \
            {                                                                                                          \
                /*assert(*_it2.ref == *_iter);*/                                                                       \
                _iter++;                                                                                               \
            }                                                                                                          \
        }                                                                                                              \
    }

#define SETUP_LIST1                                                                                                    \
    list_int a = list_int_init();                                                                                      \
    std::list<int> b;                                                                                                  \
    for (int i = 0; i < TEST_RAND(TEST_MAX_SIZE); i++)                                                                 \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        list_int_push_back(&a, vb);                                                                                    \
        b.push_back(vb);                                                                                               \
    }                                                                                                                  \
    print_list(&a)

#define SETUP_LIST2                                                                                                    \
    list_int aa = list_int_init();                                                                                     \
    std::list<int> bb;                                                                                                 \
    for (int i = 0; i < TEST_RAND(25); i++)                                                                            \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        list_int_push_back(&aa, vb);                                                                                   \
        bb.push_back(vb);                                                                                              \
    }                                                                                                                  \
    list_int_it range2 = list_int_begin(&aa);                                                                          \
    print_list(&aa)

#define SETUP_VEC1                                                                                                     \
    vec_int a = vec_int_init();                                                                                        \
    std::vector<int> b;                                                                                                \
    for (int i = 0; i < TEST_RAND(TEST_MAX_SIZE); i++)                                                                 \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        vec_int_push_back(&a, vb);                                                                                     \
        b.push_back(vb);                                                                                               \
    }                                                                                                                  \
    vec_int_sort(&a);                                                                                                  \
    std::sort(b.begin(), b.end());                                                                                     \
    print_vec(&a)
#define SETUP_VEC2                                                                                                     \
    vec_int aa = vec_int_init();                                                                                       \
    std::vector<int> bb;                                                                                               \
    for (int i = 0; i < TEST_RAND(25); i++)                                                                            \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        vec_int_push_back(&aa, vb);                                                                                    \
        bb.push_back(vb);                                                                                              \
    }                                                                                                                  \
    vec_int_sort(&aa);                                                                                                 \
    std::sort(bb.begin(), bb.end());                                                                                   \
    vec_int_it range2 = vec_int_begin(&aa);                                                                            \
    print_vec(&aa)

#define SETUP_ARR1                                                                                                     \
    arr25_int a = arr25_int_init();                                                                                    \
    std::array<int, 25> b;                                                                                             \
    for (int i = 0; i < TEST_RAND(TEST_MAX_SIZE); i++)                                                                 \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        a.vector[i] = vb;                                                                                              \
        b[i] = vb;                                                                                                     \
    }                                                                                                                  \
    arr25_int_sort(&a);                                                                                                \
    std::sort(b.begin(), b.end());                                                                                     \
    print_arr25(&a)
#define SETUP_ARR2                                                                                                     \
    arr25_int aa = arr25_int_init();                                                                                   \
    std::array<int, 25> bb;                                                                                            \
    for (int i = 0; i < TEST_RAND(25); i++)                                                                            \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        aa.vector[i] = vb;                                                                                             \
        bb[i] = vb;                                                                                                    \
    }                                                                                                                  \
    arr25_int_sort(&aa);                                                                                               \
    std::sort(bb.begin(), bb.end());                                                                                   \
    arr25_int_it range2 = arr25_int_begin(&aa);                                                                        \
    print_arr25(&aa)

#define SETUP_DEQ1                                                                                                     \
    deq_int a = deq_int_init();                                                                                        \
    std::deque<int> b;                                                                                                 \
    for (int i = 0; i < TEST_RAND(TEST_MAX_SIZE); i++)                                                                 \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        deq_int_push_back(&a, vb);                                                                                     \
        b.push_back(vb);                                                                                               \
    }                                                                                                                  \
    deq_int_sort(&a);                                                                                                  \
    std::sort(b.begin(), b.end());                                                                                     \
    print_deq(&a)
#define SETUP_DEQ2                                                                                                     \
    deq_int aa = deq_int_init();                                                                                       \
    std::deque<int> bb;                                                                                                \
    for (int i = 0; i < TEST_RAND(25); i++)                                                                            \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        deq_int_push_back(&aa, vb);                                                                                    \
        bb.push_back(vb);                                                                                              \
    }                                                                                                                  \
    deq_int_sort(&aa);                                                                                                 \
    std::sort(bb.begin(), bb.end());                                                                                   \
    deq_int_it range2 = deq_int_begin(&aa);                                                                            \
    print_deq(&aa)

#define SETUP_SET1                                                                                                     \
    set_int a = set_int_init(NULL);                                                                                    \
    std::set<int> b;                                                                                                   \
    for (int i = 0; i < TEST_RAND(TEST_MAX_SIZE); i++)                                                                 \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        set_int_insert(&a, vb);                                                                                        \
        b.insert(vb);                                                                                                  \
    }                                                                                                                  \
    print_set(&a)
#define SETUP_SET2                                                                                                     \
    set_int aa = set_int_init(NULL);                                                                                   \
    std::set<int> bb;                                                                                                  \
    for (int i = 0; i < TEST_RAND(25); i++)                                                                 \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        set_int_insert(&aa, vb);                                                                                       \
        bb.insert(vb);                                                                                                 \
    }                                                                                                                  \
    set_int_it range2 = set_int_begin(&aa);                                                                            \
    print_set(&aa)

#define SETUP_USET1                                                                                                    \
    uset_int a = uset_int_init(NULL, NULL);                                                                            \
    std::unordered_set<int> b;                                                                                         \
    for (int i = 0; i < TEST_RAND(TEST_MAX_SIZE); i++)                                                                 \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        uset_int_insert(&a, vb);                                                                                       \
        b.insert(vb);                                                                                                  \
    }                                                                                                                  \
    print_uset(&a)
#define SETUP_USET2                                                                                                    \
    uset_int aa = uset_int_init(NULL, NULL);                                                                           \
    std::unordered_set<int> bb;                                                                                        \
    for (int i = 0; i < TEST_RAND(25); i++)                                                                 \
    {                                                                                                                  \
        const int vb = TEST_RAND(TEST_MAX_VALUE);                                                                      \
        uset_int_insert(&aa, vb);                                                                                      \
        bb.insert(vb);                                                                                                 \
    }                                                                                                                  \
    uset_int_it range2 = uset_int_begin(&aa);                                                                          \
    print_uset(&aa)

int main(void)
{
    int errors = 0;
    INIT_SRAND;
    INIT_TEST_LOOPS(10);
    for (size_t loop = 0; loop < loops; loop++)
    {
        const types_t t1 = pick_type();
        LOG("main type: %d, ", t1);
        const types_t t2 = pick_type();
        LOG("2nd type: %d\n", t2);

#define FOREACH_METH(TEST)                                                                                             \
    TEST(INSERT_GENERIC)                                                                                               \
    TEST(MERGE_RANGE)                                                                                                  \
    TEST(INCLUDES_RANGE)                                                                                               \
    TEST(EQUAL_RANGE)                                                                                                  \
    TEST(MISMATCH)                                                                                                     \
    TEST(UNION_RANGE)                                                                                                  \
    TEST(INTERSECTION_RANGE)                                                                                           \
    TEST(SYMMETRIC_DIFFERENCE_RANGE)                                                                                   \
    TEST(DIFFERENCE_RANGE)

#define FOREACH_DEBUG(TEST)                                                                                            \
    TEST(SEARCH_RANGE)                                                                                                 \
    TEST(FIND_END_RANGE)                                                                                               \
    TEST(FIND_FIRST_OF_RANGE)                                                                                          \
    // TEST(REMOVE_RANGE)

#define GENERATE_ENUM(x) TEST_##x,
#define GENERATE_NAME(x) #x,

        // clang-format off
        enum
        {
            FOREACH_METH(GENERATE_ENUM)
#ifdef DEBUG
            FOREACH_DEBUG(GENERATE_ENUM)
#endif
            TEST_TOTAL
        };
#ifdef DEBUG
        static const char *test_names[] = {
            FOREACH_METH(GENERATE_NAME)
            FOREACH_DEBUG(GENERATE_NAME)
            ""};
#endif
        // clang-format on
        int which = TEST_RAND(TEST_TOTAL);
        if (test >= 0 && test < (int)TEST_TOTAL)
            which = test;
        LOG("TEST %s %d\n", test_names[which], which);
        switch (which)
        {

        case TEST_INSERT_GENERIC:

#define INSERT_INTO_SET(ty2, ty1, cppty)                                                                               \
    LOG("insert " #ty2 " into " #ty1 "\n");                                                                            \
    /* C++ cannot insert generic iters into set/uset */                                                                \
    ty1##_int_insert_generic(&a, (ty1##_int_it *)&range2);                                                             \
    LOG("=> ");                                                                                                        \
    print_##ty1(&a);                                                                                                   \
    ty2##_int_free(&aa)
#define INSERT_INTO(ty2, ty1, cppty)                                                                                   \
    LOG("insert " #ty2 " into " #ty1 "\n");                                                                            \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int_insert_generic(&begin, (ty1##_int_it *)&range2);                                                         \
    b.insert(b.begin(), bb.begin(), bb.end());                                                                         \
    LOG("=> ");                                                                                                        \
    print_##ty1(&a);                                                                                                   \
    CHECK(ty1, ty2, cppty, a, b);                                                                                      \
    ty2##_int_free(&aa)

            switch (t1)
            {
            // cannot insert into array
            case CTL_ARRAY : break;
#ifdef DEBUG
            case CTL_VECTOR : {
                SETUP_VEC1;
                switch (t2)
                {
                case CTL_VECTOR : {
                    SETUP_VEC2; INSERT_INTO(vec, vec, vector<int>); break;
                }
                case CTL_ARRAY : {
                    SETUP_ARR2; INSERT_INTO(arr25, vec, vector<int>); break;
                }
                case CTL_DEQUE : {
                    SETUP_DEQ2; INSERT_INTO(deq, vec, vector<int>); break;
                }
                case CTL_LIST : {
                    SETUP_LIST2; INSERT_INTO(list, vec, vector<int>); break;
                }
                case CTL_SET : {
                    SETUP_SET2; INSERT_INTO(set, vec, vector<int>); break;
                }
                case CTL_USET : {
                    SETUP_USET2; INSERT_INTO(uset, vec, vector<int>); break;
                }
                } // switch t2
                vec_int_free(&a); break;
            }
            case CTL_DEQUE : {
                SETUP_DEQ1;
                switch (t2)
                {
                case CTL_VECTOR : {
                    SETUP_VEC2; INSERT_INTO(vec, deq, deque<int>); break;
                }
                case CTL_ARRAY : {
                    SETUP_ARR2; INSERT_INTO(arr25, deq, deque<int>); break;
                }
                case CTL_DEQUE : {
                    SETUP_DEQ2; INSERT_INTO(deq, deq, deque<int>); break;
                }
                case CTL_LIST : {
                    SETUP_LIST2; INSERT_INTO(list, deq, deque<int>); break;
                }
                case CTL_SET : {
                    SETUP_SET2; INSERT_INTO(set, deq, deque<int>); break;
                }
                case CTL_USET : {
                    SETUP_USET2; INSERT_INTO(uset, deq, deque<int>); break;
                }
                } // switch t2
                deq_int_free(&a); break;
            }
#else
            // not yet stable
            case CTL_VECTOR : break;
            case CTL_DEQUE : break;
            case CTL_USET : break;
#endif
            case CTL_LIST : {
                SETUP_LIST1;
                switch (t2)
                {
                case CTL_ARRAY : {
                    SETUP_ARR2; INSERT_INTO(arr25, list, list<int>); break;
                }
                case CTL_VECTOR : {
                    SETUP_VEC2; INSERT_INTO(vec, list, list<int>); break;
                }
                case CTL_DEQUE : {
                    SETUP_DEQ2; INSERT_INTO(deq, list, list<int>); break;
                }
                case CTL_LIST : {
                    SETUP_LIST2; INSERT_INTO(list, list, list<int>); break;
                }
                case CTL_SET : {
                    SETUP_SET2; INSERT_INTO(set, list, list<int>); break;
                }
                case CTL_USET : {
                    SETUP_USET2; INSERT_INTO(uset, list, list<int>); break;
                }
                } // switch t2
                list_int_free(&a); break;
            }
            // C++ cannot insert into set. CTL can
            case CTL_SET : {
                SETUP_SET1;
                switch (t2)
                {
                case CTL_VECTOR : {
                    SETUP_VEC2; INSERT_INTO_SET(vec, set, set<int>); break;
                }
                case CTL_ARRAY : {
                    SETUP_ARR2; INSERT_INTO_SET(arr25, set, set<int>); break;
                }
                case CTL_DEQUE : {
                    SETUP_DEQ2; INSERT_INTO_SET(deq, set, set<int>); break;
                }
                case CTL_LIST : {
                    SETUP_LIST2; INSERT_INTO_SET(list, set, set<int>); break;
                }
                case CTL_SET : {
                    SETUP_SET2; INSERT_INTO_SET(set, set, set<int>); break;
                }
                case CTL_USET : {
                    SETUP_USET2; INSERT_INTO_SET(uset, set, set<int>); break;
                }
                } // switch t2
                set_int_free(&a); break;
            }
#ifdef DEBUG
            // C++ cannot insert into unordered_set. CTL can
            case CTL_USET: {
                SETUP_USET1;
                switch (t2)
                {
                case CTL_VECTOR : {
                    SETUP_VEC2; INSERT_INTO_SET(vec, uset, unordered_set<int>); break;
                }
                case CTL_ARRAY : {
                    SETUP_ARR2; INSERT_INTO_SET(arr25, uset, unordered_set<int>); break;
                }
                case CTL_LIST : {
                    SETUP_LIST2; INSERT_INTO_SET(list, uset, unordered_set<int>); break;
                }
                case CTL_DEQUE : {
                    SETUP_DEQ2; INSERT_INTO_SET(deq, uset, unordered_set<int>); break;
                }
                case CTL_SET : {
                    SETUP_SET2; INSERT_INTO_SET(set, uset, unordered_set<int>); break;
                }
                case CTL_USET : {
                    SETUP_USET2; INSERT_INTO_SET(uset, uset, unordered_set<int>); break;
                }
                } // switch t2
                uset_int_free(&a); break;
            }
#endif // DEBUG
            } // switch t1
            break;

            case TEST_MERGE_RANGE:

#ifndef _MSC_VER
#define MERGE_INTO(ty2, ty1, cppty)                                                                                    \
    LOG("merge " #ty2 " into " #ty1 "\n");                                                                             \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_merge_range(&begin, (ty1##_int_it *)&range2);                                            \
    std::cppty bbb;                                                                                                    \
    std::merge(b.begin(), b.end(), bb.begin(), bb.end(), std::back_inserter(bbb));                                     \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    CHECK(ty1, ty2, cppty, aaa, bbb);                                                                                  \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#define MERGE_INTO_SET(ty2, ty1, cppty)                                                                                \
    LOG("merge " #ty2 " into " #ty1 "\n");                                                                             \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_merge_range(&begin, (ty1##_int_it *)&range2);                                            \
    std::cppty bbb;                                                                                                    \
    std::merge(b.begin(), b.end(), bb.begin(), bb.end(), std::inserter(bbb, bbb.begin()));                             \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    CHECK(ty1, ty2, cppty, aaa, bbb);                                                                                  \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#else
#define MERGE_INTO(ty2, ty1, cppty)                                                                                    \
    LOG("merge " #ty2 " into " #ty1 "\n");                                                                             \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_merge_range(&begin, (ty1##_int_it *)&range2);                                            \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#define MERGE_INTO_SET(ty2, ty1, cppty)  MERGE_INTO(ty2, ty1, cppty)
#endif

                switch (t1)
                {
                case CTL_LIST : {
                    SETUP_LIST1;
                    switch (t2)
                    {
                    case CTL_LIST : {
                        SETUP_LIST2;
                        LOG("merge list into list\n");
                        list_int_it begin = list_int_begin(&a);
                        list_int aaa = list_int_merge_range(&begin, &range2);
                        b.merge(bb);
                        CHECK(list, list, list<int>, aaa, b);
                        list_int_free(&aa);
                        list_int_free(&aaa);
                        break;
                    }
                    case CTL_VECTOR : {
                        SETUP_VEC2; MERGE_INTO(vec, list, list<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; MERGE_INTO(arr25, list, list<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; MERGE_INTO(deq, list, list<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; MERGE_INTO(set, list, list<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; MERGE_INTO(uset, list, list<int>); break;
                    }
                    } // switch t2
                    list_int_free(&a); break;
                } // LIST
                case CTL_VECTOR : {
                    SETUP_VEC1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; MERGE_INTO(vec, vec, vector<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; MERGE_INTO(arr25, vec, vector<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; MERGE_INTO(deq, vec, vector<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; MERGE_INTO(list, vec, vector<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; MERGE_INTO(set, vec, vector<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; MERGE_INTO(uset, vec, vector<int>); break;
                    }
                    } // switch t2
                    vec_int_free(&a); break;
                } // VECTOR
                // cannot merge into array
                case CTL_ARRAY : break;
                case CTL_DEQUE : {
                    SETUP_DEQ1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; MERGE_INTO(vec, deq, deque<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; MERGE_INTO(arr25, deq, deque<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; MERGE_INTO(list, deq, deque<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; MERGE_INTO(deq, deq, deque<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; MERGE_INTO(set, deq, deque<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; MERGE_INTO(uset, deq, deque<int>); break;
                    }
                    } // switch t2
                    deq_int_free(&a); break;
                }
                case CTL_SET : {
                    SETUP_SET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; MERGE_INTO_SET(vec, set, set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; MERGE_INTO_SET(arr25, set, set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; MERGE_INTO_SET(list, set, set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; MERGE_INTO_SET(deq, set, set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; MERGE_INTO_SET(set, set, set<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; MERGE_INTO_SET(uset, set, set<int>); break;
                    }
                    } // switch t2
                    set_int_free(&a); break;
                }
                case CTL_USET : {
                    SETUP_USET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; MERGE_INTO_SET(vec, uset, unordered_set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; MERGE_INTO_SET(arr25, uset, unordered_set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; MERGE_INTO_SET(list, uset, unordered_set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; MERGE_INTO_SET(deq, uset, unordered_set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; MERGE_INTO_SET(set, uset, unordered_set<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; MERGE_INTO_SET(uset, uset, unordered_set<int>); break;
                    }
                    } // switch t2
                    uset_int_free(&a); break;
                }
                } // switch t1
                break;

            case TEST_INCLUDES_RANGE:

#define INCLUDES_RANGE(ty2, ty1)                                                                                       \
    LOG("includes " #ty2 " from " #ty1 "\n");                                                                          \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    bool a_found = ty1##_int_includes_range(&begin, (ty1##_int_it *)&range2);                                          \
    bool b_found = std::includes(b.begin(), b.end(), bb.begin(), bb.end());                                            \
    LOG("a_found %d == b_found %d\n", (int)a_found, (int)b_found);                                                     \
    assert(a_found == b_found);                                                                                        \
    ty2##_int_free(&aa)

                switch (t1)
                {
                case CTL_VECTOR : {
                    SETUP_VEC1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; INCLUDES_RANGE(vec, vec); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; INCLUDES_RANGE(arr25, vec); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; INCLUDES_RANGE(deq, vec); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; INCLUDES_RANGE(list, vec); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; INCLUDES_RANGE(set, vec); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; INCLUDES_RANGE(uset, vec); break;
                    }
                    } // switch t2
                    vec_int_free(&a); break;
                }
                case CTL_ARRAY : {
                    SETUP_ARR1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; INCLUDES_RANGE(vec, arr25); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; INCLUDES_RANGE(arr25, arr25); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; INCLUDES_RANGE(deq, arr25); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; INCLUDES_RANGE(list, arr25); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; INCLUDES_RANGE(set, arr25); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; INCLUDES_RANGE(uset, arr25); break;
                    }
                    } // switch t2
                    arr25_int_free(&a); break;
                }
                case CTL_DEQUE : {
                    SETUP_DEQ1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; INCLUDES_RANGE(vec, deq); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; INCLUDES_RANGE(arr25, deq); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; INCLUDES_RANGE(deq, deq); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; INCLUDES_RANGE(list, deq); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; INCLUDES_RANGE(set, deq); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; INCLUDES_RANGE(uset, deq); break;
                    }
                    } // switch t2
                    deq_int_free(&a); break;
                }
                case CTL_LIST : {
                    SETUP_LIST1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; INCLUDES_RANGE(vec, list); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; INCLUDES_RANGE(arr25, list); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; INCLUDES_RANGE(deq, list); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; INCLUDES_RANGE(list, list); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; INCLUDES_RANGE(set, list); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; INCLUDES_RANGE(uset, list); break;
                    }
                    } // switch t2
                    list_int_free(&a); break;
                }
                case CTL_SET : {
                    SETUP_SET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; INCLUDES_RANGE(vec, set); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; INCLUDES_RANGE(arr25, set); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; INCLUDES_RANGE(deq, set); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; INCLUDES_RANGE(list, set); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; INCLUDES_RANGE(set, set); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; INCLUDES_RANGE(uset, set); break;
                    }
                    } // switch t2
                    set_int_free(&a); break;
                }
                case CTL_USET : break;
                } // switch t1
                break;

            case TEST_EQUAL_RANGE:

#if __cpp_lib_robust_nonmodifying_seq_ops >= 201304L
#define EQUAL_RANGE(ty2, ty1)                                                                                          \
    LOG("equal_range " #ty2 " from " #ty1 "\n");                                                                       \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    bool same_a = ty1##_int_equal_range(&begin, (ty1##_int_it *)&range2);                                              \
    bool same_b = std::equal(b.begin(), b.end(), bb.begin(), bb.end());                                                \
    LOG("same_a %d == same_b %d\n", (int)same_a, (int)same_b);                                                         \
    assert(same_a == same_b);                                                                                          \
    ty2##_int_free(&aa)
#else
#define EQUAL_RANGE(ty2, ty1)                                                                                          \
    LOG("equal_range " #ty2 " from " #ty1 "\n");                                                                       \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    bool same_a = ty1##_int_equal_range(&begin, (ty1##_int_it *)&range2);                                              \
    bool same_b = std::equal(b.begin(), b.end(), bb.begin());                                                          \
    LOG("same_a %d == same_b %d\n", (int)same_a, (int)same_b);                                                         \
    if (same_a != same_b)                                                                                              \
        printf("std::equal requires C++14 with robust_nonmodifying_seq_ops\n");                                        \
    ty2##_int_free(&aa)
#endif

                switch (t1)
                {
                case CTL_VECTOR : {
                    SETUP_VEC1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; EQUAL_RANGE(vec, vec); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; EQUAL_RANGE(arr25, vec); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; EQUAL_RANGE(deq, vec); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; EQUAL_RANGE(list, vec); break;
                    }
                    case CTL_SET : break;
                    case CTL_USET : break;
                    } // switch t2
                    vec_int_free(&a); break;
                }
                case CTL_ARRAY : {
                    SETUP_ARR1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; EQUAL_RANGE(vec, arr25); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; EQUAL_RANGE(arr25, arr25); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; EQUAL_RANGE(deq, arr25); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; EQUAL_RANGE(list, arr25); break;
                    }
                    case CTL_SET : break;
                    case CTL_USET : break;
                    } // switch t2
                    arr25_int_free(&a); break;
                }
                case CTL_DEQUE : {
                    SETUP_DEQ1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; EQUAL_RANGE(vec, deq); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; EQUAL_RANGE(arr25, deq); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; EQUAL_RANGE(deq, deq); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; EQUAL_RANGE(list, deq); break;
                    }
                    case CTL_SET : break;
                    case CTL_USET : break;
                    } // switch t2
                    deq_int_free(&a); break;
                }
                case CTL_LIST : {
                    SETUP_LIST1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; EQUAL_RANGE(vec, list); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; EQUAL_RANGE(arr25, list); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; EQUAL_RANGE(deq, list); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; EQUAL_RANGE(list, list); break;
                    }
                    case CTL_SET : break;
                    case CTL_USET : break;
                    } // switch t2
                    list_int_free(&a); break;
                }
                case CTL_SET : break;
                case CTL_USET : break;
                } // switch t1
                break;

        case TEST_MISMATCH:

#if __cpp_lib_robust_nonmodifying_seq_ops >= 201304L
#define MISMATCH(ty2, ty1)                                                                                             \
    LOG("mismatch " #ty2 " from " #ty1 "\n");                                                                          \
    ty1##_int_it b1 = ty1##_int_begin(&a);                                                                             \
    ty2##_int_it b2 = ty2##_int_begin(&aa);                                                                            \
    ty1##_int_it r1a = ty1##_int_begin(&a);                                                                            \
    ty1##_int_mismatch(&r1a, (ty1##_int_it *)&range2);                                                                 \
    auto pair = std::mismatch(b.begin(), b.end(), bb.begin(), bb.end());                                               \
    int d1a = ty1##_int_it_distance(&b1, &r1a);                                                                        \
    int d2a = ty2##_int_it_distance(&b2, &range2);                                                                     \
    assert(d1a == std::distance(b.begin(), pair.first));                                                               \
    assert(d2a == std::distance(bb.begin(), pair.second));                                                             \
    ty2##_int_free(&aa)
#else
#define MISMATCH(ty2, ty1)                                                                                             \
    LOG("mismatch " #ty2 " with " #ty1 "\n");                                                                          \
    ty1##_int_it b1 = ty1##_int_begin(&a);                                                                             \
    ty2##_int_it b2 = ty2##_int_begin(&aa);                                                                            \
    ty1##_int_it r1a = ty1##_int_begin(&a);                                                                            \
    /*bool same_a = */ ty1##_int_mismatch(&r1a, (ty1##_int_it *)&range2);                                              \
    if (!bb.size() || !std::distance(bb.begin(), bb.end()))                                                            \
    {                                                                                                                  \
        printf("skip std::mismatch with empty 2nd range. use C++14\n");                                                \
        ty2##_int_free(&aa);                                                                                           \
        break;                                                                                                         \
    }                                                                                                                  \
    auto pair = std::mismatch(b.begin(), b.end(), bb.begin());                                                         \
    int d1a = ty1##_int_it_distance(&b1, &r1a);                                                                        \
    int d2a = ty2##_int_it_distance(&b2, &range2);                                                                     \
    assert(d1a == std::distance(b.begin(), pair.first));                                                               \
    assert(d2a == std::distance(bb.begin(), pair.second));                                                             \
    ty2##_int_free(&aa)
#endif

                switch (t1)
                {
                case CTL_VECTOR : {
                    SETUP_VEC1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; MISMATCH(vec, vec); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; MISMATCH(arr25, vec); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; MISMATCH(deq, vec); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; MISMATCH(list, vec); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; MISMATCH(set, vec); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    vec_int_free(&a); break;
                }
                case CTL_ARRAY : break;
                case CTL_DEQUE : {
                    SETUP_DEQ1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; MISMATCH(vec, deq); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; MISMATCH(arr25, deq); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; MISMATCH(deq, deq); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; MISMATCH(list, deq); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; MISMATCH(set, deq); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    deq_int_free(&a); break;
                }
                case CTL_LIST : {
                    SETUP_LIST1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; MISMATCH(vec, list); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; MISMATCH(arr25, list); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; MISMATCH(deq, list); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; MISMATCH(list, list); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; MISMATCH(set, list); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    list_int_free(&a); break;
                }
                case CTL_SET : {
                    SETUP_SET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; MISMATCH(vec, set); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; MISMATCH(arr25, set); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; MISMATCH(deq, set); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; MISMATCH(list, set); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; MISMATCH(set, set); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    set_int_free(&a); break;
                }
                case CTL_USET : break;
                } // switch t1
                break;

        case TEST_UNION_RANGE:

#ifndef _MSC_VER
#define UNION_RANGE_SET(ty2, ty1, cppty)                                                                               \
    LOG("union " #ty2 " from " #ty1 "\n");                                                                             \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_union_range(&begin, (ty1##_int_it *)&range2);                                            \
    std::cppty bbb;                                                                                                    \
    std::set_union(b.begin(), b.end(), bb.begin(), bb.end(), std::inserter(bbb, bbb.begin()));                         \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    CHECK(ty1, ty2, cppty, aaa, bbb);                                                                                  \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#define UNION_RANGE(ty2, ty1, cppty)                                                                                   \
    LOG("union " #ty2 " from " #ty1 "\n");                                                                             \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_union_range(&begin, (ty1##_int_it *)&range2);                                            \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    std::cppty bbb;                                                                                                    \
    std::set_union(b.begin(), b.end(), bb.begin(), bb.end(), std::back_inserter(bbb));                                 \
    CHECK(ty1, ty2, cppty, aaa, bbb);                                                                                  \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#else
#define UNION_RANGE(ty2, ty1, cppty)                                                                               \
    LOG("union " #ty2 " from " #ty1 "\n");                                                                             \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_union_range(&begin, (ty1##_int_it *)&range2);                                            \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#define UNION_RANGE_SET(ty2, ty1, cppty) UNION_RANGE(ty2, ty1, cppty)
#endif

                switch (t1)
                {
                case CTL_VECTOR : {
                    SETUP_VEC1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; UNION_RANGE(vec, vec, vector<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; UNION_RANGE(arr25, vec, vector<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; UNION_RANGE(deq, vec, vector<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; UNION_RANGE(list, vec, vector<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; UNION_RANGE(set, vec, vector<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; UNION_RANGE(uset, vec, vector<int>); break;
                    }
                    } // switch t2
                    vec_int_free(&a); break;
                }
                case CTL_ARRAY : break;
                case CTL_DEQUE : {
                    SETUP_DEQ1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; UNION_RANGE(vec, deq, deque<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; UNION_RANGE(arr25, deq, deque<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; UNION_RANGE(deq, deq, deque<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; UNION_RANGE(list, deq, deque<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; UNION_RANGE(set, deq, deque<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; UNION_RANGE(uset, deq, deque<int>); break;
                    }
                    } // switch t2
                    deq_int_free(&a); break;
                }
                case CTL_LIST : {
                    SETUP_LIST1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; UNION_RANGE(vec, list, list<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; UNION_RANGE(arr25, list, list<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; UNION_RANGE(deq, list, list<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; UNION_RANGE(list, list, list<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; UNION_RANGE(set, list, list<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; UNION_RANGE(uset, list, list<int>); break;
                    }
                    } // switch t2
                    list_int_free(&a); break;
                }
                case CTL_SET : {
                    SETUP_SET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; UNION_RANGE_SET(vec, set, set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; UNION_RANGE_SET(arr25, set, set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; UNION_RANGE_SET(deq, set, set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; UNION_RANGE_SET(list, set, set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; UNION_RANGE_SET(set, set, set<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; UNION_RANGE_SET(uset, set, set<int>); break;
                    }
                    } // switch t2
                    set_int_free(&a); break;
                }
                case CTL_USET : {
                    SETUP_USET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; UNION_RANGE_SET(vec, uset, unordered_set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; UNION_RANGE_SET(arr25, uset, unordered_set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; UNION_RANGE_SET(deq, uset, unordered_set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; UNION_RANGE_SET(list, uset, unordered_set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; UNION_RANGE_SET(set, uset, unordered_set<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; UNION_RANGE_SET(uset, uset, unordered_set<int>); break;
                    }
                    } // switch t2
                    uset_int_free(&a); break;
                }
                } // switch t1
                break;

            case TEST_INTERSECTION_RANGE:

#ifndef _MSC_VER
#define INTERSECTION_RANGE_SET(ty2, ty1, cppty)                                                                        \
    LOG("intersection " #ty2 " from " #ty1 "\n");                                                                      \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_intersection_range(&begin, (ty1##_int_it *)&range2);                                     \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    std::cppty bbb;                                                                                                    \
    std::set_intersection(b.begin(), b.end(), bb.begin(), bb.end(), std::inserter(bbb, bbb.begin()));                  \
    CHECK(ty1, ty2, cppty, aaa, bbb);                                                                                  \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#define INTERSECTION_RANGE(ty2, ty1, cppty)                                                                            \
    LOG("intersection " #ty2 " from " #ty1 "\n");                                                                      \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_intersection_range(&begin, (ty1##_int_it *)&range2);                                     \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    std::cppty bbb;                                                                                                    \
    std::set_intersection(b.begin(), b.end(), bb.begin(), bb.end(), std::back_inserter(bbb));                          \
    CHECK(ty1, ty2, cppty, aaa, bbb);                                                                                  \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#else
#define INTERSECTION_RANGE(ty2, ty1, cppty)                                                                            \
    LOG("intersection " #ty2 " from " #ty1 "\n");                                                                      \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_intersection_range(&begin, (ty1##_int_it *)&range2);                                     \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#define INTERSECTION_RANGE_SET(ty2, ty1, cppty) INTERSECTION_RANGE(ty2, ty1, cppty)
#endif

                switch (t1)
                {
                case CTL_VECTOR : {
                    SETUP_VEC1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; INTERSECTION_RANGE(vec, vec, vector<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; INTERSECTION_RANGE(arr25, vec, vector<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; INTERSECTION_RANGE(deq, vec, vector<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; INTERSECTION_RANGE(list, vec, vector<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; INTERSECTION_RANGE(set, vec, vector<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; INTERSECTION_RANGE(uset, vec, vector<int>); break;
                    }
                    } // switch t2
                    vec_int_free(&a); break;
                }
                case CTL_ARRAY : break;
                case CTL_DEQUE : {
                    SETUP_DEQ1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; INTERSECTION_RANGE(vec, deq, deque<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; INTERSECTION_RANGE(arr25, deq, deque<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; INTERSECTION_RANGE(deq, deq, deque<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; INTERSECTION_RANGE(list, deq, deque<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; INTERSECTION_RANGE(set, deq, deque<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; INTERSECTION_RANGE(uset, deq, deque<int>); break;
                    }
                    } // switch t2
                    deq_int_free(&a); break;
                }
                case CTL_LIST : {
                    SETUP_LIST1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; INTERSECTION_RANGE(vec, list, list<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; INTERSECTION_RANGE(arr25, list, list<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; INTERSECTION_RANGE(deq, list, list<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; INTERSECTION_RANGE(list, list, list<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; INTERSECTION_RANGE(set, list, list<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; INTERSECTION_RANGE(uset, list, list<int>); break;
                    }
                    } // switch t2
                    list_int_free(&a); break;
                }
                case CTL_SET : {
                    SETUP_SET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; INTERSECTION_RANGE_SET(vec, set, set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; INTERSECTION_RANGE_SET(arr25, set, set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; INTERSECTION_RANGE_SET(deq, set, set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; INTERSECTION_RANGE_SET(list, set, set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; INTERSECTION_RANGE_SET(set, set, set<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; INTERSECTION_RANGE_SET(uset, set, set<int>); break;
                    }
                    } // switch t2
                    set_int_free(&a); break;
                }
                case CTL_USET : {
                    SETUP_USET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; INTERSECTION_RANGE_SET(vec, uset, unordered_set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; INTERSECTION_RANGE_SET(arr25, uset, unordered_set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; INTERSECTION_RANGE_SET(deq, uset, unordered_set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; INTERSECTION_RANGE_SET(list, uset, unordered_set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; INTERSECTION_RANGE_SET(set, uset, unordered_set<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; INTERSECTION_RANGE_SET(uset, uset, unordered_set<int>); break;
                    }
                    } // switch t2
                    uset_int_free(&a); break;
                }
                } // switch t1
                break;

            case TEST_SYMMETRIC_DIFFERENCE_RANGE:

#ifndef _MSC_VER
#define SYMMETRIC_DIFFERENCE_RANGE_SET(ty2, ty1, cppty)                                                                \
    LOG("symmetric_difference " #ty2 " from " #ty1 "\n");                                                              \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_symmetric_difference_range(&begin, (ty1##_int_it *)&range2);                             \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    std::cppty bbb;                                                                                                    \
    std::set_symmetric_difference(b.begin(), b.end(), bb.begin(), bb.end(), std::inserter(bbb, bbb.begin()));          \
    CHECK(ty1, ty2, cppty, aaa, bbb);                                                                                  \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#define SYMMETRIC_DIFFERENCE_RANGE(ty2, ty1, cppty)                                                                    \
    LOG("symmetric_difference " #ty2 " from " #ty1 "\n");                                                              \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_symmetric_difference_range(&begin, (ty1##_int_it *)&range2);                             \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    std::cppty bbb;                                                                                                    \
    std::set_symmetric_difference(b.begin(), b.end(), bb.begin(), bb.end(), std::back_inserter(bbb));                  \
    CHECK(ty1, ty2, cppty, aaa, bbb);                                                                                  \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#else
#define SYMMETRIC_DIFFERENCE_RANGE(ty2, ty1, cppty)                                                                    \
    LOG("symmetric_difference " #ty2 " from " #ty1 "\n");                                                              \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_symmetric_difference_range(&begin, (ty1##_int_it *)&range2);                             \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#define SYMMETRIC_DIFFERENCE_RANGE_SET(ty2, ty1, cppty) SYMMETRIC_DIFFERENCE_RANGE(ty2, ty1, cppty)
#endif

                switch (t1)
                {
                case CTL_VECTOR : {
                    SETUP_VEC1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; SYMMETRIC_DIFFERENCE_RANGE(vec, vec, vector<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; SYMMETRIC_DIFFERENCE_RANGE(arr25, vec, vector<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; SYMMETRIC_DIFFERENCE_RANGE(deq, vec, vector<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; SYMMETRIC_DIFFERENCE_RANGE(list, vec, vector<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; SYMMETRIC_DIFFERENCE_RANGE(set, vec, vector<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; SYMMETRIC_DIFFERENCE_RANGE(uset, vec, vector<int>); break;
                    }
                    } // switch t2
                    vec_int_free(&a); break;
                }
                case CTL_ARRAY : break;
                case CTL_DEQUE : {
                    SETUP_DEQ1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; SYMMETRIC_DIFFERENCE_RANGE(vec, deq, deque<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; SYMMETRIC_DIFFERENCE_RANGE(arr25, deq, deque<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; SYMMETRIC_DIFFERENCE_RANGE(deq, deq, deque<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; SYMMETRIC_DIFFERENCE_RANGE(list, deq, deque<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; SYMMETRIC_DIFFERENCE_RANGE(set, deq, deque<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; SYMMETRIC_DIFFERENCE_RANGE(uset, deq, deque<int>); break;
                    }
                    } // switch t2
                    deq_int_free(&a); break;
                }
                case CTL_LIST : {
                    SETUP_LIST1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; SYMMETRIC_DIFFERENCE_RANGE(vec, list, list<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; SYMMETRIC_DIFFERENCE_RANGE(arr25, list, list<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; SYMMETRIC_DIFFERENCE_RANGE(deq, list, list<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; 
                        SYMMETRIC_DIFFERENCE_RANGE(list, list, list<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; SYMMETRIC_DIFFERENCE_RANGE(set, list, list<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; SYMMETRIC_DIFFERENCE_RANGE(uset, list, list<int>); break;
                    }
                    } // switch t2
                    list_int_free(&a); break;
                }
                case CTL_SET : break; // nyi
#if 0
                {
                    SETUP_SET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; SYMMETRIC_DIFFERENCE_RANGE_SET(vec, set, set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; SYMMETRIC_DIFFERENCE_RANGE_SET(arr25, set, set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; SYMMETRIC_DIFFERENCE_RANGE_SET(deq, set, set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; SYMMETRIC_DIFFERENCE_RANGE_SET(list, set, set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; SYMMETRIC_DIFFERENCE_RANGE_SET(set, set, set<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; SYMMETRIC_DIFFERENCE_RANGE_SET(uset, set, set<int>); break;
                    }
                    } // switch t2
                    set_int_free(&a); break;
                }
#endif 
                case CTL_USET : break; // nyi
#if 0
                {
                    SETUP_USET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; SYMMETRIC_DIFFERENCE_RANGE_SET(vec, uset, unordered_set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; SYMMETRIC_DIFFERENCE_RANGE_SET(arr25, uset, unordered_set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; SYMMETRIC_DIFFERENCE_RANGE_SET(deq, uset, unordered_set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; SYMMETRIC_DIFFERENCE_RANGE_SET(list, uset, unordered_set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; SYMMETRIC_DIFFERENCE_RANGE_SET(set, uset, unordered_set<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; SYMMETRIC_DIFFERENCE_RANGE_SET(uset, uset, unordered_set<int>); break;
                    }
                    } // switch t2
                    uset_int_free(&a); break;
                }
#endif
                } // switch t1
                break;

            case TEST_DIFFERENCE_RANGE:

#ifndef _MSC_VER
#define DIFFERENCE_RANGE_SET(ty2, ty1, cppty)                                                                          \
    LOG("difference " #ty2 " from " #ty1 "\n");                                                                        \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_difference_range(&begin, (ty1##_int_it *)&range2);                                       \
    print_##ty1(&aaa);                                                                                                 \
    std::cppty bbb;                                                                                                    \
    std::set_difference(b.begin(), b.end(), bb.begin(), bb.end(), std::inserter(bbb, bbb.begin()));                    \
    CHECK(ty1, ty2, cppty, aaa, bbb);                                                                                  \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#define DIFFERENCE_RANGE(ty2, ty1, cppty)                                                                              \
    LOG("difference " #ty2 " from " #ty1 "\n");                                                                        \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_difference_range(&begin, (ty1##_int_it *)&range2);                                       \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    std::cppty bbb;                                                                                                    \
    std::set_difference(b.begin(), b.end(), bb.begin(), bb.end(), std::back_inserter(bbb));                            \
    CHECK(ty1, ty2, cppty, aaa, bbb);                                                                                  \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#else
#define DIFFERENCE_RANGE(ty2, ty1, cppty)                                                                              \
    LOG("difference " #ty2 " from " #ty1 "\n");                                                                        \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int aaa = ty1##_int_difference_range(&begin, (ty1##_int_it *)&range2);                                       \
    LOG("=> ");                                                                                                        \
    print_##ty1(&aaa);                                                                                                 \
    ty1##_int_free(&aaa);                                                                                              \
    ty2##_int_free(&aa)
#define DIFFERENCE_RANGE_SET(ty2, ty1, cppty) DIFFERENCE_RANGE(ty2, ty1, cppty)
#endif

                switch (t1)
                {
                case CTL_VECTOR: {
                    SETUP_VEC1;
                    switch (t2)
                    {
                    case CTL_VECTOR: {
                        SETUP_VEC2; DIFFERENCE_RANGE(vec, vec, vector<int>); break;
                    }
                    case CTL_ARRAY: {
                        SETUP_ARR2; DIFFERENCE_RANGE(arr25, vec, vector<int>); break;
                    }
                    case CTL_DEQUE: {
                        SETUP_DEQ2; DIFFERENCE_RANGE(deq, vec, vector<int>); break;
                    }
                    case CTL_LIST: {
                        SETUP_LIST2; DIFFERENCE_RANGE(list, vec, vector<int>); break;
                    }
                    case CTL_SET: {
                        SETUP_SET2; DIFFERENCE_RANGE(set, vec, vector<int>); break;
                    }
                    case CTL_USET: {
                        SETUP_USET2; DIFFERENCE_RANGE(uset, vec, vector<int>); break;
                    }
                    } // switch t2
                    vec_int_free(&a); break;
                    break;
                }
                case CTL_ARRAY:
                    break;
                case CTL_DEQUE: {
                    SETUP_DEQ1;
                    switch (t2)
                    {
                    case CTL_VECTOR: {
                        SETUP_VEC2; DIFFERENCE_RANGE(vec, deq, deque<int>); break;
                    }
                    case CTL_ARRAY: {
                        SETUP_ARR2; DIFFERENCE_RANGE(arr25, deq, deque<int>); break;
                    }
                    case CTL_DEQUE: {
                        SETUP_DEQ2; DIFFERENCE_RANGE(deq, deq, deque<int>); break;
                    }
                    case CTL_LIST: {
                        SETUP_LIST2; DIFFERENCE_RANGE(list, deq, deque<int>); break;
                    }
                    case CTL_SET: {
                        SETUP_SET2; DIFFERENCE_RANGE(set, deq, deque<int>); break;
                    }
                    case CTL_USET: {
                        SETUP_USET2; DIFFERENCE_RANGE(uset, deq, deque<int>); break;
                    }
                    } // switch t2
                    deq_int_free(&a); break;
                }
                case CTL_LIST: {
                    SETUP_LIST1;
                    switch (t2)
                    {
                    case CTL_VECTOR: {
                        SETUP_VEC2; DIFFERENCE_RANGE(vec, list, list<int>); break;
                    }
                    case CTL_ARRAY: {
                        SETUP_ARR2; DIFFERENCE_RANGE(arr25, list, list<int>); break;
                    }
                    case CTL_DEQUE: {
                        SETUP_DEQ2; DIFFERENCE_RANGE(deq, list, list<int>); break;
                    }
                    case CTL_LIST: {
                        SETUP_LIST2; DIFFERENCE_RANGE(list, list, list<int>); break;
                    }
                    case CTL_SET: {
                        SETUP_SET2; DIFFERENCE_RANGE(set, list, list<int>); break;
                    }
                    case CTL_USET: {
                        SETUP_USET2; DIFFERENCE_RANGE(uset, list, list<int>); break;
                    }
                    } // switch t2
                    list_int_free(&a); break;
                }
                case CTL_SET:
                    break; // nyi
#if 0
                {
                    SETUP_SET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; DIFFERENCE_RANGE_SET(vec, set, set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; DIFFERENCE_RANGE_SET(arr25, set, set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; DIFFERENCE_RANGE_SET(deq, set, set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; DIFFERENCE_RANGE_SET(list, set, set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; DIFFERENCE_RANGE_SET(set, set, set<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; DIFFERENCE_RANGE_SET(uset, set, set<int>); break;
                    }
                    } // switch t2
                    set_int_free(&a); break;
                }
#endif
                case CTL_USET:
                    break; // nyi
#if 0
                {
                    SETUP_USET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; DIFFERENCE_RANGE_SET(vec, uset, unordered_set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; DIFFERENCE_RANGE_SET(arr25, uset, unordered_set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; DIFFERENCE_RANGE_SET(deq, uset, unordered_set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; DIFFERENCE_RANGE_SET(list, uset, unordered_set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; DIFFERENCE_RANGE_SET(set, uset, unordered_set<int>); break;
                    }
                    case CTL_USET : {
                        SETUP_USET2; DIFFERENCE_RANGE_SET(uset, uset, unordered_set<int>); break;
                    }
                    } // switch t2
                    uset_int_free(&a); break;
                }
#endif
                } // switch t1
                break;

#ifdef DEBUG
        case TEST_FIND_FIRST_OF_RANGE:

#define FIND_FIRST_OF_RANGE(ty2, ty1, cppty)                                                                           \
    LOG("find_first_of_range " #ty2 " from " #ty1 "\n");                                                               \
    ty1##_int_it pos = ty1##_int_begin(&a);                                                                            \
    bool found_a = ty1##_int_find_first_of_range(&pos, (ty1##_int_it *)&range2);                                       \
    auto iter = std::find_first_of(b.begin(), b.end(), bb.begin(), bb.end());                                          \
    bool found_b = iter != b.end();                                                                                    \
    LOG("found a: %s at %lu, ", found_a ? "yes" : "no", ty1##_int_it_index(&pos));                                     \
    LOG("found b: %s at %ld\n", found_b ? "yes" : "no", std::distance(b.begin(), iter));                               \
    assert(found_a == found_b);                                                                                        \
    if (found_a)                                                                                                       \
        assert(*pos.ref == *iter);                                                                                     \
    ty2##_int_free(&aa)

                switch (t1)
                {
                case CTL_VECTOR : {
                    SETUP_VEC1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; FIND_FIRST_OF_RANGE(vec, vec, vector<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; FIND_FIRST_OF_RANGE(arr25, vec, vector<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; FIND_FIRST_OF_RANGE(deq, vec, vector<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; FIND_FIRST_OF_RANGE(list, vec, vector<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; FIND_FIRST_OF_RANGE(set, vec, vector<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    vec_int_free(&a); break;
                }
                case CTL_ARRAY : {
                    SETUP_ARR1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; FIND_FIRST_OF_RANGE(vec, arr25, arrint<25>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; FIND_FIRST_OF_RANGE(arr25, arr25, arrint<25>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; FIND_FIRST_OF_RANGE(deq, arr25, arrint<25>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; FIND_FIRST_OF_RANGE(list, arr25, arrint<25>); break;
                    }
                    case CTL_SET : break;
                    case CTL_USET : break;
                    } // switch t2
                    arr25_int_free(&a); break;
                }
                case CTL_DEQUE : {
                    SETUP_DEQ1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; FIND_FIRST_OF_RANGE(vec, deq, deque<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; FIND_FIRST_OF_RANGE(arr25, deq, deque<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; FIND_FIRST_OF_RANGE(deq, deq, deque<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; FIND_FIRST_OF_RANGE(list, deq, deque<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; FIND_FIRST_OF_RANGE(set, deq, deque<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    deq_int_free(&a); break;
                }
                case CTL_LIST : {
                    SETUP_LIST1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; FIND_FIRST_OF_RANGE(vec, list, list<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; FIND_FIRST_OF_RANGE(arr25, list, list<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; FIND_FIRST_OF_RANGE(deq, list, list<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; FIND_FIRST_OF_RANGE(list, list, list<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; FIND_FIRST_OF_RANGE(set, list, list<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    list_int_free(&a); break;
                }
                case CTL_SET : {
                    SETUP_SET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; FIND_FIRST_OF_RANGE(vec, set, set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; FIND_FIRST_OF_RANGE(arr25, set, set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; FIND_FIRST_OF_RANGE(deq, set, set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; FIND_FIRST_OF_RANGE(list, set, set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; FIND_FIRST_OF_RANGE(set, set, set<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    set_int_free(&a); break;
                }
                case CTL_USET : break;
                } // switch t1
                break;

            case TEST_SEARCH_RANGE:

#define SEARCH_RANGE(ty2, ty1, cppty)                                                                                  \
    LOG("search_range " #ty2 " from " #ty1 "\n");                                                                      \
    ty1##_int_it pos = ty1##_int_begin(&a);                                                                            \
    bool found_a = ty1##_int_search_range(&pos, (ty1##_int_it *)&range2);                                              \
    auto iter = std::search(b.begin(), b.end(), bb.begin(), bb.end());                                                 \
    bool found_b = iter != b.end();                                                                                    \
    LOG("found a: %s at %lu, ", found_a ? "yes" : "no", ty1##_int_it_index(&pos));                                     \
    LOG("found b: %s at %ld\n", found_b ? "yes" : "no", std::distance(b.begin(), iter));                               \
    assert(found_a == found_b);                                                                                        \
    if (found_a)                                                                                                       \
        assert(*pos.ref == *iter);                                                                                     \
    ty2##_int_free(&aa)

                switch (t1)
                {
                case CTL_VECTOR : {
                    SETUP_VEC1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; SEARCH_RANGE(vec, vec, vector<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; SEARCH_RANGE(arr25, vec, vector<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; SEARCH_RANGE(deq, vec, vector<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; SEARCH_RANGE(list, vec, vector<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; SEARCH_RANGE(set, vec, vector<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    vec_int_free(&a); break;
                }
                case CTL_ARRAY : {
                    SETUP_ARR1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; SEARCH_RANGE(vec, arr25, arrint<25>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; SEARCH_RANGE(arr25, arr25, arrint<25>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; SEARCH_RANGE(deq, arr25, arrint<25>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; SEARCH_RANGE(list, arr25, arrint<25>); break;
                    }
                    case CTL_SET : break;
                    case CTL_USET : break;
                    } // switch t2
                    arr25_int_free(&a); break;
                }
                case CTL_DEQUE : {
                    SETUP_DEQ1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; SEARCH_RANGE(vec, deq, deque<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; SEARCH_RANGE(arr25, deq, deque<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; SEARCH_RANGE(deq, deq, deque<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; SEARCH_RANGE(list, deq, deque<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; SEARCH_RANGE(set, deq, deque<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    deq_int_free(&a); break;
                }
                case CTL_LIST : {
                    SETUP_LIST1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; SEARCH_RANGE(vec, list, list<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; SEARCH_RANGE(arr25, list, list<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; SEARCH_RANGE(deq, list, list<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; SEARCH_RANGE(list, list, list<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; SEARCH_RANGE(set, list, list<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    list_int_free(&a); break;
                }
                case CTL_SET : {
                    SETUP_SET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; SEARCH_RANGE(vec, set, set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; SEARCH_RANGE(arr25, set, set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; SEARCH_RANGE(deq, set, set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; SEARCH_RANGE(list, set, set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; SEARCH_RANGE(set, set, set<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    set_int_free(&a); break;
                }
                case CTL_USET : break;
                } // switch t1
                break;

        case TEST_FIND_END_RANGE:

#if __cpp_lib_erase_if >= 202002L
#define FIND_END_RANGE(ty2, ty1, cppty)                                                                                \
    LOG("find_end_range " #ty2 " from " #ty1 "\n");                                                                    \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int_it it = ty1##_int_find_end_range(&begin, (ty1##_int_it *)&range2);                                       \
    auto iter = std::find_end(b.begin(), b.end(), bb.begin(), bb.end());                                               \
    bool found_a = !ty1##_int_it_done(&it);                                                                            \
    bool found_b = iter != b.end();                                                                                    \
    LOG("found a: %s at %lu, ", found_a ? "yes" : "no", ty1##_int_it_index(&it));                                      \
    LOG("found b: %s at %ld\n", found_b ? "yes" : "no", std::distance(b.begin(), iter));                               \
    assert(found_a == found_b);                                                                                        \
    if (found_a)                                                                                                       \
        assert(*it.ref == *iter);                                                                                      \
    ty2##_int_free(&aa)
#else
#define FIND_END_RANGE(ty2, ty1, cppty)                                                                                \
    LOG("find_end_range " #ty2 " from " #ty1 "\n");                                                                    \
    ty1##_int_it begin = ty1##_int_begin(&a);                                                                          \
    ty1##_int_it it = ty1##_int_find_end_range(&begin, (ty1##_int_it *)&range2);                                       \
    bool found_a = !ty1##_int_it_done(&it);                                                                            \
    LOG("found a: %s at %lu, ", found_a ? "yes" : "no", ty1##_int_it_index(&it));                                      \
    ty2##_int_free(&aa)
#endif

                switch (t1)
                {
                case CTL_VECTOR : {
                    SETUP_VEC1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; FIND_END_RANGE(vec, vec, vector<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; FIND_END_RANGE(arr25, vec, vector<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; FIND_END_RANGE(deq, vec, vector<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; FIND_END_RANGE(list, vec, vector<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; FIND_END_RANGE(set, vec, vector<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    vec_int_free(&a); break;
                }
                case CTL_ARRAY : {
                    SETUP_ARR1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; FIND_END_RANGE(vec, arr25, arrint<25>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; FIND_END_RANGE(arr25, arr25, arrint<25>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; FIND_END_RANGE(deq, arr25, arrint<25>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; FIND_END_RANGE(list, arr25, arrint<25>); break;
                    }
                    case CTL_SET : break;
                    case CTL_USET : break;
                    } // switch t2
                    arr25_int_free(&a); break;
                }
                case CTL_DEQUE : {
                    SETUP_DEQ1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; FIND_END_RANGE(vec, deq, deque<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; FIND_END_RANGE(arr25, deq, deque<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; FIND_END_RANGE(deq, deq, deque<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; FIND_END_RANGE(list, deq, deque<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; FIND_END_RANGE(set, deq, deque<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    deq_int_free(&a); break;
                }
                case CTL_LIST : {
                    SETUP_LIST1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; FIND_END_RANGE(vec, list, list<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; FIND_END_RANGE(arr25, list, list<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; FIND_END_RANGE(deq, list, list<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; FIND_END_RANGE(list, list, list<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; FIND_END_RANGE(set, list, list<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    list_int_free(&a); break;
                }
                case CTL_SET : {
                    SETUP_SET1;
                    switch (t2)
                    {
                    case CTL_VECTOR : {
                        SETUP_VEC2; FIND_END_RANGE(vec, set, set<int>); break;
                    }
                    case CTL_ARRAY : {
                        SETUP_ARR2; FIND_END_RANGE(arr25, set, set<int>); break;
                    }
                    case CTL_DEQUE : {
                        SETUP_DEQ2; FIND_END_RANGE(deq, set, set<int>); break;
                    }
                    case CTL_LIST : {
                        SETUP_LIST2; FIND_END_RANGE(list, set, set<int>); break;
                    }
                    case CTL_SET : {
                        SETUP_SET2; FIND_END_RANGE(set, set, set<int>); break;
                    }
                    case CTL_USET : break;
                    } // switch t2
                    set_int_free(&a); break;
                }
                case CTL_USET : break;
                } // switch t1
                break;
#endif // DEBUG

        default:
#ifdef DEBUG
            printf("unhandled testcase %d %s\n", which, test_names[which]);
#else
            printf("unhandled testcase %d\n", which);
#endif
            break;
        }
    }
    if (errors)
        TEST_FAIL(__FILE__);
    else
        TEST_PASS(__FILE__);
}

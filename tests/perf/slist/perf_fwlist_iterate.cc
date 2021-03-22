#include "../../test.h"

#include <forward_list>
#include <time.h>

int main()
{
    puts(__FILE__);
    srand(time(NULL));
    for(int run = 0; run < TEST_PERF_RUNS; run++)
    {
        std::forward_list<int> c;
        int elems = TEST_PERF_CHUNKS * run;
        for(int elem = 0; elem < elems; elem++)
            c.push_front(rand());
        int t0 = TEST_TIME();
        volatile int sum = 0;
        for(auto& x : c)
            sum += x;
        int t1 = TEST_TIME();
        printf("%10d %10d\n", elems, t1 - t0);
    }
}

#include "../../test.h"

#include <algorithm>
#include <vector>
#include <time.h>

static bool compare(int& a, int& b) { return a < b; }

int main()
{
    puts(__FILE__);
    srand(0xbeef);
    for(int run = 0; run < TEST_PERF_RUNS; run++)
    {
        std::vector<int> c;
        int elems = TEST_PERF_CHUNKS * run;
        for(int elem = 0; elem < elems; elem++)
            c.push_back(rand());
        int t0 = TEST_TIME();
        std::sort(c.begin(), c.end(), compare);
        int t1 = TEST_TIME();
        printf("%10d %10d\n", elems, t1 - t0);
    }
}

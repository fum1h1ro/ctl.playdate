# C CONTAINER TEMPLATE LIBRARY (CTL)

CTL is a fast compiling, type safe, header only, template-like
container library for ISO C99/C11.

## Motivation

CTL aims to improve ISO C99/C11 developer productivity by implementing
the following STL containers in ISO C99/C11:

```
ctl/deque.h          = std::deque           prefix: deq
ctl/list.h           = std::list            prefix: list
ctl/priority_queue.h = std::priority_queue  prefix: pqu
ctl/queue.h          = std::queue           prefix: queue
ctl/set.h            = std::set             prefix: set
ctl/stack.h          = std::stack           prefix: stack
ctl/string.h         = std::string          prefix: str
ctl/vector.h         = std::vector          prefix: vec
ctl/ust.h            = std::unordered_set   prefix: uset
```
map and forward_list are in work still.

It is based on glouw's ctl, but with proper names, and using the incpath `ctl/` prefix.

## Use

Configure a CTL container with a built-in or typedef type `T`.

```C
#include <stdio.h>

#define POD
#define T int
#include <ctl/vector.h>

int compare(int* a, int* b) { return *b < *a; }

int main(void)
{
    vec_int a = vec_int_init();
    vec_int_push_back(&a, 9);
    vec_int_push_back(&a, 1);
    vec_int_push_back(&a, 8);
    vec_int_push_back(&a, 3);
    vec_int_push_back(&a, 4);
    vec_int_sort(&a, compare);
    foreach(vec_int, &a, it)
        printf("%d\n", *it.ref);
    vec_int_free(&a);
}
```

Definition `POD` states type `T` is Plain Old Data (POD).

For a much more thorough getting started guide,
see the wiki: https://github.com/rurban/ctl/wiki and
https://github.com/glouw/ctl/wiki for the original sample with three-letter names.

## Memory Ownership

Types with memory ownership require definition `POD` be omitted, and require
function declarations for the C++ equivalent of the destructor and copy constructor,
prior to the inclusion of the container:

```C
typedef struct { ... } type;
void type_free(type*);
type type_copy(type*);
#define T type
#include <ctl/vector.h>
```

Forgetting a declaration will print a human-readable error message:

```shell
tests/test_c11.c:11:11: error: ‘type_free’ undeclared (first use in this function)
   11 | #define T type
```

## Performance

CTL performance is presented in solid colors, and STL in dotted colors,
for template type `T` as type `int` for all measurements.

![](docs/images/vec.log.png)
![](docs/images/list.log.png)
![](docs/images/deq.log.png)
![](docs/images/set.log.png)
![](docs/images/pqu.log.png)
![](docs/images/compile.log.png)

Omitted from these performance measurements are `queue.h`, `stack.h`, and `string.h`,
as their performance characteristics can be inferred from `deque.h`, and `vector.h`,
respectively.

Note, CTL strings do not support short strings yet.

## Running Tests

To run all functional tests, run:

```shell
make
```

To compile examples, run:

```shell
make examples
```

To generate performance graphs, run:

```shell
sh gen_images.sh
# Graphing requires python3 and the Plotly family of libraries via pip3.
pip install plotly
pip install psutil
pip install kaleido
```

To do all of the above in one step, run:

```shell
./all.sh
```

The full CI suite is run via: (1-2 hrs)
```shell
./ci-all.sh
```

To generate the manpages or run `make install` install the `ronn` gem.

For maintaining CTL, a container templated to type `int` can be
output to `stdout` by running make on the container name with .i, eg:

```shell
make deque.i
make list.i
make priority_queue.i
make queue.i
make set.i
make stack.i
make string.i
make vector.i
make ust.i
```

## Other

STL `std::map` and `std::unordered_set` are in work as unordered hashmap by
default, no extra `std::unordered_map`.
The map hopefully with something like greg7mdp/parallel-hashmap or at least
khash, not chained lists as in the STL.

STL variants of multi-sets and multi-maps will not be implemented because
similar behaviour can be implemented as an amalgamation of a `set` and `list`.
See `tests/func/test_container_composing.cc`

UTF-8 strings and identifiers will be added eventually, Wide, UTF-16 or UTF-32
not. All methods from algorithm, iterator and range are in work, as well as some
type utilities to omit default compare, equal and hash methods.

## Base Implementation Details

```
vector.h: realloc
string.h: vector.h
deque.h:  realloc (paged)
queue.h:  deque.h
stack.h:  deque.h
pque.h:   vector.h
forward_list.h: single linked list
list.h:   doubly linked list
set.h:    red black tree
uset.h:   hashed forward linked lists

                    vec  str  deq  lst  set  pqu  que  stk  ust
+-------------------------------------------------------------+
empty               x    x    x    x    x    x    x    x    x
each                x    x    x    x    x                   x
equal               x    x    x    x    x    x    x    x    x
swap                x    x    x    x    x    x    x    x    x
bucket                                                      x
bucket_size                                                 x
load_factor                                                 x
rehash                                                      x
insert              x    x    x    x    x                   x
init                x    x    x    x    x    x    x    x    x
free                x    x    x    x    x    x    x    x    x
step                x    x    x    x    x                   x
range               x    x    x    x    x                   x
find                x    x    x    x    x                   x
count                    x              x                   x
erase               x    x    x    x    x                   x
copy                x    x    x    x    x                   x
begin               x    x    x    x    x                   x
end                 x    x    x    x    x                   x
intersection                            x                   x
union                                   x                   x
difference                              x                   x
symmetric_difference                    x                   x
top                                          x         x
push                                         x    x    x
pop                                          x    x    x
at                  x    x    x
front               x    x    x    x              x
back                x    x    x    x              x
set                 x    x    x
pop_back            x    x    x    x
pop_front                     x    x
clear               x    x    x    x    x
reserve             x    x
push_back           x    x    x    x
push_front                    x    x
transfer                           x
disconnect                         x
connect                            x
resize              x    x    x    x
assign              x    x    x    x
reverse                            x
shrink_to_fit       x    x
data                x    x
erase_node                              x
sort                x    x    x    x
remove_if           x    x    x    x    x
splice                             x
merge                              x
unique                             x
append                   x
insert_str               x
replace                  x              x
c_str                    x
find                     x
contains                                x
rfind                    x
find_first_of            x
find_last_of             x
find_first_not_of        x
find_last_not_of         x
substr                   x
compare                  x
key_compare              x
```

## Acknowledgements

Thank you `glouw` for the initial three-letter variant https://github.com/glouw/ctl.
Thank you `kully` for the Plotly code, and thank you for the general review.
Thank you `smlckz` for the `foreach` cleanup.

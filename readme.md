# Simple Stacktrace

## Example

```c
// main.c
#include "simple-stacktrace.h"

void test() {
  stacktrace();
}

int main() {
  test();
}
```

```sh
$ clang -g -o main main.c && ./main
0x105b6de49: test (in main) (/Users/arnaud/dev/cpp-stacktrace/main.c:5)
0x105b6de59: main (in main) (/Users/arnaud/dev/cpp-stacktrace/main.c:8)
```

## Getting started

Just copy `simple-stacktrace.h` to your project, add `#include "simple-stacktrace.h"` where you want and call `stacktrace()` where you want to display the stacktrace.

`atos` command must be installed on your system, but I think it is by default.

You must compile with `-g` and have the `.dSYM` folder next to your binary.

## Caveats

Only works on MacOs. May work on linux but I did not try.


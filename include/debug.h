#pragma once

#define ASSERT(e) do {                      \
    if (!(e))                               \
        error_check(__FILE__, __LINE__);    \
} while (0)

void print_tf(struct TrapFrame tf);
void error_check(char *file, uint64_t line);
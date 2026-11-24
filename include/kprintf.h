#ifndef KPRINTF_H
#define KPRINTF_H

int kprintf(const char* fmt, ...) __attribute__((format(printf, 1, 2)));

#endif
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((uint16_t*)0xB8000)

static uint16_t cursor_position = 0;

// Basic outb function for port I/O — **declare first**
static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static void update_cursor() {
    outb(0x3D4, 14);
    outb(0x3D5, cursor_position >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, cursor_position & 0xFF);
}

void print(const char* str) {
    while (*str) {
        if (*str == '\n') {
            cursor_position += VGA_WIDTH - (cursor_position % VGA_WIDTH);
        } else {
            VGA_MEMORY[cursor_position++] = (uint8_t)(*str) | (0x07 << 8); // light grey on black
        }
        str++;
    }
    if (cursor_position >= VGA_WIDTH * VGA_HEIGHT) {
        cursor_position = 0;  // simple wraparound, you can implement scrolling later
    }
    update_cursor();
}

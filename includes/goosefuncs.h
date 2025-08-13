#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((uint16_t*)0xB8000)

static uint8_t cursor_row = 0;
static uint8_t cursor_col = 0;

// ---------- Low-level port I/O ----------
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

// ---------- Cursor control ----------
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void update_cursor(int col, int row) {
    uint16_t pos = row * VGA_WIDTH + col;

    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

// ---------- Screen scrolling ----------
void scroll_screen() {
    // Move every row up by 1
    for (int y = 1; y < VGA_HEIGHT; y++) {
        for (int x = 0; x < VGA_WIDTH; x++) {
            VGA_MEMORY[(y - 1) * VGA_WIDTH + x] =
                VGA_MEMORY[y * VGA_WIDTH + x];
        }
    }

    // Clear the last row
    for (int x = 0; x < VGA_WIDTH; x++) {
        VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
            (0x07 << 8) | ' ';
    }

    cursor_row = VGA_HEIGHT - 1; // stay on last row
}

void outp(const char *msg) {
    while (*msg) {
        if (*msg == '\n') {
            cursor_col = 0;
            cursor_row++;
        } else {
            VGA_MEMORY[cursor_row * VGA_WIDTH + cursor_col] =
                (0x07 << 8) | *msg;
            cursor_col++;
            if (cursor_col >= VGA_WIDTH) {
                cursor_col = 0;
                cursor_row++;
            }
        }

        msg++;
    }
    update_cursor(cursor_col, cursor_row); // only update at end of print
}


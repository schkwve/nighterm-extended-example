#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <limine.h>

#include <nighterm-extended/nighterm.h>

LIMINE_BASE_REVISION(1)

struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

void *memcpy(void *dest, const void *src, size_t n)
{
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    for (size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, size_t n)
{
    uint8_t *p = (uint8_t *)s;

    for (size_t i = 0; i < n; i++) {
        p[i] = (uint8_t)c;
    }

    return s;
}

void *memmove(void *dest, const void *src, size_t n)
{
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;

    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n)
{
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

void _start(void)
{
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        for (;;) {
            __asm__("cli;hlt");
        }
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == NULL
     || framebuffer_request.response->framebuffer_count < 1) {
        for (;;) {
            __asm__("cli;hlt");
        }
    }

    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];

    // Create a new Nighterm context
    struct nighterm_ctx context;

    // Initialize Nighterm Extended
    nighterm_initialize(&context,
            NULL,
            framebuffer->address,
            framebuffer->width,
            framebuffer->height,
            framebuffer->pitch,
            framebuffer->bpp,
            NULL,
	    NULL);

    char *long_string = "Hello. This is a very very long string. If Nighterm is working correctly, this text should span across multiple lines. "
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890123456789\n"
                        "Let's test some escape sequences!\n"
                        "This text should be on its own line.\n"
                        "\tThis text should be indented by a tab (4 spaces). "
                        "This text contains a word that is missing\b its last letter.\n\n"
                        "...And how about some colors?\n";

    char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
    char *alphabet_low = "abcdefghijklmnopqrstuvwxyz\n";

    nighterm_flush(&context, 60, 55, 255);
    nighterm_set_fg_color(&context, 255, 255, 255);
    nighterm_set_bg_color(&context, 60, 55, 255);

    while (*long_string != 0) {
      nighterm_write(&context, *long_string++);
    }

    const uint8_t colors[26] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 255};

    int i = 0;
    while (*alphabet != 0) {
      nighterm_set_fg_color(&context, colors[15 - i], colors[26 - i], colors[i]);
      nighterm_write(&context, *alphabet++);
      i++;
    }

    nighterm_set_fg_color(&context, 255, 255, 255);
    i = 0;

    while (*alphabet_low != 0) {
      nighterm_set_bg_color(&context, colors[15 - i], colors[26 - i], colors[i]);
      nighterm_write(&context, *alphabet_low++);
      i++;
    }

    nighterm_set_bg_color(&context, 0, 0, 0);

    nighterm_shutdown(&context);

    // We're done, just hang...
    for (;;) {
        __asm__("cli;hlt");
    }
}

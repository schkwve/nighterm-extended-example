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

    // Initialize Nighterm Extended
    nighterm_initialize(NULL,
            framebuffer->address,
            framebuffer->width,
            framebuffer->height,
            framebuffer->pitch,
            framebuffer->bpp,
            NULL, NULL);

    char *long_string = "Hello. This is a very very long string. If Nighterm is working correctly, this text should span across multiple lines. "
                        "ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890123456789\n"
                        "Let's test some escape sequences!\n"
                        "This text should be on its own line.\n"
                        "\tThis text should be indented by a tab (4 spaces). "
                        "This text contains a word that is missing\b its last letter.\n\n"
                        "...And how about some colors?\n";

    char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
    char *alphabet_low = "abcdefghijklmnopqrstuvwxyz\n";

    while (*long_string != 0) {
      nighterm_write(*long_string++);
    }

    const uint8_t colors[26] = {0, 10, 20, 30, 40, 50, 60, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200, 210, 220, 230, 240, 255};

    int i = 0;
    while (*alphabet != 0) {
      nighterm_set_fg_color(colors[15 - i], colors[26 - i], colors[i]);
      nighterm_write(*alphabet++);
      i++;
    }

    nighterm_set_fg_color(255, 255, 255);
    i = 0;

    while (*alphabet_low != 0) {
      nighterm_set_bg_color(colors[15 - i], colors[26 - i], colors[i]);
      nighterm_write(*alphabet_low++);
      i++;
    }

    nighterm_set_bg_color(0, 0, 0);

    // We're done, just hang...
    for (;;) {
        __asm__("cli;hlt");
    }
}

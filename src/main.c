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
            NULL);
    
    nighterm_write('H');
    nighterm_write('e');
    nighterm_write('l');
    nighterm_write('l');
    nighterm_write('o');

    // We're done, just hang...
    for (;;) {
        __asm__("cli;hlt");
    }
}

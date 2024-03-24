# Nighterm EXtended Example

![Nightmare EXtended](img/screenshot_240324.png)

## Dependencies

- `make` / `gmake`
- `xorriso`
- `qemu-system-x86_64`
- `x86_64-elf-gcc` (macOS only)
- `x86_64-elf-ld` (macOS only)

## Building and Running

Running `make all` will compile the kernel (from the `kernel/` directory) and then generate a bootable ISO image.

Running `make run` will build the kernel and a bootable ISO (equivalent to make all) and then run it using `qemu` (if installed).

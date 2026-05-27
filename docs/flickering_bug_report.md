# Bug Report: Screen Flickering / Reboot Loop

**Date:** 2026-05-25  
**Symptom:** Screen rapidly alternates between real-mode bootloader text and protected-mode kernel output.  
**Root Cause:** Triple fault → CPU reset → reboot loop.

---

## What Was Happening

The screen was "flickering" — rapidly switching between showing the real-mode messages
(`"Bootsector Loaded Successfully!"`, `"Loading Kernel..."`) and the 32-bit kernel output
(MeowOS ASCII art). This isn't actually a display bug — the CPU was **rebooting in a loop**.

The cycle looked like this:

```
BIOS → Bootloader (real mode text) → Kernel (protected mode text) → CRASH → BIOS → ...
```

---

## Bugs Found (3 total)

### 1. IDT Gate Flags — Missing Present Bit

**File:** `cpu/idt.c`

```diff
- idt[n].flags = 0x0E;
+ idt[n].flags = 0x8E;
```

**Explanation:**  
The IDT gate flags byte layout is: `P | DPL (2 bits) | 0 | Type (4 bits)`

| Value  | Binary       | Present? | Meaning                              |
|--------|-------------|----------|--------------------------------------|
| `0x0E` | `0000 1110` | **No**   | Gate type correct, but marked absent |
| `0x8E` | `1000 1110` | **Yes**  | Valid 32-bit interrupt gate, DPL=0   |

With Present=0, the CPU treats **every single IDT entry as invalid**. When any interrupt
fires (hardware timer, software `int`, or a fault), the CPU can't find a handler, which
triggers the **triple fault cascade**:

```
Interrupt → No valid handler → #GP (General Protection Fault)
    → #GP handler also not present → #DF (Double Fault)
        → #DF handler also not present → TRIPLE FAULT → CPU RESET
```

**Lesson:** Always set bit 7 (Present) in IDT gate flags. `0x8E` is the standard value for
a kernel-level 32-bit interrupt gate.

---

### 2. Kernel Too Large for Loaded Sectors

**File:** `bootloader/boot_sect_main.asm`

```diff
- mov dh, 4
+ mov dh, 32
```

**Explanation:**  
The bootloader uses BIOS `int 0x13` to load the kernel from disk into memory at `0x1000`.
The `dh` register specifies how many 512-byte sectors to read.

| Sectors | Bytes Loaded | Kernel Size | Result                          |
|---------|-------------|-------------|----------------------------------|
| 4       | 2,048       | 8,320       | ❌ Only 24% of kernel loaded    |
| 32      | 16,384      | 8,320       | ✅ Full kernel + room to grow   |

With only 4 sectors loaded, the CPU was jumping into **incomplete or garbage memory** after
the first 2KB of kernel code, causing unpredictable crashes.

**Lesson:** Always check `wc -c bin/kernel.bin` after building and make sure the bootloader
loads enough sectors: `⌈kernel_size / 512⌉` sectors minimum.

---

### 3. ISR Gate Numbers All Set to 0

**File:** `cpu/isr.c`

```diff
- set_idt_gate(0, (u32)isr11);
- set_idt_gate(0, (u32)isr12);
+ set_idt_gate(11, (u32)isr11);
+ set_idt_gate(12, (u32)isr12);
  // ... same pattern through isr31
```

**Explanation:**  
ISRs 11–31 were all being installed into gate **0** instead of their correct gate numbers.
This meant:
- Gate 0 kept getting overwritten (ended up pointing to `isr31`)
- Gates 11–31 were completely empty (no handler installed)

---

### Bonus: Slow `clear_screen()` Causing Visual Jitter

**File:** `drivers/screen.c`

The original `clear_screen()` called `print_char()` for all 2000 cells (25×80), and each
`print_char` call does 4 port I/O operations via `set_cursor_offset()`. That's **8,000 port
I/O ops** just to clear the screen.

```diff
  // Before: called print_char 2000 times (8000 port I/O ops)
- print_char(i / MAX_COLS, i % MAX_COLS, ' ', WHITE_ON_BLACK);

  // After: write directly to video memory, set cursor once
+ video_address[i * 2] = ' ';
+ video_address[i * 2 + 1] = WHITE_ON_BLACK;
```

---

### Bonus: Wrong `sizeof` in IDT Limit

**File:** `cpu/idt.c`

```diff
- idt_reg.limit = IDT_ENTRIES * sizeof(idt_register_t) - 1;
+ idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
```

The limit should be calculated from the size of each **gate entry** (`idt_gate_t`, 8 bytes),
not the IDT register struct (`idt_register_t`, 6 bytes). This would tell the CPU the table
is smaller than it actually is, potentially causing entries near the end to be ignored.

---

## Key Takeaway

The "flickering" was never a display issue — it was a **triple fault reboot loop**. When
debugging OS-level display issues, first rule out whether the CPU is just crashing and
resetting. A good way to check: if you see the BIOS/bootloader text reappearing, it's a
reboot, not a rendering bug.

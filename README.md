# mini_bringup

Kleines Bare-Metal-Lernprojekt für ARM64 (AArch64): Von der ersten Instruktion nach dem Reset über UART-Ausgabe und Exception-Handling bis zu einem Timer-Interrupt, ohne Betriebssystem und ohne libc. Läuft in QEMU (`-machine virt`).

## Was drin ist

| Datei | Aufgabe |
|---|---|
| `linker/linker_script.ld` | Speicherkarte: Code ab `0x40000000` (RAM von QEMU `virt`), Sections inkl. auf 2 KiB ausgerichteter Vector Table, 16 KiB Stack |
| `src/start.s` | Startup-Code: Stack setzen, BSS nullen, `VBAR_EL1` setzen, `bl main` |
| `src/vector_table.s` | Exception Vector Table (16 Einträge). Sync-Exceptions und IRQs aus EL1 sind verdrahtet, der Rest hängt vorerst in einer Endlosschleife |
| `src/exception.c` | Handler für synchrone Exceptions: gibt `ESR_EL1`, `ELR_EL1` und `FAR_EL1` über UART aus |
| `src/exception_entry.s` | IRQ-Einsprung: Register sichern, C-Handler aufrufen, Register wiederherstellen, `eret` |
| `src/irq_handler.c` | IRQ-Handler für den Timer: IAR lesen, Tick zählen, Timer neu stellen, EOIR schreiben |
| `src/interrupt_controller.c` | GICv2-Setup |
| `src/timer.c` | ARM Generic Timer, Verzögerungen |
| `include/system_registers.h` | Zugriff auf System-Register per Inline-Assembler |
| `src/uart.c`, `include/uart.h` | UART-Treiber für den PL011 (Memory-Mapped I/O, `0x09000000`) |
| `src/main.c` | Startet den Timer-Interrupt und gibt im Sekundentakt den Tick-Zähler aus |

## Bauen und starten

Benötigt: `gcc-aarch64-linux-gnu`, `binutils-aarch64-linux-gnu`, `qemu-system-arm`, `gdb-multiarch`, `make`

```
make          # baut build/kernel.elf
make run      # startet QEMU (beenden: Ctrl-A, dann X)
make debug    # startet QEMU pausiert, wartet auf GDB (Port 1234)
make gdb      # in zweitem Terminal: verbindet GDB
make disasm   # Disassembly
make clean
```

Nach `make run` erscheint `Hello world from bare metal!`, danach gibt `main` im Sekundentakt den Tick-Zähler aus (der Timer tickt alle 100 ms).

## Debuggen mit GDB

```
(gdb) break irq_handler
(gdb) continue
(gdb) info registers esr_el1 elr_el1 spsr_el1 sp
(gdb) x/4xg &bss_begin
```

## Was ich dabei gelernt habe

- Der Linker kennt die Speicherkarte der Hardware nicht, sie steht im Linker-Script.
- C braucht einen Stack und genulltes BSS. Ohne OS muss der Startup-Code beides selbst einrichten.
- Hardware-Register brauchen `volatile` und die richtige Zugriffsbreite (hier 32 Bit).
- Fehlersuche: erst statisch (`nm`, `readelf`, `objdump`), dann Schritt für Schritt in GDB.

## Als Nächstes

- `printf` auf Basis der UART-Ausgabe
- Mini-Bootloader, der vor dem Start ein Image per SHA-256 prüft
- Portierung auf echte Hardware (z. B. Raspberry Pi mit USB-UART-Adapter)

Erstellt mit Claude als Lernbegleiter (Erklärungen und Code-Review). Den Code habe ich selbst geschrieben.
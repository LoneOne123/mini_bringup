# mini_bringup

Kleines Bare-Metal-Lernprojekt für ARM64 (AArch64): Von der ersten Instruktion nach dem Reset bis zur Textausgabe über UART, ohne Betriebssystem und ohne libc. Läuft in QEMU (`-machine virt`).

## Was drin ist

| Datei | Aufgabe |
|---|---|
| `linker/linker_script.ld` | Speicherkarte: Code ab `0x40000000` (RAM von QEMU `virt`), Sections `.text.boot`, `.text`, `.rodata`, `.data`, `.bss`, 16 KiB Stack |
| `src/start.s` | Startup-Code: Stack-Pointer setzen, BSS nullen, `bl main` |
| `src/uart.c`, `include/uart.h` | UART-Treiber für den PL011 per Memory-Mapped I/O (`0x09000000`), mit Warten auf freien TX-FIFO |
| `src/main.c` | Gibt über UART eine Meldung aus |

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

Erwartete Ausgabe:

```
Hello world from bare metal!
```

## Debuggen mit GDB

```
(gdb) break main
(gdb) continue
(gdb) info registers sp
(gdb) x/4xg &bss_begin
```

## Was ich dabei gelernt habe

- Der Linker kennt die Speicherkarte der Hardware nicht, sie steht im Linker-Script.
- C braucht einen Stack und genulltes BSS. Ohne OS muss der Startup-Code beides selbst einrichten.
- Hardware-Register brauchen `volatile` und die richtige Zugriffsbreite (hier 32 Bit).
- Fehlersuche: erst statisch (`nm`, `readelf`, `objdump`), dann Schritt für Schritt in GDB.

## Als Nächstes

- Exception-Vector-Table und Timer-Interrupt (GIC)
- `printf` auf Basis der UART-Ausgabe
- Mini-Bootloader, der vor dem Start ein Image per SHA-256 prüft
- Portierung auf echte Hardware (z. B. Raspberry Pi mit USB-UART-Adapter)

Erstellt mit Claude als Lernbegleiter (Erklärungen und Code-Review). Den Code habe ich selbst geschrieben.

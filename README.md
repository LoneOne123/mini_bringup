# mini_bringup

Kleines Bare-Metal-Lernprojekt für ARM64 (AArch64): Von der ersten Instruktion nach dem Reset über UART-Ausgabe, Exception-Handling und Timer-Interrupt bis zu einem Mini-Bootloader, der ein Programm per SHA-256 prüft, bevor er es startet. Ohne Betriebssystem und ohne libc. Läuft in QEMU (`-machine virt`).

## Aufbau

```
bootloader/   Bootloader: Start-Code, Exceptions, IRQ, main
payload/      Das Programm, das der Bootloader prüft und startet
drivers/      Hardware-Zugriff: UART, Timer, Interrupt-Controller
lib/          kprintf, SHA-256
include/      Header
```

| Datei | Aufgabe |
|---|---|
| `bootloader/linker_script.ld` | Speicherkarte: Code ab `0x40000000` (RAM von QEMU `virt`), Sections inkl. auf 2 KiB ausgerichteter Vector Table, 16 KiB Stack |
| `bootloader/start.s` | Startup-Code: Stack setzen, BSS nullen, `VBAR_EL1` setzen, `bl main` |
| `bootloader/vector_table.s` | Exception Vector Table (16 Einträge). Sync-Exceptions und IRQs aus EL1 sind verdrahtet, der Rest hängt vorerst in einer Endlosschleife |
| `bootloader/exception.c` | Handler für synchrone Exceptions: gibt `ESR_EL1`, `ELR_EL1` und `FAR_EL1` aus |
| `bootloader/exception_entry.s` | IRQ-Einsprung: Register sichern, C-Handler aufrufen, Register wiederherstellen, `eret` |
| `bootloader/irq_handler.c` | IRQ-Handler für den Timer: IAR lesen, Tick zählen, Timer neu stellen, EOIR schreiben |
| `bootloader/main.c` | Prüft die Payload (SHA-256) und springt hinein, sonst Abbruch |
| `drivers/uart.c` | UART-Treiber für den PL011 (Memory-Mapped I/O, `0x09000000`) |
| `drivers/interrupt_controller.c` | GICv2-Setup |
| `drivers/timer.c` | ARM Generic Timer, Verzögerungen |
| `lib/kprintf.c` | `printf` für Bare Metal (`%c %s %p %d %i %u %x %X %%`, `l` für 64 Bit), portiert aus meinem `ft_printf` vom 42-Curriculum |
| `lib/sha256.c` | SHA-256 nach FIPS 180-4, zum Lernen selbst geschrieben |
| `include/system_registers.h` | Zugriff auf System-Register per Inline-Assembler |
| `payload/` | Eigenes Programm mit eigenem Linker-Skript (Adresse `0x41000000`), gibt nur eine Meldung aus |

Der Timer-Interrupt (GIC, IRQ-Einsprung, Handler) ist implementiert und getestet, in der aktuellen `main` aber nicht aktiv.

## Bauen und starten

Benötigt: `gcc-aarch64-linux-gnu`, `binutils-aarch64-linux-gnu`, `qemu-system-arm`, `gdb-multiarch`, `make`

```
make              # baut build/kernel.elf (Bootloader) und build/payload.bin
make run-boot     # Bootloader startet, prüft die Payload und startet sie
make run-boot-bad # dasselbe mit einer manipulierten Payload (ein Byte geändert)
make run-payload  # die Payload allein
make run          # nur der Bootloader, ohne Payload
make debug        # QEMU pausiert, wartet auf GDB (Port 1234)
make gdb          # in zweitem Terminal: verbindet GDB
make disasm       # Disassembly (disasm-payload für die Payload)
make clean
```

QEMU beenden: Ctrl-A, dann X.

## Der Bootloader

1. QEMU legt `payload.bin` per `-device loader` an `0x41000000` in den RAM (steht für: Image aus Flash oder SD-Karte gelesen).
2. Der Bootloader berechnet den SHA-256 über genau diese Bytes im RAM.
3. Er vergleicht ihn mit dem Sollwert. Länge und Sollwert erzeugt das Makefile aus `payload.bin` und legt sie in `build/payload_info.h` ab.
4. Stimmt alles, springt er auf `0x41000000`. Sonst gibt er `Payload rejected!` aus und hält an.

`make run-boot` zeigt `Payload running at 0x41000048`, `make run-boot-bad` zeigt `Payload rejected!`.

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
- `ESR_EL1` sagt, was passiert ist (Exception Class, Details im ISS). `ELR_EL1` zeigt bei `svc` auf den Befehl danach, bei `brk` und Data Abort auf den Befehl selbst.
- Endianness spielt nur dort eine Rolle, wo Bytes und Wörter ineinander umgewandelt werden (bei SHA-256 Ein- und Ausgabe). Mit Schieben statt Zeiger-Cast ist der Code auf jeder CPU richtig.
- Mein `ft_printf` hat `va_list` als Wert weitergegeben. Auf x86-64 geht das, auf AArch64 ist `va_list` eine Struktur und wird kopiert, jedes Argument kam dann falsch an.
- Fehlersuche: erst statisch (`nm`, `readelf`, `objdump`), dann Schritt für Schritt in GDB.

Erstellt mit Claude als Lernbegleiter (Erklärungen und Code-Review). Den Code habe ich selbst geschrieben.
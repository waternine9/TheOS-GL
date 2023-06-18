[BITS 16]
section .boot


Boot:
    

    ; NOTE: At boot the boot drive number is stored in DL,
    ;       Preserve it for later 
    mov   [DriveNumber], dl

    xor ax, ax
    mov ss, ax
    mov ds, ax
    mov ax, 0x7C00
    mov sp, ax

    ; NOTE: SETUP VBE
    jmp SetupVbe
    %include "src/bootloader/vesa_vbe_setup.asm"
SetupVbe:
    call VesaVbeSetup

    ; NOTE: Activate A20
    mov   ax, 0x2403
    int   0x15

    ; NOTE: Load GDT and activate protected mode
    cli
    lgdt  [GDTDesc]
    mov   eax, cr0
    or    eax, 1
    mov   cr0, eax



    jmp   8:After

global DriveNumber
DriveNumber: db 0

 GDTStart:
     dq 0
 GDTCode:
     dw 0xFFFF     ; Limit
     dw 0x0000     ; Base
     db 0x00       ; Base
     db 0b10011010 ; Access
     db 0b11001111 ; Flags + Limit
     db 0x00       ; Base
 GDTData:
     dw 0xFFFF     ; Limit
     dw 0x0000     ; Base
     db 0x00       ; Base
     db 0b10010010 ; Access
     db 0b11001111 ; Flags + Limit
     db 0x00       ; Base
 GDTEnd:

 GDTDesc:
     .GDTSize dw GDTEnd - GDTStart - 1 ; GDT size
     .GDTAddr dd GDTStart          ; GDT address

[BITS 32]

After:

    ; NOTE: Setup segments.
    mov   ax, 16
    mov   ds, ax
    mov   es, ax
    mov   fs, ax
    mov   gs, ax
    mov   ss, ax

    mov eax, 0x1000000
    mov esp, eax

    mov edi, unkernel_end
    mov ecx, 2

.read_loop:
    mov dl, [DriveNumber]
    push edi
    push ecx
    call read_ata
    pop ecx
    pop edi

    inc ecx
    add edi, 512
    cmp edi, OsEnd
    jl .read_loop

    ; Use scancode set 1 (PS/2 keyboard)
    mov dx, 0x60
    mov al, 0xF0
    out dx, al
    mov al, 1
    out dx, al

    jmp unkernel_end



%include "src/bootloader/ata/ata.asm"

times 510-($-$$) db 0
dw 0xAA55

unkernel_end:

; Find RSDP
    mov ecx, 0x2000
    mov esi, 0xE000
    RSDPfind:
    mov es, si
    mov eax, [es:0]
    cmp eax, "RSD "
    jne .NotFound
    mov eax, [es:4]
    cmp eax, "PTR "
    je .Found
    .NotFound:
    inc esi
    loop RSDPfind
    mov eax, 0xEEEEEE
    cli
    hlt
    .Found:
    mov edi, rsdp
    mov esi, 0
    mov ecx, 24
    .Copy:
    mov eax, [es:esi]
    mov [edi], eax
    inc esi
    inc edi
    loop .Copy

    xor ax, ax
    mov es, ax

extern kmain
cli
call kmain
cli
hlt

rsdp:
.Signature times 8 db 0
.Checksum db 0
.OEMID times 6 db 0
.Revision db 0
.Rsdt dd 0

global rsdp

%include "src/bootloader/irq_handlers.asm"
%include "src/bootloader/vesa_vbe_setup_vars.asm"

section .text

section .end
OsEnd:

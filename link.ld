OUTPUT_FORMAT(binary)
ENTRY(kmain)
SECTIONS {
    . = 0x7C00;
    
    .boot :
    {
        *(.boot)
        *(.text)  
        *(.glyphs)
        *(.images) 
    }
    .rodata : 
    {
        *(.rodata)
    }
    .data : 
    {
        *(.data)
    }
    .end :
    {
        *(.end)
    }
    . = 0x1000000 + 0x7C00;
    .bss : 
    {
        *(.bss)
    }
    
    
}

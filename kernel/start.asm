
;;; kernel entry point

		[BITS 32]
		global _start
		global start
_start:
start:	
		mov esp, _sys_stack 	; This points the stack to our new stack area
		jmp stublet

;;; This part MUST be 4byte aligned, so we tell the assembler this
ALIGN 4
mboot:
		;; multiboot macros to make a few lines later more readable
		MULTIBOOT_PAGE_ALIGN 	equ 1<<0
		MULTIBOOT_MEMORY_INFO	equ 1<<1
		MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
		MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO 
		MULTIBOOT_CHECKSUM		equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
		EXTERN code, bss, end

		;; This is the GRUB multiboot header, a boot signature
		dd MULTIBOOT_HEADER_MAGIC
		dd MULTIBOOT_HEADER_FLAGS
		dd MULTIBOOT_CHECKSUM

		;; An endless look
stublet:
		extern main
		push ebx
		call main
		jmp $

;;; Load the GDT, defined in gdt.c
		global gdt_flush		
		extern gp
gdt_flush:
		lgdt [gp]
		mov ax, 0x10 			; 0x10 is the offset of the data in our gdt table
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax
		jmp 0x08:flush2			; 0x08 is the offset to our code segment, need a far jump
flush2:
		ret						; return to the C code 

;;; load the IDT table, defined in idt.c
		global idt_load
		extern idtp
idt_load:
		lidt [idtp]
		ret

;;; ISR stuff
		global isr0
		global isr1
		global isr2
		global isr3
		global isr4
		global isr5
		global isr6
		global isr7
		global isr8
		global isr9
		global isr10
		global isr11
		global isr12
		global isr13
		global isr14
		global isr15
		global isr16
		global isr17
		global isr18
		global isr19
		global isr20
		global isr21
		global isr22
		global isr23
		global isr24
		global isr25
		global isr26
		global isr27
		global isr28
		global isr29
		global isr30
		global isr31

		;;  division by zero interupt
isr0:
		cli
		push byte 0
		push byte 0
		jmp isr_common_stub

		;; debug exception
isr1:	
		cli
		push byte 0
		push byte 1
		jmp isr_common_stub

		;; non maskable interrupt exception
isr2:	
		cli
		push byte 0
		push byte 2
		jmp isr_common_stub

		;; breakpoint exception
isr3:	
		cli
		push byte 0
		push byte 3
		jmp isr_common_stub
		
		;; into detected overflow exception
isr4:	
		cli
		push byte 0
		push byte 4
		jmp isr_common_stub

		;; Out of Bounds Exception
isr5:	
		cli
		push byte 0
		push byte 5
		jmp isr_common_stub

		;; Invalid Opcode Exception
isr6:	
		cli
		push byte 0
		push byte 6
		jmp isr_common_stub

		
		;; No Coprocessor Exception
isr7:	
		cli
		push byte 0
		push byte 7
		jmp isr_common_stub
	
isr8:
		cli
		push byte 8
		;; note that we don't push a value onto the stack here, it has already pushed one
		jmp isr_common_stub

		;; Coprocessor Segment Overrun Exception
isr9:	
		cli
		push byte 0
		push byte 9
		jmp isr_common_stub

		;; Bad TSS Exception
isr10:
		cli
		push byte 10
		jmp isr_common_stub

		;; Segment Not Present Exception
isr11:
		cli
		push byte 11
		jmp isr_common_stub

		;; Stack Fault Exception
isr12:
		cli
		push byte 12
		jmp isr_common_stub

		;; General Protection Fault Exception
isr13:
		cli
		push byte 13
		jmp isr_common_stub

		;; Page Fault Exception
isr14:
		cli
		push byte 14
		jmp isr_common_stub

		;; Unknown Interrupt Exception
isr15:	
		cli
		push byte 0
		push byte 15
		jmp isr_common_stub

		;; Coprocessor Fault Exception
isr16:	
		cli
		push byte 0
		push byte 16
		jmp isr_common_stub
		
		;; Alignment Check Exception (486+)
isr17:	
		cli
		push byte 0
		push byte 17
		jmp isr_common_stub

		;; Machine Check Exception (Pentium/586+)
isr18:	
		cli
		push byte 0
		push byte 18
		jmp isr_common_stub

		;; Reserved exception
isr19:	
		cli
		push byte 0
		push byte 19
		jmp isr_common_stub

		;; Reserved exception
isr20:	
		cli
		push byte 0
		push byte 20
		jmp isr_common_stub

		;; Reserved exception
isr21:	
		cli
		push byte 0
		push byte 21
		jmp isr_common_stub

		;; Reserved exception
isr22:	
		cli
		push byte 0
		push byte 22
		jmp isr_common_stub

		;; Reserved exception
isr23:	
		cli
		push byte 0
		push byte 23
		jmp isr_common_stub

		;; Reserved exception
isr24:	
		cli
		push byte 0
		push byte 24
		jmp isr_common_stub

		;; Reserved exception
isr25:	
		cli
		push byte 0
		push byte 25
		jmp isr_common_stub

		;; Reserved exception
isr26:	
		cli
		push byte 0
		push byte 26
		jmp isr_common_stub
		
		;; Reserved exception
isr27:	
		cli
		push byte 0
		push byte 27
		jmp isr_common_stub

		;; Reserved exception
isr28:	
		cli
		push byte 0
		push byte 28
		jmp isr_common_stub

		;; Reserved exception
isr29:	
		cli
		push byte 0
		push byte 29
		jmp isr_common_stub
		
		;; Reserved exception
isr30:	
		cli
		push byte 0
		push byte 30
		jmp isr_common_stub

		;; Reserved exception
isr31:	
		cli
		push byte 0
		push byte 31
		jmp isr_common_stub

		
		extern fault_handler		
		
isr_common_stub:
		pusha
		push ds
		push es
		push fs
		push gs
		mov ax, 0x10 			; load kernel data segment 
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov eax, esp			; push the stack
		push eax
		mov eax, fault_handler
		call eax				; special call, preserved eip register
		pop eax
		pop gs
		pop fs
		pop es
		pop ds
		popa
		add esp, 8				; cleans up the pushed error code and pushed ISR error code
		sti
		iret 					; pops 5 things at once, CS, EIP, EFLAGS, SS, and ESP!
		
		
		
;;; ISQ stuff
		global irq0
		global irq1
		global irq2
		global irq3
		global irq4
		global irq5
		global irq6
		global irq7
		global irq8
		global irq9
		global irq10
		global irq11
		global irq12
		global irq13
		global irq14
		global irq15

irq0:
		cli
		push byte 0
		push byte 32
		jmp irq_common_stub
		
irq1:
		cli
		push byte 0
		push byte 33
		jmp irq_common_stub

irq2:
		cli
		push byte 0
		push byte 34
		jmp irq_common_stub
		
irq3:
		cli
		push byte 0
		push byte 35
		jmp irq_common_stub
		
irq4:
		cli
		push byte 0
		push byte 36
		jmp irq_common_stub

irq5:
		cli
		push byte 0
		push byte 37
		jmp irq_common_stub

irq6:
		cli
		push byte 0
		push byte 38
		jmp irq_common_stub

irq7:
		cli
		push byte 0
		push byte 39
		jmp irq_common_stub

irq8:
		cli
		push byte 0
		push byte 40
		jmp irq_common_stub

irq9:
		cli
		push byte 0
		push byte 41
		jmp irq_common_stub

irq10:
		cli
		push byte 0
		push byte 42
		jmp irq_common_stub

irq11:
		cli
		push byte 0
		push byte 43
		jmp irq_common_stub

irq12:
		cli
		push byte 0
		push byte 44
		jmp irq_common_stub

irq13:
		cli
		push byte 0
		push byte 45
		jmp irq_common_stub

irq14:
		cli
		push byte 0
		push byte 46
		jmp irq_common_stub

irq15:
		cli
		push byte 0
		push byte 47
		jmp irq_common_stub

		
		extern irq_handler

irq_common_stub:
		pusha
		push ds
		push es
		push fs
		push gs
		mov ax, 0x10
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov eax, esp
		push eax
		mov eax, irq_handler
		call eax
		pop eax
		pop gs
		pop fs
		pop es
		pop ds
		popa
		add esp, 8
		iret

;;; Definition of the BSS section
		SECTION .bss
		resb 8192				; reserves 8 Kbytes of memory here
_sys_stack:
		
		
		
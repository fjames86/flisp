FLISP
=====

A simple Scheme-like Lisp interpreter that can also be run as a simple self-contained operating system.

Features the following:

* Scheme-like semantics (so it's a Lisp-1)

* Robust move-and-copy garbage collector

* User-space flisp can load code from files. This is currently not implemented in the OS version because the flisp kernel lacks
any code for accessing hardware storage or filesystems in general.

* A Common Lisp-like macro system. 

* Simple error reporting system

* Built-in reader macros: ' (quote), ` (quasiquote) , (unquote) and @ (unquote-splicing).
User-defined reader-macros are not currently supported.

* Standard operators are: define, defmacro, set!, quote, quasiquote, unquote, unquote-splicing,
begin, if, let, lambda

* A reasonably complete set of built-in procedures written in C. These are bundled with library
and get loaded into the image.

* First class data types: (signed fixed precision) integer, double, arrays, hash tables, symbols,
strings, procedures and closures (lambda expressions).

* Unfortunately tail-call optimisation is currently not implemented (and probably won't be either ... so
don't try to write deeply recursive functions).

* The flisp-os kernel is subtly broken in several ways...

Build instructions
-------------------

The Makefile provided builds three different things: the flisp library, libflisp.a which is used to
compile the user-space interpreter and the OS-version. Because building the kernel requires loopback devices,
it must typically be built with superuser priviliges.

Builds using gcc and nasm for the assembly language in the kernel.

Note: older version of nasm do not generate working object files (it leaves global symbols undefined).
It is recommended to build using nasm version 2.10.

Usage guide
------------

flisp launches into a REPL. Use (quit) to exit.

* Define constants using (define name expr)

* Write procesures using (define (proc args...) body). Note that this is equivalent to (define proc (lambda (args...) body))

* Define macros using (defmacro (mac args...) body)
Macros are just procedures that return code. They are defined internally as (MACRO <procedure>) so this is really just a shorthand for
(define macro (list 'MACRO (lambda (args ...) body)))

* A variety of built-in procedures come with flisp. Use (toplevel) to inspect currently defined toplevel symbols.

* Memory management is split into the heap, that manages all dynamic objects (cons cells, strings etc) and a seperate,
non-garbage collected, table for symbols. When this table is exhausted you are out of luck. flisp is unable to request extra
memory and therefore the table cannot be expanded at runtime, meaning no more symbols may be interned. It allocates 1MB of
memory by default, which should be easily enough for most uses, however.
Use the procedure (heap) to inspect memory usage.


Flisp as an operating system
------------------------------

Flisp comes with a simple kernel, inspired and largely taken from Bran's Kernel Tutorial, http://www.osdever.net/bkerndev/Docs/gettingstarted.htm

The build script compiles the kernel and generates a floppy disk image with the grub bootloader installed. You can test it out using, e.g.,

qemu -fda floppy.img

Feel free to try out using virtualization software like VMWare, or even running it directly on hardware!

A word of warning: parts of the kernel are not really complete and really could do with more work. The keyboard driver
is partially broken as it doesn't handle capslock and other control keys propertly. If any exception fires it goes into a kernel
panic and a reboot is required. This shouldn't be a major issue because it enters an flisp REPL almost straight away and
doesn't allow any user binary code to be executed at all so there's no danger of protection or paging faults occuring for instance.


Developer's guide
------------------

Flisp is pretty simple in design and can be easily extended if you so wish. This is a broad outline of its implementation.

* First class types are defined in types.h. To add a new first class type, add a new data structure and enum here.
Add the required functions for memory management to gc.c

* Memory management is handled by a simple copy-and-move garbage collector, defined in gc.c
All types have a gc_new_<type> to create a new default object. Add a clause to the switch statement in gc_new_copy() to
copy an object. If the type references other objects then it will need special treatment during a gc cycle. Write a gc_relocate_<type>
function for your type. See the examples for lists, arrays and hash tables for inspiration. Finally, add a clause to the
switch statement in gc_relocate() to dispatch to your custom relocation function for your new type.
The garbage collector now knows about your new type and you can use it wherever you wish! Add some functions to manipulate your
new type and add them as built-in procedures to flisp (see below).

* Built-in procedures are in the file procs.c. These take a single parameter, a list of arguments given to the procedure.
They must unpack and check for errors themselves. For instance, if you write a function that MUST take 2 numbers as arguments then
it is up to you to check this.
To install a new procedure is pretty simple. Just write it as above, add the definition to procs.h and a line in env_init in env.c
(just copy what's already there). 

* Lisp code can be added to the files in lisp/ These are concatenated and bundled into the binary image by the build script
and get loaded by flisp and flisp-os. Feel free to define more lisp here if you want to.

* Errors are handled very simply. Flisp just keeps a list of error messages and reports them at the repl. To signal an error in C
code, just call: error ("my error message", "function location"). In Lisp, use (error "my message" "location")

* New special forms can be added too. Just add a new clause to eval_expr in eval.c with your code. This shouldn't really be needed
in most cases because flisp has a robust Lisp macro system. There could be cases for it though, so feel free to have a go if you can
think of a reason!


Some things that might be useful to add:
Custom data structures, tail call optimisation, math functions (see libflisp/libs/ for some example code), reader macros, fixing the kernel keyboard driver, ...

Closing notes
----------------

This is a hobby project and was written over a couple of weeks for fun and as a learning exercise.
It is intended for educational more than serious use. I'd be interested if anyone is using it for any reason at all, so feel free to email any
questions or queries concerning this little project.

Frank James, (frank.a.james AT gmail.com)
August 2013


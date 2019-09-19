class: center, middle, inverse

# The **Spirit** of C++

### Sundaram Ramaswamy

---

class: center, middle, inverse

## Within C++, there is a much **smaller** and **cleaner** language struggling to get out.
### Bjarne Stoustrup

It’s easy to get lost in the details; don’t.

**Look for intuitions and at the big picture.**

---
.left-column[
  ## Why Use C++?
  #### Why not Python, JavaScript, …?
]

.right-column[
Why go through …

- crude data types
- raw _pointers_, contorted _references_
- manual memory management

.pull-left[
… _just be cool_ and use Python ➜
]

.pull-right[
```python
print(2 ** 63) # 2⁶³
print(2 ** 80) # 2⁸⁰

> 9223372036854775808
> 1208925819614629174706176
```
]

How C++ fares against Python here?

``` c++
int      x1 = pow(2, 63); // GCC: overflow
unsigned x2 = pow(2, 63); // GCC: overflow
uint64_t x3 = 1   << 63;  // GCC: shift count >= type width
*uint64_t x4 = 1ul << 63;  // finally!
//  2⁸⁰?  Hmmm... all in good time!
```

.pull-left[
**Results with [GCC](http://coliru.stacked-crooked.com/a/506ad2b09f29373d)**
```
2147483647
4294967295
0
9223372036854775808
```
]

.pull-right[

**Results with [Clang](http://coliru.stacked-crooked.com/a/1a1d7903eaa1024b)**
```
1703138392
0
0
9223372036854775808
```
]

]

---

class: center, middle, inverse

So many retries! Aargh!! 🤦

# **Intuitiveness**: Epic fail!

Output varies for same program between compilers!?! Oh mama! 😲

---
class: center, inverse
name: ipow2-1

# Let’s write an `ipow2()`

---
template: ipow2-1
name: ipow2-2

<br />
## Pop quiz: **What’s the size of `int`?**

---
template: ipow2-2

<br />
## Answer: **No fixed size!**

Ask your compiler: `sizeof(int)`; _never assume_ 🤔

---

## 1. **Built-in** Data Types

.pull-left[
### Integral Types.red[¹]

* `char` .little[(≥ 8-bits == 1 byte)].red[2]
* `short` .little[(≥ 16-bits)]
* `int` .little[(≥ 16-bits)]
* `long` .little[(≥ 32-bits)]
* `long long`  .little[(≥ 64-bits)]
]

.pull-right[
### Floating-point Types

* `float` .little[(32-bit)].red[3]
* `double` .little[(64-bit)].red[3]
* `long double` .little[(usually 80, 96 or 128 bits)]

.little[
_Assuming anything more than below rule is [risky](https://godbolt.org/z/UW0rCc)_!]
]

**`sizeof(char) ≤ sizeof(short) ≤ sizeof(int) ≤ sizeof(long) ≤ sizeof(long long)`**<br />

.left[### Other Types]
- `void`
- `bool`
- Pointer types e.g. `int*`, `unsigned char**`, `bool*`<br />
.little[Arithmetic based on pointed-to type e.g. `int *p; ++p;` moves `p` by `sizeof(int)`]
- Array types e.g. `int[2], char[6][5]`

.footnote[.red[¹]: variants of `unsigned` and `signed` ([two’s complement](https://en.wikipedia.org/wiki/Two%27s_complement))]
.footnote[.red[²]: A byte needn’t be 8 bits; it’s whatever `char`’s bit width is 😳]
.footnote[.red[³]: Not guaranteed to be, but mostly, IEEE-754 floats]

---

## C++ Standard .little[[(ISO/IEC 14882:2017)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf)]

* Many modern language users are at the mercy of one implementation

* **The ISO C++ standard guards C++ programmers with certain guarantees** <br />.little[A contract between language users and compiler writers ] 🤝

* **Programs adhering to the standard are always portable and work** <br />
.little[e.g. Compile a 20-year old program `g++ -std=c++98 old.cpp` _even today on any platform_ with a compiler; it works] <br />

* Standard precisely defines many aspects of a program: **well-defined** ← this is 🏠
* Standard loosely defines some aspects .little[(_Implementation-defined_,  _Unspecified_ and [_Undefined behaviour_](https://stackoverflow.com/q/2397984/183120))] ☠
.little[
- for exotic architectures having C++ compilers <br /> .little[e.g. [Unisys Servers with 9-bit bytes and 36-bit ints](https://stackoverflow.com/a/6972551/183120) programmable in C and C++ (not Python or JS — _sorry!_)]
- for freedom to compiler-authors .little[different compilers, varying implementations: a healthy competition]
]

> _But it works on my machine!?_
>
> **Say, you survived a wrong side drive on a highway at midnight once, would you argue its repeatable/correct?**

???
e.g. array access out of bounds, accessing a zombie object, null pointer dereference and many more!


---

``` c++
#include <cstdint>   // ← fixed-width integer types
#include <limits>    // ← query type limits from compiler
#include <optional>
#include <iostream>

std::optional<uintmax_t> ipow2(unsigned pow) {
  // Future-proof by not limiting to uint64_t.
  // Obtain size from compiler at compile-time; thanks to static typing
  if (pow >= std::numeric_limits<uintmax_t>::digits)
    return {};

  uintmax_t value = 1;
  value <<= pow;
  return value;
}

int main(int argc, char** argv) {
  if (argc >= 2) {
    auto const pow = std::strtoul(argv[1], nullptr /*str_end*/, 10 /*base*/);
    auto const result = ipow2(pow);
    if (result)
      std::cout << *result << '\n';
    else
      std::cout << "Power beyond machine limits!\n";
  }
}
```

* **Use [fixed-width integers](https://devdocs.io/c/types/integer)**:  `uint8_t`, `int_fast16_t`, `int32_t`, `uintptr_t`, …
.little[- `unsigned`: beware of wrap around behaviour; decrement with extreme care e.g. `uint8_t x = 0; --x; // x is now 255`]
* Use `short`, `int`, `long`, etc. when you’re _sure_ minimum (guaranteed) width is enough <br />
  .little[- Chromium has `int`s but use only a few compilers (all having 32-bit `int`) and [target only `i686`, `x86_64` and `ARM32` builds](https://www.chromium.org/chromium-os/how-tos-and-troubleshooting/chromiumos-architecture-porting-guide) ]

---

class: center, middle, inverse

### Yeah.. yeah!

# What about **2⁸⁰** ?


---

## 2. Free-standing **Function**s

``` c++
// Simple, complete program: no classes, libraries or includes.      +-------------+
int add(int x, int y) {            //     +-------------+            |    free     |
  return x + y;                    //     |    free     |            |             |
}                                  //  S  |             | ←add()    +-------------+
                                   //  T  |             |            | int, int    |
int main(int argc, char** argv) {  //  A  +-------------+   add()→  +-------------+
  int a = 1, b = -4;               //  C  | int, char** |            | int, char** |
  return add(a, b);                //  K  | int, int    | ← main → | int, int    |
}                                  //     +-------------+            +-------------+
```

- **Compile-time**: types, qualifiers, functions, structs, classes, templates, etc. exist.
- **Run-time**: Oodles of binary code your machine loves to gobble!

> C++ data types **inherit nothing** — both in-built and custom.  No compiler-supplied base `Object` under the hood.

* Languages compiling to byte code, keep type information.red[1] at run-time too.<br />
.little[VM needs them for _reflection_, _garbage collection_, _JIT optimizations_, …]
* C++ strips them and spits plain assembly. <br />
.little[[Data and code vanish into zeros and ones](https://godbolt.org/z/YZCG8u).  _Raw binary_, just as advertised 👍]

.footnote[.red[¹]: Commonly called _boxed datatypes_ e.g. `Integer` inheriting `Java.lang.Object` wraps the actual integer. C++’s `int` is machine integer.]

---

## 3. Types, Variables and **Objects**

.pull-left[
* C++’s static type system
.little[
- Expects all types, variables known at compile-time
- Key to compile-time error detection and performance]
* Type = _(Size, Operations)_.little[- e.g. `int` loads `sizeof(int)` bytes to an integer register; allows binary and arithmetic operations.]
* **Object = _(Type, Memory) → Value_**
.little[
- Allocation + Initialization
- Value interpretation based on type]
* Other languages hide locations
.little[
- But every variable is a pointer! e.g. CPython internally uses [`PyObject*` for _every_ variable](https://stackoverflow.com/a/57380719/183120)]
]

.pull-right[
* Most languages treat variable as stickers; labels stuck on objects
.little[- Types associated with objects (at runtime too)
- Variables = handle to be (re)stuck on an object]

```python
> i = 0     # type ∉ variable
> i = "hi"  # no error on reseat
```

* Variable = _(Type, Location, Value)_<br />
.little[  - Born, live, die with same type and location]

```c++
int a = 1;
float a = 0.5;  // redefinition error
```
]

### **Memory model**
<!-- Great ASCII art reference -->
<!-- http://xahlee.info/comp/unicode_ascii_art.html -->
```c
char c = 0xde;      // value: 0xde (c), loc = 1000 (&c), type: char (gone at runtime)
int a = 3203338898;                     // allocate sizeof(int) bytes, initilize to 1
short *b = reinterpret_cast<short*>(&a);     // *b = 0x1292 (on a big-endian machine)
char *p = &c;                        ╭━━━━━━━━━━━━━━━━━━┈┈┈┈┈
                                     ▼
 …   1000   1001   1002   …    ╭━ short ━╮╭━━━━ char* ━━━━╮
----------------------------------------------------------------------------
.. | 0xde | 0xad | 0xbe | 0xef | 0x12 | 0x92 | 0x00 | 0x00 | 0x10 | 0x00 | ..
----------------------------------------------------------------------------
      ▲         ╰━━━━━ int ━━━━━╯                ┃
      ╰━━━━━━━━━━━━━━━━━━━━━━━━━━━╯
```

---

## 4. **Memory**

.pull-left[
### Stack
- Automatic allocation and deallocation
.little[- Alloc/dealloc order: lexical/reverse lexical
]
- Much faster than heap
.little[
- Allocation is a mere pointer move
- Spatial and temporal coherence
]
- Limited scope
.little[
- Alive within current and deeper functions
- Can’t return local variable _address_
]
- Limited size (configurable)
.little[
- Default MiB/thread: 2 ([GCC](https://stackoverflow.com/a/32543529/183120)), 1 ([MSVC](https://docs.microsoft.com/en-us/cpp/build/reference/f-set-stack-size?view=vs-2019))
]
]

.pull-right[
### Heap
- Manual allocation and deallocation
.little[- Manual memory management is a _land mine_ 💥
- Over 40 years of experience proves humans are bad at it; use smart pointers `make_unique`, `make_shared`
- Never even write `new`, `malloc`, `CoTaskMemAlloc`, …
- Know: `delete ≠ delete []`, `delete ≠ free()`
]
- Alloc/dealloc involves OS calls (slow)
.little[- Virtual memory, book keeping, correct CRT]
- No scope; alive until manually freed
- Practically no limit
.little[([32-bit: 3 GiB, 64-bit: 16 EiB](https://softwareengineering.stackexchange.com/a/207390/4154))]
]

.left[> **Stack-allocate** if size known at compile-time, within scope and within size limit;  **heap-allocate** otherwise.]

``` c++
void getItemCount(int* count);
// GOOD                // BAD                 // UGLY
int c;                 int* c = new int;      unique_ptr<int> c = make_unique<int>();
getItemCount(&c);      getItemCount(c);       getItemCount(c.get());
                       delete c;
```

???
- Stack size is OS-dependant too: 8 MiB on macOS
- Know: `delete` between runtimes are unequal!

---

## 5. **User-defined** Types

``` c++
// 1. Using built-in
typedef int Id;
using GroupId = float;
enum : uint8_t Weekends { Sat, Sun };

// 2. Composite
struct Point { float x; float y; };
class Canvas {                      //  Canvas
  Point origin;                     //  +-------+-------+-------+
  int max_memory;                   //  | float | float |  int  |
  void* data;                       //  +-------+-------+-------+

public:
  Canvas(float max);
};                        //       /<--- c[0]  --->\ /<---- c[1] --->\
                          //   ---+-----+-----+-----+-----+-----+-----+---
// 3. Aggregate           //      |float|float| int |float|float| int |
int vals[3] = {0, 1, 2};  //   ---+-----+-----+-----+-----+-----+-----+---
Canvas c[2] = { Canvas(1024), Canvas(512) };
```

* **When authoring a class data is key**
  - Methods are useful only when using a class
* Decide what you need and keep it minimal

---

## Hardware vs Software

* Hardware vendors make hardware dedicated for some algorithms
  - .tag[Performance] **Using dedicated hardware is a lot faster**
  - E.g. [GPU](http://en.wikipedia.org/wiki/GPU)s for 3D data and pixels, hardware decoders for MP3, etc.
  - .tag[Hardware] SIMD instruction sets to perform multiple operations in one CPU cycle.  With C++ you can access them; [live example](http://coliru.stacked-crooked.com/a/256d134083aa6118) of 4-vector addition using [SSE2](https://en.wikipedia.org/wiki/SSE2)
* When such hardware is non-existent, software implementation is provided as a fallback
  - However many languages project nothing more than a simple model of x86; some don’t even do that!
--------------------------------------------------------------------------------
* Python’s `int` has arbitrary-precision because internally [**all** integers are big ints](https://rushter.com/blog/python-integer-implementation/); software emulation as hardware natively can’t.
  - Every programmer is penalized for greater flexibility
  - Programmer is never exposed to system’s native `int`, so that expressions like this work: `2 ** 80 = 1208925819614629174706176`
* C++ doesn’t provide it since your machine doesn’t have one!
  - Most machines can natively only do upto 64-bit math since they’re 64-bit CPUs
  - .tag[Flexibility] Programmer needing _big int_ can always include a library or write a custom implementation e.g. [GNU Multiple Precision](https://gmplib.org/) library is one of the fastest

---

## **Critical** vs **Non-Critical** Software

* **Turn-around vs throughput**: different settings, different expectations
    - Example: Ordering food at a restaurant vs ordering for home delivery 🍜<br />
        - Wasting time on _avoidable_, petty procedures means an overall delay in eating
        - Stopping 5 mins for a smoke is OK when it’d be 30 mins restaurant ➜ home
* Time-wise non-critical software examples
  - Server-side code looking up voluminous database — .little[**Overarching bottleneck**: disk access, network]
  - Content generators e.g. Doxygen — .little[**Output quality matters but not time-taken**]

* **Non-critical software**: When **bottleneck is elsewhere**, optimizing every instruction or data is **pessimization**

> Shaving off a few (unnoticeable) microseconds when overall lag would be in seconds is _**pessimisation**._

* **Critical software**: No such luxury; picoseconds and bytes matter
  - Very small lags every operation leads to a sluggish system
  - You can’t put a finger on it but system feels slow

---

class: left, inverse
name: knuth-1

## .center[**Use the Right Tool for the Right Job**]

---

template: knuth-1
name: knuth-2

<br />

.little[### We should forget about small efficiencies, say about 97% of the time: premature optimization is the root of all evil.]

---

template: knuth-2
name: knuth-3

.little[##Yet **we should not pass up our opportunities in that critical 3%**.
### .right[— Donald Knuth / Tony Hoare]]

---

template: knuth-3

#### C++ is keen about _that_ 3% — as a language community.

We mean to _eek out the very last drop of juice a CPU’s got!_

???

We’ll discuss sane defaults when writing C++ software — just good habits, not pessimizations.

---

## Languages for **non-critical** software <small>(C#, Python, JS, …)</small>

* .tag[Performance] Most run on a VM.red[1] with garbage collection.

* .tag[Flexibility] Sacrifices finer control for more features.<br />
.little[e.g. reflection, garbage collection, rich built-in types like _lists, dictionaries/maps, big integers_, … ]

* .tag[Hardware] Abstracts machine away as much as possible.<br />
.little[Assumes programmers doesn’t know hardware; relieves programmer from worrying about hardware intricacies]

## Languages for **critical** software <small>(C++, C, Rust, …)</small>

* .tag[Performance] _Zero-overhead abstractions_.red[2]; as close to hardware as possible but not closer.<br />
.little[Easy to reason about the machine code generated for your program.]

* .tag[Flexibility] You choose what you want. _You only pay for what you use_.red[3].<br />
.little[Programmer, not the language, is in charge. Believes programmer knows what s/he doing.]

* .tag[Hardware] _Low-level access_ enables authoring kernel, virtual machines, …<br />
.little[Direct access to CPU/GPU/OS facilities, _yum_!  No VM, no GC  _no middleman — no comission_.]

<div>
.footnote[.red[¹]: Not system virtual machine (like VMWare, Qemu, VirtualBox, …), but **[process virutal machine](https://en.wikipedia.org/wiki/Virtual_machine#Process_virtual_machines)** (like JVM, CLR, Python interpreter, …)]

.footnote[.red[²]: `std::vector` or `std::map` should perform _almost_ like a vector or red-black tree hand-coded in assembly]

.footnote[.red[³]: A key design guideline of the C++ standards committee; another is _portability_.]
</div>

---

# Browser is **system software**

It’s complex and critical; comparable to a

.pull-left[
* Kernel

* Game engine

* Compiler / virtual machine

* Real-time financial trading system
]

.pull-right[
> Wasting a small time delta every operation = **systemic lag** hurting productivity.
]

Ever wondered what language virtual machines are written in?

* [C#](https://stackoverflow.com/q/1324919/183120)
* [Java](https://stackoverflow.com/a/10028154/183120)
* [Python](https://en.wikipedia.org/wiki/CPython)
* JavaScript
  - Mozilla’s [SpiderMonkey](https://developer.mozilla.org/en-US/docs/Mozilla/Projects/SpiderMonkey)
  - Google’s [V8](https://v8.dev/)

_The Need for Speed_ justifies writing browsers in C++ 🏁

---

class: center, middle, inverse

## That makes you a
# **System Programmer**
### Take pride — there aren’t many.

---

# 2. **Methods**

Functions with a hidden first argument for object pointer:

``` c++
struct Widget {
  void Scroll(int clicks) { y_ += (clicks * OFFSET); }

private:
  constexpr static int OFFSET = 3;

  float x_, y_;
  int scale_;
};
```

Equivalent to

``` c++
// a simple struct with just data; called POD (plain-old data) at times
struct __Widget {    // object has only the data (member variables)
  float x_, y_;
  int scale_;
};

// code elsewhere (.text segment)
void __Widget_Scroll(__Widget* w, int clicks) {
  w->y_ += (clicks * 3);  // OFFSET resolved at compiled-time
}
```

---

# What are little objects made of?

Show what’s a variable made of: raw memory interpreted by CPU based on type.
Show the stack and heap; show the stack growing and explain.

``` c++
            Widget w1              Widget w2              Widget w2
       +-------------------+  +-------------------+  +-------------------+
       |                   |  |                   |  |                   |
       |    float x_       |  |    float x_       |  |    float x_       |
       |                   |  |                   |  |                   |
       +-------------------+  +-------------------+  +-------------------+
       |                   |  |                   |  |                   |
       |    float y_       |  |    float y_       |  |    float y_       |
       |                   |  |                   |  |                   |
       +-------------------+  +-------------------+  +-------------------+
       |                   |  |                   |  |                   |
       |    int scale_     |  |    int scale_     |  |    int scale_     |
       |                   |  |                   |  |                   |
       +-------------------+  +-------------------+  +-------------------+

```

* Only instance variables make up an object; use `sizeof(Widget)` to check size

* No code is within the object

* Static variables aren’t part of the object

* `Widget::Scroll` acts on any object of type `Widget`, passed in as first argument

---

# 3.1 Function/method **dispatch**

* Most function calls in C++ are glorified `JMP` instructions (assembly)
* **Static typing**: **at compile-time** we know all types, sizes, functions, parameters,…

* Exact offset where functions code resides in memory is known at compile-time

* The compiler _dispatches_ this function call with the exact `JMP` instruction

* Sometimes exact function is known only at run-time: we need **dynamic dispatch**

* Useful for abstractions; situations where caller doesn’t know the exact function to run

* **Need**: plug-ins, interfaces, separation of concern, etc. E.g. text editor with plugins
  - Text editor declares (not defines) a `bool post-process(std::string)` function
  - Every plug-in author implements (defines) it differently
  - When compiling editor the actual function’s code is non-existent; offset unknown
  - At run-time, if a plug-in SO/DLL is present, function is dynamically dispatched
  - **Decouples code**; both editor and plug-in code can build independently

---

# 3.2 Virtual methods and interfaces

``` c++
struct IDisplay {
  virtual ~IDisplay() { }
  virtual void Init() { }
  // pure virtual makes IDisplay an abstract base class (ABC) / interfaces
  // can't be instantiated; need to have a concerte implementation
  virutal Size GetResolution() const = 0;
  virtual void PutPixel(float x, float y, Color c) = 0;
};

struct CCRTMonitor : public IDisplay {
  Size GetResolution() const override { return my_resolution_; }
  void PutPixel(float x, float y, Color c) override {
    /* put color in frame buffer */
  }                     .

private:
  void* frame_buffer_;
  Size resolution;
};

struct CProjector : public IDisplay {
  /* similar definitions */
};

struct CLCDDisplay : public IDisplay {
  /* similar definitions */
};
```

---

# 3.3 Memory Layout

``` c++
  CCRTMonitor m1 in memory    .--->  Virtual table of CCRTMonitor in ".text"
 +-------------------------+  |    +-------------------------------------------+
 |                         |  |    |                                           |
 |   void* vptr            |--’    |  Offset to ~CCRTMonitor()                 |
 |                         |       |                                           |
 |-------------------------|       |-------------------------------------------|
 |                         |       |                                           |
 |   void* frame_buffer_   |       |  Offset to Init()                         |
 |                         |       |                                           |
 |-------------------------|       |-------------------------------------------|
 |                         |       |                                           |
 |   float x_              |       |  Offset to GetResolution()                |
 |                         |       |                                           |
 |-------------------------|       |-------------------------------------------|
 |                         |       |                                           |
 |   float y_              |       |  Offset to PutPixel(float, float, Color)  |
 |                         |       |                                           |
 +-------------------------+       +-------------------------------------------+
```

* Compiler inserts a `vptr` to all objects of type `CCRTMonitor`
* They all point to `CCRTMonitor::vtable`
* Objects of type `CLCDDisplay` also get their own `vptr` pointing to `CLCDDisplay::vtable`
* Dynamic dispatch at run-time **if these methods are called through a pointer / reference of the interface**

---

# C++ Constituents

* Language standard (C++03, C++11, C++14, C++17, …)
  - An ISO certified document defining grammar and semantics
  - Talks about results and guarantees only (_what_ but not _how_)
  - Freedom to implementations using _implementation-defined_, _unspecified_, _**undefined**_
  - Programmers bank on it for maximum portability
* Implementation (G++, Clang, MSVC++)
  - A compiler that confirms to the standard (the _how_)
  - Free to do anything as long as results and guarantees are met
* Library (libstdc++, libc++, Boost …)
  - Collection of reusable code written using pure language facilities or other libraries
  - Distributed as source {`.h` and/or `.cpp`}, static library {`.h`, `.a/.lib`}, dynamic library {`.h`, `.so/.dll/.dylib`}
  - .tag[Flexibility] Python provides list and dictionary as a language-feature, while C++ gives it as library facilities

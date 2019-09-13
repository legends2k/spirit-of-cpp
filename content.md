class: center, middle, inverse
count: false

# The **Spirit** of C++

### Sundaram Ramaswamy

---

class: center, middle, inverse
count: false

## Within C++, there is a much **smaller** and **cleaner** language struggling to get out.
### Bjarne Stoustrup

It’s easy to get lost in the details, but don’t.  **Look for intuitions and at the big picture.**

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
… when you can _just be cool_ and use Python instead ➜
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
// 2⁸⁰ not possible with plain C++!
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
count: false

So many retries! Aargh 🤦

# **Intuitiveness**: Epic fail!

Output varies for same program between compilers!?! Oh mama 😲

---
class: center, inverse
count: false

# Let’s write an `ipow2()`

---
class: center, inverse
count: false

# Let’s write an `ipow2()`
<br />
## Quick quiz: **What’s the size of `int`?**

---
class: center, inverse
count: false

# Let’s write an `ipow2()`
<br />
## Quick quiz: **What’s the size of `int`?**
<br />
## Answer: **It depends!**

<small>(as always)</small>

---

## Built-in Data Types / Primitives ([example](https://godbolt.org/z/UW0rCc))

.pull-left[
### Integral Types.red[¹]

* `char` .little[(≥ 8-bits == 1 byte)]
* `short` .little[(≥ 16-bits)]
* `int` .little[(≥ 16-bits)]
* `long` .little[(≥ 32-bits)]
* `long long`  .little[(≥ 64-bits)]
]

.pull-right[
### Floating-point Types

* `float` .little[(32-bit)].red[²]
* `double` .little[(64-bit)].red[²]
* `long double` .little[(usually 80, 96 or 128 bits)]
]

`sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long)`

.left[### Other Types]
- `void`
- `bool`
- Pointer types
  * **Width**: executable’s bit width; check `sizeof(void*)`
  * Arithmetic based on pointed-to type e.g. `int *p; ++p;` moves `p` by `sizeof(int)`
- Array types e.g. `int[2], char[6]`

.footnote[.red[¹] : variants of `unsigned` and `signed` ([two’s complement](https://en.wikipedia.org/wiki/Two%27s_complement))]
.footnote[.red[²]: Not guaranteed to be, but mostly, IEEE-754 floats]

---

## C++ Standard .little[[(ISO/IEC 14882:2017)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf)]

* Modern language users are at the mercy of their singular implementations

* **The ISO C++ standard guards C++ programmers with certain guarantees**

* **Programs adhering to the standard are always portable and work** <br /> .little[e.g. Compile a 20-year old program `g++ -std=c++98 old.cpp` even today on any platform with a compiler and its output works]

* Standard precisely defines many aspects of a program: **well-defined** ← this is home 🏠

* Standard loosely defines some aspects .little[(_Implementation-defined_,  _Unspecified_ and [_Undefined behaviour_](https://stackoverflow.com/q/2397984/183120))]
  - for exotic architectures having C++ compilers <br /> .little[e.g. [Unisys Servers with 9-bit bytes and 36-bit ints](https://stackoverflow.com/a/6972551/183120) programmable in C and C++ (not Python or JS — _sorry!_)]
  - for freedom to compiler-authors .little[e.g. to optimize aggressively, compete with other implementations, …]

> But it works on my machine!?
>
> **Say, if you survived a wrong side drive on a highway at midnight, would you argue its correct?**

???
e.g. array access out of bounds, accessing a zombie object, null pointer dereference and many more!


---

``` c++
#include <cstdint>   // ← fixed width integers
#include <limits>
#include <optional>
#include <iostream>

std::optional<uintmax_t> ipow2(unsigned pow) {
  // evaluated and folded into a constant number at compile-time
  constexpr uint8_t max_digits = std::numeric_limits<uintmax_t>::digits;
  if (pow >= max_digits)
    return {};

  uintmax_t value = 1;
  value <<= pow;
  return value;
}

int main(int argc, char** argv) {
  if (argc >= 2) {
    auto const pow = std::strtoul(argv[1], nullptr, 0);
    auto const result = ipow2(pow);
    if (result)
      std::cout << *result << '\n';
    else
      std::cout << "Power beyond limits!\n";
  }
}

```

* **Use [fixed-width integers](https://devdocs.io/c/types/integer)**:  `uint8_t`, `int_fast16_t`, `int32_t`, `uintptr_t`, …
* Use `short`, `int`, `long` … when you _know_ minimum (guaranteed) width is enough
  - Chromium has `int`s but [they only’ve 32 and 64 builds](https://www.chromium.org/chromium-os/how-tos-and-troubleshooting/chromiumos-architecture-porting-guide) and specific compilers

---
class: center, middle, inverse
count: false

# OK.  Let’s order food.

---

# At a Restaurant

- For **every item** you order, the _waiter_

  - goes to the kitchen

  - checks availability

  - acknowledges its availability

  - makes an entry

- requests food preparation

- checks prepared items against register

- delivers to you

> **Terrible. Turn-around. Time**
>
> You get hungry + angry → `hangry` 😠


---

# At Home

- For **every item** you order, _Swiggy_

  - goes to the kitchen

  - checks availability

  - acknowledges its availability

  - makes an entry

- requests food preparation

- checks prepared items against register

- delivers to you

> You tip the man **delivering 30 mins later**!

---

class: center, middle, inverse
count: false

# Part 1

# **Why?**

---

## Different Settings, Different Requirements

* At a restaurant, you expect a high turn-around time
  - Wasting time on **avoidable**, petty procedures means an overall delay in eating

* When ordering from home, you know it’ll take time
  - Wasting 5 mins is OK when it’s 30 mins b/w restaurant ↔ home (bottleneck elsewhere)

> Shaving off a few (unnoticeable) microseconds when overall lag would be in seconds is _**pessimisation**._

### Translating to Software

* **Server-side/cloud code**: network latency is the overarching bottleneck

* **GUI application, I/O heavy code**: User-input/disk-access is the biggest latency

---

class: left, middle, inverse

## We should forget about small efficiencies, say about 97% of the time: premature optimization is the root of all evil. Yet **we should not pass up our opportunities in that critical 3%**.

### .right[Donald Knuth / Tony Hoare]

.center[We’ll discuss sane defaults when writing C++ software — just good habits, not pessimizations.]

---

## Languages for **non-critical** software <small>(C#, Python, JS, …)</small>

* .tag[Performance] Most run on a VM with garbage collection: when network lag, I/O latency or user input is the bottleneck, wasting a few milliseconds/operation is OK.

* .tag[Flexibility] Assumes programmers doesn’t know hardware; sacrifices finer control for more features e.g. well-polished data structures like _dictionaries, big integers, …_

* .tag[Hardware] Machine abstracted away as much as possible to relieve programmer from worrying about both hardware intricacies and problem domain.

## Languages for **critical** software <small>(C++, C, Rust, …)</small>

* .tag[Performance] Provides _zero-overhead abstractions_.red[1] by inserting very minimal code; doesn’t get in the way of programmer talking to hardware.  No VM, no GC i.e. <br />  _no middleman — no comission_.

* .tag[Flexibility] Exposes richness of hardware to extract more from it; doesn’t insult programmer’s intelligence.  Direct access to CPU/GPU/OS facilities, _yum_!

* .tag[Hardware] Enables authoring device drivers, virtual machines.red[2], simulators, …

<div>
.footnote[.red[¹] : `std::vector` or `std::map` should perform _almost_ like a vector or red-black tree hand-coded in assembly]

.footnote[.red[²] : Not system virtual machine (like VMWare, Qemu, VirtualBox, …), but **[process virutal machine](https://en.wikipedia.org/wiki/Virtual_machine#Process_virtual_machines)** (like JVM, CLR, …)]
</div>

---

class: center, middle, inverse
count: false

# Good engineers **use the right tool for the right job**.

---

# Problems with unsuitable tools

* Non-availability of facilities
* Community’s mindset will be orthogonal
* Imposes a mental model not suitable for the task
* Examples
  - using Python to write a virtual machine
  - using C++ for client-side browser scripting
  - using JavaScript for … (_never mind_)

> If all you have is a hammer, everything looks like a nail. .right[— [Law of the Instrument](https://en.wikipedia.org/wiki/Law_of_the_instrument)]

* Learning more than one language, tool or framework pays off in the long run
  - Exposes you to more techniques and idioms from different spheres

  - Making your mental model flexible, free-thinking

  - Leads to making right choice in choosing a tool for a problem

---

# Hardware vs Software

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

It’s justified that browsers are written in C++.

---

class: center, middle, inverse
count: false

## That makes you a
# **System Programmer**
### Take pride — there aren’t many.

---

class: center, middle, inverse
count: false

# Part 2

# **Code**

---

# 1. Free-standing **function**s

``` c++
int add(int x, int y) {
  return x + y;
}

int main() {
  int a = 1, b = -4;
  return add(a, b);
}
```

Simple, _complete_ program: no classes, no libraries.

- **Compile-time**: types, qualifiers, functions, structs, classes, templates, etc. exist.
- **Run-time**: Oodles of relocatable machine code your machine loves to gobble!

While languages compiling to byte code, keep type information at run-time too for _reflection_, _garbage collection_, _optimization_, … C++ strips them and spits plain assembly; [most things vanish into 0 s and 1 s](https://godbolt.org/z/YZCG8u).

> **Machine**: `int`?  Who’s she?  I’ve never heard of her.

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

> **You only pay for what you use**. — C++ Design Guidelines

class: center, middle, inverse

# The **Spirit** of C++

### Sundaram Ramaswamy

---

class: center, middle, inverse

## .left[Within C++, there is a much **smaller** and **cleaner** language struggling to get out.]
### .right[— Bjarne Stroustrup, creator of C++]

<br /><br />

It’s easy to get lost in the details; don’t.

**Look for intuitions and at the big picture.**

We want to build a _mental model_, not a rule book.

---

class: center, middle, inverse

# **Experts**
<br />
## Anything you say.red[1] can and will probably be taken offline
<br />
### .left[When starting out, detailing every nook and cranny of a concept would be detrimental.]

.footnote[.red[¹]: that goes too deep, deviates from the topic or only shows _our_ prowess]

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
int      x1 = pow(2, 63); // Warning: overflow (GCC)
unsigned x2 = pow(2, 63); // Warning: overflow (GCC)
uint64_t x3 = 1   << 63;  // Warning: shift count ≥ width
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

Output varies between compilers for same program?! Oh mama! 😲

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

* `char` .little[(≥ 8-bits = 1 byte)].red[2]
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
Only the _below rule about integers is well-defined_; assuming more is [risky](https://godbolt.org/z/UW0rCc)!]
]

**`sizeof(char) ≤ sizeof(short) ≤ sizeof(int) ≤ sizeof(long) ≤ sizeof(long long)`**<br />

.left[### Other Types]
- `void`
- `bool`
- Pointer types e.g. `int*`, `unsigned char**`, `void (*)(float)`<br />
.little[Arithmetic based on pointed-to type e.g. `int *p; ++p;` moves `p` by `sizeof(int)`]
- Array types e.g. `int[2], char[6][5]`

.footnote[.red[¹]: variants of `unsigned` and `signed` ([two’s complement](https://en.wikipedia.org/wiki/Two%27s_complement))]
.footnote[.red[²]: A byte needn’t be 8 bits; it’s whatever `char`’s bit width is 😳]
.footnote[.red[³]: Not guaranteed to be, but mostly, [IEEE-754](https://en.wikipedia.org/wiki/IEEE_754) float]

---

## C++ Standard .little[[(ISO/IEC 14882:2017)](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf)]

* Many modern language users are at the mercy of one implementation

* **The ISO C++ standard guards C++ programmers with certain guarantees** <br />.little[A contract between language users and compiler writers ] 🤝

* **Programs adhering to the standard always work and remain portable** <br />
.little[e.g. Compile a 20-year old program `g++ -std=c++98 old.cpp` _even today on any platform_ with a compiler; it works] <br />

* Standard precisely defines many aspects of a program: **well-defined** ← this is 🏠
* Standard loosely defines some aspects .little[(_Implementation-defined_,  _Unspecified_ and [_Undefined behaviour_](https://stackoverflow.com/q/2397984/183120))] ☠
.little[
- for many exotic architectures having C++ compilers <br /> .little[e.g. [Unisys Servers with 9-bit bytes and 36-bit ints](https://stackoverflow.com/a/6972551/183120) programmable in C and C++ (not Python or JS — _sorry!_)]
- for freedom to compiler-authors — different compilers, varying implementations: a healthy competition
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
  // Future-proof by not using uint64_t and limiting to 64-bit architectures.
  // Obtain size from compiler at compile-time; thanks to static typing
  if (pow >= std::numeric_limits<uintmax_t>::digits)
    return {};
                           //                 2⁸  2⁷  2⁶  2⁵  2⁴  2³  2²  2¹
  uintmax_t value = 1;     // ----------------------------------------------+
  value <<= pow;           // … | 0 | 0 | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
  return value;            // ----------------------------------------------+
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

* **Use [fixed-width integers](https://en.cppreference.com/w/cpp/types/integer)**:  `uint8_t`, `int_fast16_t`, `int32_t`, `uintptr_t`, …
.little[- `unsigned`: beware of wrap around behaviour; decrement with extreme care e.g. `uint8_t x = 0; --x; // x is now 255`]
* Use `short`, `int`, `long`, etc. when you’re _sure_ minimum (guaranteed) width is enough <br />
  .little[- Chromium has `int`s but use only a few compilers (all having 32-bit `int`) and [target only `i686`, `x86_64` and `ARM32` builds](https://www.chromium.org/chromium-os/how-tos-and-troubleshooting/chromiumos-architecture-porting-guide) ]

---

name: func-1

## 2. Free-standing **Function**s

- **Compile-time**: types, qualifiers, functions, structs, classes, templates, … exist.
- **Run-time**: Oodles of [binary](https://godbolt.org/z/vrRUe9) your machine loves to gobble!

---

template: func-1
name: func-2

<!-- Refer code/basic.cpp -->
``` c++
// Simple, complete program: no classes, libraries or includes.      +-------------+
int add(int x, int y) {            //     +-------------+            |    free     |
  return x + y;                    //     |    free     |            |             |
}                                  //  S  |             | <-- add()  +-------------+
                                   //  T  |             |            | int  int    |
int main(int argc, char** argv) {  //  A  +-------------+  add() --> +-------------+
  int a = 1, b = -4;               //  C  | int  int    |            | int  int    |
  return add(a, b);                //  K  | int  char** | <- main -> | int  char** |
}                                  //     +-------------+            +-------------+
```

---

template: func-2

* Languages compiling to byte code, keep type information.red[1] at run-time too<br />
.little[VM needs them for _reflection_, _garbage collection_, _JIT optimizations_, …]
* .tag[Hardware] C++ strips them and spits [object code](https://en.wikipedia.org/wiki/Object_code)<br />
.little[Data and code vanish into zeros and ones.  _Raw binary_, just as advertised 👍]

> .tag[Flexibility] .tag[Performance] C++ data types — built-ins & custom — are raw; no base `Object` under the hood.  They **inherit nothing**..red[2]

.footnote[.red[¹]: Commonly called _boxed datatypes_ e.g. `Integer` inheriting `Java.lang.Object` wraps the actual integer. C++’s `int` is machine integer.]
.footnote[.red[²]: You don’t pay for what you don’t use.]

---

## 3. **Objects in Memory**

.pull-left[
* Static type system
.little[
- Expects all types, variables known at compile-time
- Key to compile-time error detection and performance]
* Type = _(Size, Operations)_.little[- e.g. `int` loads `sizeof(int)` bytes to an integer register; allows binary and arithmetic operations.]
* Object = _(Type, Memory) → Value_
.little[
- Allocation + Initialization
- Value interpretation based on type]
* Other languages hide locations
.little[
- But **every variable is a pointer**! e.g. Python.red[1] internally uses [`PyObject*` for _every_ variable](https://stackoverflow.com/a/57380719/183120)]
]

.pull-right[
* Most languages treat variable as stickers; labels stuck on objects
.little[- Types associated with objects (at runtime too)
- Variables = handle to be (re)stuck on an object]

```python
> i = 0     # type ∉ variable
> i = "hi"  # no error on reseat
```

* **Variable = _(Type, Location, Value)**_<br />
.little[  - Born, live, die with same type and location]

```c++
int a = 1;
float a = 0.5;  // redefinition error
```
]

### Memory model
<!-- Great ASCII art reference -->
<!-- http://xahlee.info/comp/unicode_ascii_art.html -->
```c
char c = 0xde;      // value: 0xde (c), loc = 1000 (&c), type: char (gone at runtime)
int a = 3203338898;                     // allocate sizeof(int) bytes, initilize to 1
short *b = reinterpret_cast<short*>(&a);     // *b = 0xfeed (on a big-endian machine)
char *p = &c;            ,--------------------------------------------------
                        ↓
 …   1000   1001  /-- short --\               /--------- char* ---------\
----------------------------------------------------------------------------
.. | 0xde | 0xad | 0xfe | 0xed | 0x12 | 0x92 | 0x00 | 0x00 | 0x10 | 0x00 | ..
----------------------------------------------------------------------------
      ↑          \----------- int ----------/                 |
       `------------------------------------------------------'
```

.footnote[.red[¹]: [CPython](https://en.wikipedia.org/wiki/CPython) – the most common Python implementation]

---

class: center, middle, inverse

### Yeah.. yeah! 😈

# What about **2⁸⁰** ?

---

## Hardware Limits and **Software Emulation**

* Consider an architecture, say [x64](https://en.wikipedia.org/wiki/64-bit_computing); maximum representable `int` on x64 is `0xffff'ffff'ffff'ffff` = `18446744073709551615`

* Values, instructions, addresses cannot exceed as CPU cannot understand a larger _integer_

* How then to represent larger integers?  _Big Integers_ a.k.a _Big Numbers_

* Make an array of 64-bit integers each representing a part of the constituted big integer

* e.g. for `1 << 80`, you’d need 3 integers; result, if shifted again by `20`, prefix one more, …

``` c++
//  bits                     127  96 95   64 63   32 31    0
            +-----------------------------------------------+
            | . . . |  int  |  int  |  int  |  int  |  int  |  // a big int in memory
            +-----------------------------------------------+
// elements             4       3       2       1       0
```

* **Fun Exercise**: Implement `class BigInt` using a `std::vector<uint8_t>`
  - Implement arithmetic and shift operators
  - Needs overflow/underflow detection and bit manipulation

* Use rudimentary hardware features as building blocks to build higher abstractions

---

name: zero-cost-abs-1

## **Zero Cost Abstraction** .little[answer to 2⁸⁰, life and beyond …!]

* .tag[Hardware] Manufacturers make _dedicated_ hardware for certain algorithms
.little[
- E.g. [GPU](http://en.wikipedia.org/wiki/GPU)s for 3D data and pixels, hardware decoders for MP3, etc.
- SIMD instruction sets to perform multiple operations in one CPU cycle.
]
* .tag[Performance] **Using dedicated hardware is fast; _really fast_** 🚀
* Software emulation provided as a fallback when dedicated hardware absent – _slow_ 🚲
.little[
- e.g. Graphics using CPU when GPU is missing
]

---

template: zero-cost-abs-1
name: zero-cost-abs-2

* Python’s `int` has _arbitrary-precision_ because internally [**all** integers are big!](https://rushter.com/blog/python-integer-implementation/) 😲

  - Programmer is never exposed system’s native integer (64-bits)
  - Software emulation, not for specific algorithms but, for a basic functionality
  - Every programmer penalized for flexibility so that expressions like `2 ** 80` work

---

template: zero-cost-abs-2

* C++ only provides `int`, and no more, since your machine only has that!

  - .tag[Flexibility] Include a library or write one just for the module needing `BigInt`
.little[- e.g. [GNU Multiple Precision](https://gmplib.org/) library is one of the fastest out there]
  - .tag[Hardware] .tag[Performance] Exposes dedicated hardware by providing direct access
.little[- Using [SSE2](https://en.wikipedia.org/wiki/SSE2) is just a header away: [live example](https://godbolt.org/z/OuwQE1) of a 4-vector addition using 1 assembly instruction]

> **You only pay for what you use**. — C++ Design Principle

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

## Languages for **non-critical** software <small>(C#, Python, JS, …)</small>

* .tag[Performance] Most run on a VM.red[1] with garbage collection.

* .tag[Flexibility] Sacrifices finer control for more features.<br />
.little[e.g. reflection, garbage collection, rich built-in types like _lists, dictionaries/maps, big integers_, … ]

* .tag[Hardware] Abstracts machine away as much as possible.<br />
.little[Assumes programmers doesn’t know hardware; relieves programmer from worrying about hardware intricacies]

## Languages for **critical** software <small>(C++, C, Rust, …)</small>

* .tag[Performance] [Zero-overhead abstractions](https://isocpp.org/wiki/faq/big-picture#zero-overhead-principle).red[2]; as close to hardware as possible but not closer.<br />
.little[Easy to reason about the machine code generated for your program.]

* .tag[Flexibility] You choose what you want. _You only pay for what you use_.red[3].<br />
.little[Programmer, not the language, is in charge. Believes programmer knows what s/he doing.]

* .tag[Hardware] _Low-level access_ enables authoring kernel, virtual machines, …<br />
.little[Direct access to CPU/GPU/OS facilities, _yum_!  No VM, no GC  _no middleman — no commission_.]

<div>
.footnote[.red[¹]: Not system virtual machine (like VMWare, Qemu, VirtualBox, …), but **[process virutal machine](https://en.wikipedia.org/wiki/Virtual_machine#Process_virtual_machines)** (like JVM, CLR, Python interpreter, …)]

.footnote[.red[²]: `std::vector` or `std::map` should perform _almost_ like a vector or red-black tree hand-coded in assembly]

.footnote[.red[³]: A key design guideline of the C++ standards committee; another is _portability_.]
</div>

---

## 4. **Memory**: Types

.pull-left[
### Stack
- **Automatic** allocation and deallocation
.little[- Alloc/dealloc order: lexical/reverse lexical order.red[1]
]
- **Fast**: _really_ fast! 🚀
.little[
- Allocation is a mere register increment/decrement
- Spatial and temporal coherence; cache-friendly
]
- Limited scope
.little[
- Alive within current and called functions
- Can’t `return` local variable _address_ but can pass up
]
- Limited size (configurable)
.little[
- Default MiB/thread: 2 ([GCC](https://stackoverflow.com/a/32543529/183120)), 1 ([MSVC](https://docs.microsoft.com/en-us/cpp/build/reference/f-set-stack-size?view=vs-2019))
]
- Unexposed in most languages 😱
]

.pull-right[
### Heap
- **Manual** allocation and deallocation
.little[- Manual memory management is a _land mine_ 💥
- [40 years of experience proves we are bad at it](https://youtu.be/u_ij0YNkFUs?t=1670); be smart, use `unique_ptr`, `shared_ptr`, `weak_ptr`, `vector`, `string` …
- Never even write `new`, `malloc`, `CoTaskMemAlloc`, …
- Know: `delete ≠ delete []`, `delete ≠ free()`, …
]
- **Slow**: de/alloc involves OS intervention
.little[
- Virtual memory, book keeping, fragmentation
- _Pointer chasing_ isn’t cache-friendly
]
- No scope; alive until manually freed
.little[
- Rich source of [memory leaks](https://en.wikipedia.org/wiki/Memory_leak)! [Live example](http://coliru.stacked-crooked.com/a/7a2ece5e08c85380).
]
- Practically no limit
.little[([32-bit: 3 GiB, 64-bit: 16 EiB](https://softwareengineering.stackexchange.com/a/207390/4154))]
]

.left[> **Stack-allocate** if size known at compile-time, within scope and within size limit;  **heap-allocate** otherwise.]

``` c++
void LoadCount(int* count);
// THE GOOD          // THE BAD              // THE UGLY
int c = 0;           int* c = new int;       unique_ptr<int> c = make_unique<int>();
LoadCount(&c);       LoadCount(c);           LoadCount(c.get());
                     delete c;               // needless alloc and dealloc - slow
```

.footnote[.red[¹]: Order matters: so from top to bottom, declare least dependant to most dependant – both in function and `struct`/`class`.]

???
- Stack size is OS-dependant too: 8 MiB on macOS
- Know: `delete` between runtimes are unequal!

---

## 4.1 Memory: **Objects across Stack and Heap**

``` c++
struct Passport {                       struct SmartPassport {
  char* id;                               string id;
  int expiry;                             int expiry;
}                                       }

struct User {                           struct SmartUser {
  int dob;                                int dob;
  char* name;                             string name;
  Passport* pass;                         unique_ptr<SmartPassport> pass;
};                                      };

int main() {                            SmartUser u1 = {0, {}, new SmartPassport()};
  User *u2 = new User();
  u2->name = new char[10]();
  u2->pass = new Passport();
  u2->pass->id = new char[10]();                    HEAP
                                        ---------------------------
  delete [] u2->pass->id;        ------/      .-----------------.  \------
  delete u2->pass;           ---/          .->| | | | | | | | | |         \---
  delete [] u2->name;     --/              |  '-----------------'             \--
  delete u2;            -/   .-----------. |    .-------.                        \-
}                      /  .->| int       | | .->| char* |-    .-----------------.  \
   +-------------+    /   |  |-----------| | |  |-------| `-->| | | | | | | | | |   \
   |             |    |   |  | char*     |-' |  | int   |     '-----------------'   |
 S |-------------|  .-\---'  |-----------|   |  '-------'                           /
 T | User*       |--'  \     | Passport* |---'    .-----------.                    /
 A |-------------|      -\   '-----------'    .-->| char*     |                  /-
 C | int | SP*   |--,     --\                 |   |-----------|               /--
 K |-------------|  '-----------\-------------'   | int | int |           /---
   | int | char* |               ------\          '-----------'    /------
   +-------------+                      ---------------------------
```

---

## 4.2 Memory: **Shallow Copy**, **Deep Copy** and **Move**

``` c++
//--SHALLOW COPY: copy values as-is -------------------------------------------------
//                                                        owner     clone
  int owner = 12;  // owner has data originally          +----+    +----+
  int clone = 0;   // need to make a copy                | 12 |    | 12 |
  clone = owner;   // copy value as-is                   +----+    +----+

//--SHALLOW COPY of pointer (mostly wrong, unless intended sharing) -----------------
//                                                   owner       data      clone
  int* owner = new int(12); // owner is a pointer  +-------+   .-----.   +-------+
  int* clone = owner;       // clone copies it!!   | 0x100 |-->| 12  |<--| 0x100 |
// Inconsistent ownership!!                        +-------+   '-----'   +-------+
// Responsibility unclear: who'll free data?                    0x100

//--DEEP COPY: both gets to have its own copy----------------------------------------
//                                                    owner            clone
  int* owner = new int; *owner = 12; //              +-----+  .----.  +-----+  .----.
  int* clone = new int; // allocate memory           |0x100|->| 12 |  |0x200|->| 12 |
  *clone = *owner;      // copy data, not address    +-----+  '----'  +-----+  '----'
//                                                            0x100            0x200

//--MOVE: steal data from the owner--------------------------------------------------
  int* owner = new int(12); //              owner         data         thief
  int* thief = nullptr;     //            +-------+     .------.     +-------+
  std::swap(owner, thief);  //            | 0x100 |---->|  12  |     |  0x0  |---->X
// super cheap                            +-------+     '------'     +-------+
//   - just an integer swap                               0x100
//   - no memory allocation
//                                          owner         data         thief
//                                        +-------+     .------.     +-------+
//                                  X<----|  0x0  |     |  12  |<----| 0x100 |
//                                        +-------+     '------'     +-------+
```

---

## 4.3 Memory: **Object Copy and Move**

<!-- Refer code/obj_copy_move.cpp -->
``` c++
struct Machine {                               struct Image {
  int max_memory;                                int w = 0, h = 0;
  float dimensions;                              uint32_t* pixels = nullptr; // rgba
};
int main() {                                     int size() const { return w * h; }
  // shallow / trivial copy
  Machine m1 = { 1, 1.2f }, m2;                  Image(int width, int height)
  m2.max_memory = m1.max_memory;                   : w(width), h(height) { }
  m2.dimensions = m2.dimensions;                 Image() : Image(0, 0) { }
                                                 ~Image() { if (pixels)
                                                               delete [] pixels; }
                                               };
  // make an image, allocate pixel data
  Image owner(4, 4);
  owner.pixels = new uint32_t[owner.size()];

  Image clone(owner.w, owner.h);
  clone.pixels = owner.pixels; // BAD! clone.~Image will try delete on dangling ptr

  // deep copy - byte by byte copy: each image gets its own copy of pixels
  clone.pixels = new uint32_t[owner.size()];
  std::copy_n(owner.pixels, owner.size(), clone.pixels);

  // move: no allocation, no deep copy
  Image thief;
  thief.w = owner.w; thief.h = owner.h;
  std::swap(thief.pixels, owner.pixels);
  owner.w = owner.h = 0;                  // be a responsible thief ;)
}                                         // leave owner in a decent state
```

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
  Point origin = {};                //  +-------+-------+-------+
  int max_memory = 64;              //  | float | float |  int  |
                                    //  +-------+-------+-------+
public:
  Canvas(float max);
};
                                    //       /<--- c[0]  --->\ /<---- c[1] --->\
                                    //   ---+-----+-----+-----+-----+-----+-----+---
// 3. Aggregate                     //      |float|float| int |float|float| int |
int vals[3] = {0, 1, 2};            //   ---+-----+-----+-----+-----+-----+-----+---
Canvas c[2] = { Canvas(1024), Canvas(512) };
```

.pull-left[
* When **authoring** a class think **data** first
.little[- Methods are vital to class usage; think user and design]
* In C++, [inheritance isn’t for code reuse](https://isocpp.org/wiki/faq/objective-c#objective-c-and-inherit)
.little[- It is for interface compliance; prefer _composition_]
* [Accessors (`const`) and mutators](https://stackoverflow.com/q/9627093)
* Well-written classes are **easy to use and hard to misuse**
]

.pull-right[
* Managing resources? or [POD](http://en.cppreference.com/w/cpp/types/is_trivial)?
  - Smart wrappers or scoped guards
  - Keep compiler-supplied functions?
  - Operators overloading? `operator=`?
* Non-friend free functions > methods
  .little[- Utilities shouldn’t be methods]
]

---

## 5.1 **Methods**: Accessors and Mutators

Methods are functions with a hidden first argument for `this` — the object pointer

``` c++
struct Widget : public Point {
  void Scroll(int clicks) { y_ += (clicks * OFFSET); }  // change state of object
  void SetScale(int s) { scale_ = s; }                  // directly or indirectly
  int GetScale() const { return scale_; }  // just observe; const guarantees
                                           // no state change; no write to data members
                                           // or call to non-const methods
private:
  constexpr static int OFFSET = 3;         // compile-time; stripped at run-time
  int scale_;
};
```

Roughly equivalent to

``` c++
struct __Widget {    // object only has data (member variables)
  float x_, y_;      // base class members first and in order of declaration
  int scale_;        // code operating on it isn't part of it
};

// code elsewhere (.text segment)
void __Widget_Scroll(__Widget* w, int clicks) {
  w->y_ += (clicks * 3);  // OFFSET resolved at compiled-time
}

int __GetScale(const __Widget* w) { return w->scale_; }
```

---

## 5.2 Function / Method **Dispatch**

* Simple Function/Method calls
  - Most function calls in C++ are glorified `JMP` instructions (assembly)
  - Exact offset of functions’ code in memory known at compile-time (_Static typing_)
  - Compiler _dispatches_ function calls with `JMP` to known offset
* Sometimes exact function is known only at run-time; we need **Dynamic Dispatch**.red[1]

* Useful for abstractions fulfilled by another class / programmer — interfaces, plug-ins, … i.e. to facilitate _separation of concerns_ .red[2]

* Example 1: `IDisplay::clear()` needn’t know if it’s a `CLcdDisplay` or `CCrtMonitor`

* Example 2: a text editor with plug-ins
.little[
- Text editor declares, **not defines**, a `bool IPlugin::post-process(std::string)`
- Every plug-in author implements (defines) it differently
- When compiling editor the actual function’s code is non-existent; offset unknown – can’t do static dispatch
- At run-time, if a plug-in SO/DLL is present, load function (`dlsym` or `GetProcAddress`) and dispatch dynamically
- .tag[Flexibility] **Both editor and plug-in code can build independently**! Weak coupling FTW 💪
]

.footnote[.red[¹]: a.k.a. _double dispatch_; broadly known as (run-time) _polymorphism_]
.footnote[.red[²]: The _S_ in [SOLID Principles](https://en.wikipedia.org/wiki/SOLID_(object-oriented_design)]

---

## 5.3 Virtual methods, Interfaces and Concretes

``` c++
struct IDisplay {
  virtual void Init() { }  // virtual, not pure virtual, method; no `= 0`
                           // A concrete class may / may not override.
  virtual ~IDisplay() { }  // Interfaces MUST have virtual destructor.

  // Pure virtual makes IDisplay an abstract base class (ABC) / interface.
  // It can't be instantiated; need to have a concerte implementation.
  // A concrete class MUST override these to be called concrete.
  virutal Size GetResolution() const = 0;
  virtual void PutPixel(float x, float y, Color c) = 0;
};

struct CCrtMonitor : public IDisplay {
  Size GetResolution() const override { return my_resolution_; }
  void PutPixel(float x, float y, Color c) override {
    /* put color in frame buffer */
  }

private:
  void* frame_buffer_;
  Size resolution;
};

struct CProjector : public IDisplay {  // still an ABC as not all pure virtuals
  Size GetResolution() const { }       // are overridden
};

struct CLcdDisplay : public IDisplay {
  /* similar definitions */
};
```

---

## 5.4 Memory Layout of Objects with Virtual

``` c++
  CCrtMonitor m1 in memory    .--->  Virtual table of CCrtMonitor in ".text"
 +-------------------------+  |    +-------------------------------------------+
 |                         |  |    |                                           |
 |   void* vptr            |--’    |  Offset to ~CCrtMonitor()                 |
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

* Compiler inserts a `vptr` to all objects of type `CCrtMonitor`

* They all point to `CCrtMonitor::vtable`

* Objects of type `CLcdDisplay` also get their own `vptr` pointing to `CLcdDisplay::vtable`

---

## 5.5 Virtual Methods: Usage

``` c++
struct IDisplay { virtual ~IDisplay() { } };

struct CCrtMonitor : public IDisplay { ~CCrtMonitor() { /* cleanup stuff */ } };

class CLcdDisplay : public IDisplay { /* ... some code ... */ };

IDisplay* MakeDisplay(DisplayParams params) {  // factory function
  if (params.resolution <= 480)
    return new CCrtMonitor;
  else
    return new CLcdDisplay;
}

int main() {
  IDisplay od;    // Error: can't instantiate an abstract class
  IDisplay *pd;   // OK: pointer to abstract IDisplay

  CCrtMonitor crtMon;
  crtMon.PutPixel(10, 5, Color(255, 0, 0));  // static dispatch

  pd = MakeDisplay(params);
  pd->PutPixel(20, 3, Color(0, 0, 255));     // dynamic dispatch

  delete pd;    // Destroy object behind IDisplay*.  Calls
                // 1. ~CCrtMonitor properly (thanks to virtual ~IDisplay)
                // 2. ~IDisplay
                // 3. releases memory
                // Without virtual base destructor step 1 will be skipped!
                // Cleanup code in ~CCrtMonitor won't be run!!
}
```

---

## 6: **RAII**.red[¹]: acquire in `T()` and release in `~T()`

.tag[Flexibility] .tag[Performance] C++ doesn’t come with garbage collection because it has RAII!

``` c++
#include <cstddef>  // ← for std::byte; preferred over uint8_t

struct FileReader {       // a crude smart wrapper

  FileReader(std::string path) {
    // accquire resources in the constructor
    file_ = fopen(path.c_str(), 'r');
    if (file_) {
      data_ = new std::byte[100];
      if (data_) fread(data_, sizeof(std::byte), 100, file_);
    }
  }

  ~FileReader() {
    // destruct resources in the destructor
    if (file_)
      fclose(file_);

    if (data_)
      delete [] data_;
  }

private:
  std::byte* data_ = nullptr;
  FILE* file_ = nullptr;
};
```

.footnote[.red[¹]: RAII – _Resource Acquisition Is Initialization_ – is the cornerstone idiom of modern C++ but with the worst possible name 😠]

---

## 6.1: **RAII** recursively

Didn’t we say _NO `new` or `delete`_?  Let’s try again.

``` c++
struct FileReader {       // a smart wrapper with no low-level fiddling
  File(std::string path) {
    file_.reset(fopen(path.c_str(), 'r'));
    if (file_) {
      const auto file_size = getFileSize(file_.get()); // get() gives underlying T*
      data_.resize(file_size);                         // auto resize vector's memory
      fread(data_.data(), sizeof(std::byte), file_size, file_);
    }
  }

  // ~FileReader() — Look ma, no destructor!
  // Under the hood, when some `File f` gets destroyed, these are called (in order)
  //     1. ~unique_ptr<FILE, FileCloser> calls fclose()
  //     2. ~vector<std::byte>() auto deletes memory its ptr_ is pointing at
  //     3. ~File() finally but since it's a no-op, it'd be optimized away
private:
  std::unique_ptr<FILE, FileCloser> file_;  // unique_ptr auto closes file
  std::vector<std::byte> data_; // vector manages bytes, auto resizes array
};
// Functor is like a function but can have states; lambda functions are functors too.
struct FileCloser {           // This function takes a FILE* and returns nothing.
  void operator()(FILE* f) {  // Callable thus: FileCloser fc;  fc(file_ptr);
    if (f) fclose(f);         // Usable as unique_ptr<T>'s Deleter as its prototype
  }                           // matches it by a taking T*; here T = FILE.
};
```
Recursive since `FileReader` – a smart wrapper – is now embed-able in another higher abstraction 💡 When _that_ gets destroyed, `FileReader` will automatically release its resources.

---

## 5.6: **Constructor**: Initialize invariants

* Auto-generated default constructor is a no-op; members would be garbage values
* Write a constructor to initialize states / data members
* Prefer in-class initializers; sometimes no constructors may be needed

``` c++
struct Point {
  float x, y;    // garbage by default
};

struct Circle {

  float radius = 1.0f;  // in-class initializer
  Point centre;         // garbage if created using Circle()

  Circle() = default;

  Circle(float r) : radius(r), centre(0.0f, 0.0f) {  // member initializer list
  }

  static Circle* MakeCircle(float radius, Point centre) { /* return a circle */ }
};
```

* Prefer member initializers over setting them in the body
* [Make constructors `explicit`](https://isocpp.org/wiki/faq/ctors#explicit-ctors) to prevent on-the-fly creation<br />
.little[e.g. `DrawFigure(float) ≠ DrawFigure(Circle(float))` for heavy-classes, on-the-fly creation is a red flag 🚷]
* Want to control creation?
.little[
- Make constructors `private`
- Author a `static` class function that acts as a factory ([_Named Constructor Idiom_](https://isocpp.org/wiki/faq/ctors#named-ctor-idiom))
]

---

## 1.1 **Temporaries, References**


* _References_ are just nicknames; underlying object is exactly the same
.little[
* Edit the reference and you edit the value
* `const` reference simply disallows editing object through that reference
]

* _Temporaries_ are compiler-generated objects during expression evaluation

``` c++
Size Circle::ComputeBounds() { return Size(width_, height_); }

c.ComputeBounds();  // returned Size is a temporary alive until expression evaluation
```

* Objects only have types; expressions have a **non-reference** _type_ and a _value category_

``` c++
Image i1, &i2, &&i3;  // objects of type lvalue, lvalue-ref, rvalue-ref to Image
std::move(i2)         // expression type: Image, value category: xvalue
```

### [l-values, r-values](https://en.cppreference.com/w/cpp/language/value_category)… oh-my-values!! 🤯

* _lvalue_: have identity, cannot be stolen e.g. `int a; Circle &&c;`

* _xvalue_: have identity, can be stolen e.g. what’s returned from `std::move(c)`

* _prvalue_: no identity, can be stolen e.g. `str.substr(1, 2)`

---

name: copy-move-ctor1

## 5.7 **Copy** and **Move Constructors**

``` c++
struct Image {
  int width_; int height_; std::vector<std::bytes> pixels_;

  // copy constructor for deep copying
  Image(const Image& that) : w_(that.w_), h_(that.h_) {
    const auto count = w_ * h_;
    if (that.pixels_ && (count > 0)) {
      this->pixels_.resize(count);
      std::copy_n(that.pixels_, count, this->pixels_);
    }
  }
  // move constructor
  Image(Image&& that) : w_(that.w_)
                      , h_(that.h_)
                      , pixels_(std::move(that.pixels_)) {
  }
};
```

---

template: copy-move-ctor1

But wait! RAII recursively, right? Wouldn’t the defaults suffice? They totally do!! 👌

``` c++
class Image {
  // ... and we're done! We wisely chose RAII wrapper over raw pointers :)

  // How implicitly generated defaults look like?  Move, same as above.  Copy:
  //
  // Image(const Image& that) : w_(that.w_), h_(that.h_), pixels_{that.pixels_} {
  // }                      // std::vector's copy constructor does the right thing 👍
};
```

---

## 5.8 **Copy** and **Move Assignment** operators

``` c++
class Image {
  // operator methods are just like any other methods; can be called with expression
  // Image i1, i2;  i1 = i2 is the same i1.operator=(i2)

  // copy-assignment operator, similar to copy ctor but without the memory alloc
  Image& operator= (const Image &that) {
    this->w_ = that->w_;
    this->h_ = that->h_;
    this->pixels_ = that->pixels_;  // std::vector::operator= does deep copy, yay!
    return *this;
  }
  // move-assignment operator
  Image& operator=(Image&& that) {
    this->w_ = std::move(that->w_);
    this->h_ = std::move(that->h_);
    this->pixels_ = std::move(that->pixels_);   // std::vector::operator= does move
    return *this;                               // its stuff safely, yay!
  }
  
  // You only used smart types as members, didn't ya, you sly fox?!
  // Skip writing both and bask in the glory: “The Rule of Zero” 😏
};
```

- Implicitly generated move constructor will fallback to copy if a type isn’t move-friendly e.g. is a POD or has no move constructor
- No implicit generation if even one member is non-copyable or immovable

---

## 2.1 Function **Parameter and Return Type** — Defaults.red[¹]

* `Out` parameter a.k.a return value: `X f()` – by value
.little[
- xvalue would get moved
- Even for move-unfriendly types compiler does _Return Value Optimization_ / [_Copy Elision_](https://isocpp.org/wiki/faq/myths#copy-elision)
]

* `Out` but expensive to move (e.g. `std::vector<BigPOD>`): `f(X&)` or `f(X*)` – by reference

--------------------------------------------------------------------------------

* `In/Out` parameter: `f(X&)` – by reference

--------------------------------------------------------------------------------

* `In` parameter: `f(const X&)` – by `const` reference

* `In` but cheap or impossible to copy (e.g. `int`, `std::unique_ptr`): `f(X)`  – by value

* `In` but need ownership: `f(X&&)` – rvalue reference

* `In` but need copy: give two overloads
  1. `f(const X&)` – `const` reference – for those who want to keep theirs
  2. `f(X&&)` – rvalue reference – for those who want to give up theirs

.footnote[.red[¹]: based on [Herb Sutter’s CppCon 2014 presentation](https://github.com/CppCon/CppCon2014/blob/master/Presentations/Back%20to%20the%20Basics!%20Essentials%20of%20Modern%20C%2B%2B%20Style/Back%20to%20the%20Basics!%20Essentials%20of%20Modern%20C%2B%2B%20Style%20-%20Herb%20Sutter%20-%20CppCon%202014.pdf); see pages 22 to 34.]

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

# Links and Recommendations

* [Standard C and C++ Documentation](https://en.cppreference.com/)

* [ISO C++ FAQ](https://isocpp.org/wiki/faq/)

* [The Definitive C++ Book Guide and List](https://stackoverflow.com/q/388242/183120)

# Online Compilers

* [Coliru Stacked Crooked](http://coliru.stacked-crooked.com/) – Minimal with Share option

* [Compiler Explorer / Godbolt](https://godbolt.org/) – Disassembly<br />
.little[Online Compiler with Disassembly – supports numerous compilers!]

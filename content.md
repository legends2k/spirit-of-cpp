class: center, middle, inverse

# The Spirit of **C++**

## Sundaram Ramaswamy

---
.left-column[
  ## Why Use C++?
  #### Why not Python or JavaScript?
]

.right-column[
Why go through …

- crude data types
- raw _pointers_, contorted _references_
- manual memory management

.pull-left[
… when you can _just be cool_ and use Python instead
]

.pull-right[
```python
x = 2 ** 63  # 2 power 63
print(x)

> 9223372036854775808
```
]

How C++ fares against Python here?

``` c++
int      x1 = pow(2, 63);  // GCC: overflow
unsigned x2 = pow(2, 63);  // GCC: overflow
uint64_t x3 = 1   << 63;   // GCC: shift count >= type width
*uint64_t x4 = 1ul << 63;
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

**Results with Clang**
```
-1657111256
0
0
9223372036854775808
```
]

]

---

class: center, middle, inverse

# **Intuitiveness**: Epic fail!

---
class: center, middle, inverse

# OK.  Let’s order food.

---

# At a Restaurant

For every item you order, say _the waiter_

- goes to a register

- checks availability

- makes an entry

- requests food preparation

- checks prepared items against register

- delivers to you

> **Terrible. Turnaround. Time**
>
> You get hungry + angry → `hangry` 😠


---

# At Home

For every item you order, say _Squiggy_

- goes to a register

- checks availability

- makes an entry

- requests food preparation

- checks prepared items against register

- delivers to you

> You tip the man **delivering 30 mins later**!

---

class: center, middle, inverse

# Why?

---

## Different Settings, Different Requirements

* At a restaurant, you expect a high turn-around time
  - Wasting time on **avoidable**, petty procedures means an overall delay in eating

* When ordering from home, you know it’ll take time
  - Wasting 5 mins is OK when it’s 30 mins b/w restaurant ↔ home (bottleneck elsewhere)

## Critical vs Non-critical Software

* .tag[Performance] When the network lag, I/O read latency or user input is the bottleneck, wasting a few milliseconds/operation is OK _e.g. running a VM, garbage collecting, etc._

* .tag[Flexibility] Since performance isn’t epitome, sacrificing finer control for better features is OK too! _e.g. well-polished data structures like in-built dictionaries, big integers, etc._

> Good engineers use the **right tool for the right job**.

---

# Browser is **system software**

.pull-left[
It’s complex and critical; comparable to a

* Kernel

* Game engine

* Compiler / virtual machine

* Real-time financial trading system
]
.pull-right[
> Wasting some minuscule time delta every operation = **systemic lag** hurting productivity.
]

.left[## C++]

* .tag[Performance] Gives very minimal abstraction thereby leading to native speeds
* .tag[Flexibility] Projects the underlying machine, doesn’t get in programmer’s way

---

class: center, middle, inverse

## That makes you a
# **System Programmer**
### Take pride — there aren’t many.

---

# Browser Expectations

* No lags
  - Used for almost every task: minor lags ≅ huge productivity loss
  - **Need**: Choosing infra without systemic lags
  - **Need**: Flexibility to have custom memory managers / allocators

* Feature abundance
  - Every user is unique: each would’ve have come to expect different things from it
  - **Need**: Different subsystems – made up of even different codebases – interface smoothly
  - **Need**: Access to various hardware-exposed features e.g. [SIMD](https://en.wikipedia.org/wiki/SIMD), AES instruction set, GPU, etc.

---

# Browser Expectations

* No memory leaks
  - Run the entire day: having leaks would mean hitting memory cap
  - **Need**: Stringent, controlled memory access patterns

* No security holes
  - It’d mean user’s personal data loss, money loss, etc.
  - **Need**: Access to underlying machine’s security features

---

## Built-in Data Types / Primitives

.pull-left[
### Integral Types.red[¹]

* `char`     (== 1 byte)
* `char16_t` (>= 2 bytes)
* `char32_t` (>= 4 bytes)
* `short`    (>= 2 bytes)
* `int`      (>= short)
* `long`     (>= 4 bytes)
* `long long` (>= 8 bytes)

]

.pull-right[
### Floating-point Types

* `float` (32-bit; IEEE-754 specified single-precision)
* `double` (64-bit; IEEE-754 specified double-precision)
* `long double` (unspecified: can be 80, 96 or 128 bits depending on compiler and machine)

]

### Other Types
- `void`
- `bool` (implementation-defined)
- Pointer types
  * Width: executable’s bit width; check `sizeof(void*)`
  * Arithmetic based on pointed-to type e.g. `int *p; ++p;` moves `p` by `sizeof(int)`
- Array types e.g. `int[2], char[6]`

.footnote[.red[¹] : variants of `unsigned` and `signed` — usually [two’s complement](https://en.wikipedia.org/wiki/Two%27s_complement)]

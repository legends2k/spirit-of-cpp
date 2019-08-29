
class: center, middle, inverse

# The Spirit of _C++_

## Sundaram Ramaswamy

---
.left-column[
  ## Why Use C++
  ### Why not Python, JavaScript or …
]

.right-column[
Why go through

- multiple data types
- raw _pointers_, contorted _references_
- manual memory management

.pull-left[
> … when you can "just be cool"
]

.pull-right[
```python
x = 2 ** 63  # 2 power 63
print(x)

> 9223372036854775808
```
]

How [C++](http://coliru.stacked-crooked.com/a/506ad2b09f29373d) fares against Python here?

``` c++
int      x1 = pow(2, 63);  // GCC: overflow
unsigned x2 = pow(2, 63);  // GCC: overflow
uint64_t x3 = 1   << 63;   // GCC: shift count >= type width
*uint64_t x4 = 1ul << 63;
```

.pull-left[
**Results with GCC**
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

# C++ Epic fail!

---
class: center, middle, inverse

# OK.  Let’s order food!

---

# At a Restaurant

For every item you order, _the waiter_

- goes to a register

- makes an entry

- requests food preparation

- checks prepared items against register

- delivers to you

> **Terrible. Turnaround. Time**
>
> You: hungry + angry = `hangry` 😠


---

# At Home

For every item you order, _Squiggy_

- goes to a register

- makes an entry

- requests food preparation

- checks prepared items against register

- delivers to you

> You tip the man for **delivering 30 mins later**!

---

class: center, middle, inverse

# What changed?

---

## Different Settings, Different Expectations

* At a restaurant, you expect a high turn-around time
  - Wasting time on petty procedures is frustrating

* When ordering from home, you know it’ll take time
  - Wasting 5 mins is OK when it’s 30 mins b/w restaurant ↔ home (bottleneck elsewhere)

> Browser is **system software**.

It’s as complex as a

* Kernel

* Game engine

* Compiler / virtual machine

* Real-time financial trading system

---

class: center, middle, inverse

# That makes you a _System Programmer_
## Take pride; there aren’t many

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

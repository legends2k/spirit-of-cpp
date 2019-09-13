# Differences w.r.t. Other Languages

* Most scripting languages run on a VM which on-the-fly translates calls to native machine leading to a small lag there too
* In scripting languages, every function call is `virtual` leading to a minute lag every function call
* All objects inherit from `System.Object`
  - Native datatype isn’t exposed
* Every variable is a pointer
* All data is in heap! Much faster stack is never utilized.

# Data: C++ vs Others

* All Python numbers are [big int](https://rushter.com/blog/python-integer-implementation/)s
* JavaScript’s only [number format](https://www.w3schools.com/js/js_numbers.asp) is a `double`
* Flexibility in each class deciding how memory allocations happen.
* No garbage collector so no performance hit but still having almost no leaks with RAII.
* Variables aren’t labels like in Python; variable and type are inseparable.

See Also: https://stackoverflow.com/questions/538551/handling-very-large-numbers-in-python

# Feedback

1. Show SSE2 register as comment
2. First explain about critical and non-critical then show other slides related to perf
4. Insert a usage slide in between _Virtual methods_ and _Memory Layout_; make a it a `IEditorPlugin` example
5. Don’t show, just tell: _C++ Constituents_?

# Organisation

* How C++ depends on a standard and not on an implementation
* Uniqueness of C++ in having undefined, implementation-{specific,defined}
  - Since it’s standardized, many platforms and compilers exist
  - All have to adhere to a standard version
  - Programmers bank on the guaranteed given by the standard to get predictable behaviour
* Difference between standard library vs third party library

# Performance

* On critical systems having infinitesimal lags in every operation leads to a systemic lag lowering productivity
* Systemic issues
> What distinguishes C++ from most other languages is that, while the other languages have implemented these abstractions at the cost of runtime performance, C++ has always strived to implement its abstractions at zero cost at runtime.
> Only language that provides programmers the ability to write an expressive and robust code base while still having the ability to target almost any hardware platform or real-time requirements.
* Show IT-Hare’s chart for operations

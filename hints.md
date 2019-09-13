# Introduction

* Show how newer languages are easier to C++.  Ask why still use C++?
  - Performance
  - Flexibility
    + finer control over facilities used
    + generated machine code
    +  memory footprint/usage pattern
  - Hardware access

# Performance

* Explain the difference between critical vs non-critical systems using **food ordering**
* Using the right tool for the right job
* Show having lag in python when doing server-side programming is OK since the bottleneck is elsewhere
* On critical systems having infinitesimal lags in every operation leads to a systemic lag lowering productivity
* Systemic issues
> Lesser the number of abstractions/layers between you and raw metal, the better.
* Explain C++ exposing the underlying machine openly by having minimal abstractions
* For the ones where abstractions are provided, they’re zero cost!
> What distinguishes C++ from most other languages is that, while the other languages have implemented these abstractions at the cost of runtime performance, C++ has always strived to implement its abstractions at zero cost at runtime.
> Only language that provides programmers the ability to write an expressive and robust code base while still having the ability to target almost any hardware platform or real-time requirements.
* C++ believes in programmer intelligence since it knows its speakers are system programmers or the like

# Data: C++ vs Others

* Explain scripting languages give more features but steer more and more away from the machine and lower performance e.g. bit int
* All Python numbers are [big int](https://rushter.com/blog/python-integer-implementation/)s, JavaScript’s only [number format](https://www.w3schools.com/js/js_numbers.asp) is a `double`
* Makes programmers life easy in calculating say `2^73 * 4` but internally [PyLong](https://docs.python.org/3/c-api/long.html#c.PyLongObject) is nothing but a C `struct` doing software emulation of an arbitrarily long integer object
* Flexibility is lost since all the time you mightn’t need a big int
* In most scripting languages, everything is an object i.e. there’s a wrapper around the real data, which also has metadata around the type, reflection-related data, etc.
* In C++, if you need big ints in a particular module, you can include a library only for it; it’ll provide a data type for which you can create an object. While native data types happily exists as POD.
* Most languages allow you to place objects only in the heap i.e. every variable is actually a pointer to an object in the heap.  C++ allows you to place them in the stack (much faster) or heap depending on need.
* Flexibility in each class deciding how memory allocations happen.
* No garbage collector so no performance hit but still having almost no leaks with RAII.
* Difference between language vs library
* Other languages give a map/dictionary, vector, etc. in-built into the language while C++ stay minimal
* Use a container of choice when you need it; even in that you’re not limited to the standard library
* Variables aren’t labels like in Python

See Also: https://stackoverflow.com/questions/538551/handling-very-large-numbers-in-python

# Code: C++ vs Others

* Most scripting languages run on a VM which on-the-fly translates calls to native machine leading to a small lag there too
* In scripting languages, every function call is `virtual` leading to a minute lag every function call
* Show IT-Hare’s chart for operations

# Organisation

* How C++ depends on a standard and not on an implementation
* Uniqueness of C++ in having undefined, implementation-{specific,defined}
  - Since it’s standardized, many platforms and compilers exist
  - All have to adhere to a standard version
  - Programmers bank on the guaranteed given by the standard to get predictable behaviour
* Difference between standard library vs third party library


# Feedback

1. Show SSE2 register as comment
2. _Translating to Software_ needs more clarification
3. First explain about critical and non-critical then show other slides related to perf
4. Give a concrete example of a non-critical software
5. Insert a usage slide inbetween _Virtual methods_ and _Memory Layout_; make a it a `IEditorPlugin` example
6. Don’t show, just tell _C++ Constituents_?

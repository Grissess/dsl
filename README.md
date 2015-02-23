dsl/2.0
===

*NOTE:* You are in the *2.0* branch; _this branch is not yet stable_. If you are looking for a stable release, use the master branch instead.

DSL (pronounced "diesel") is a simple C Data Structure Library in the vein of the C++ Standard Template Library. Presently, it supports:

* Arrays (corresponding to vectors) and array iterators
* Linked lists and list iterators
* Generic sequences (which may be either of the above) and generic iterators

More data structures will come soon (mostly as needed).

This library is intended to be at the core of various other software projects; for that reason, I will generally only try to push stable updates. My usual test battery is a fairly involved test of the data structure properties, which I run both in gdb (to track various faults) and valgrind (to track memory leaks). My goal is to have the library be totally stable and leak free; as I find bugs in other use cases, I will push patches as needed.

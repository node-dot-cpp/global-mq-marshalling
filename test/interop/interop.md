
interop1
--------
Basic tests including `MESSAGE` and `PUBLISHABLE`, for basic types, `STRUCTURES`, and `VECTOR`.

interop2
--------
Partial support for `DICTIONARY` and `DISCRIMINATED-UNION`, up to the point supported by original __cpptemplates__ generator.

interop3
--------
Remaining support for `DICTIONARY` and `DISCRIMINATED-UNION`, inluding mesages and publishables. Only __csharp__ and __cplusplus__ generators.

interop4
--------
Support for __soft__ dependency cycle of idl structures. By __soft__ we mean that one edge of the cycle is a `VECTOR of` or `DICTIONARY of` dependency and not a direct dependency.






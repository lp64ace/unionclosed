# About

This repository contains the Y Algorithm described in the paper about the "Union Closed Sets" as defined by Ηρακλής Χαλκίδης in C++.

NOTE: This is not an optimized version but the algorithm described on the paper translated to code, as it would otherwise defeat the purposes of this project. An optimized version may be released too if we (or you) deem it necessary.

# Generating Build Files

Make sure you have CMake installed on your system.

1. Clone this repository to your local machine:
```bash
git clone https://github.com/lp64ace/unionclosed.git
```

2. Navigate to the cloned directory:
```bash
cd unionclosed
```

3. Create a build directory (recommended to keep the source directory clean):
```bash
mkdir build & cd build
```

4. Generate the build files using CMake:
```base
cmake ..
```

# Example

The program generate a random family base that it will then use to create the full family using Union operations.

Sample Base
`{{1}, {2}, {3, 4, 5}, {5}, {6, 7, 8}, {9}, {10, 11}, {11}}`

The Family that will be produced will have a cardinality of `2^n` but with the following function you can edit the percentage of elements you want to leave outside the generated family that way the algorithm becomes more "interesting" in a way.
(The recommended setting for that value is `0.25` which would leave 1/4 of the elements out of the final Family)
```cpp
Set<Set<int>> Family = MakeFamilyFromBase(FamilyBase, 0.0);
```

# Conclusion

Enjoy.

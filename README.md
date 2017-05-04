# Version

C++ class to check if a version number is within an interval

## Dependencies for test

- [unittest-cpp](https://github.com/unittest-cpp/unittest-cpp)
- [cmake](https://cmake.org/)

## Intervals

  | Syntax        | Explanation |
  | ------------- | ----------- |
  | '4.2.0['      | 'x < 4.2.0' |
  | `4.2.0]'      | 'x <= 4.2.0' |
  | `]4.2.0'      | 'x > 4.2.0' |
  | `[4.2.0'      | 'x >= 4.2.0' |
  | `4.2.0'       | 'x == 4.2.0' |
  | `[4.2.0]'     | 'x == 4.2.0' |
  | `]4.2.0['     | 'x != 4.2.0' |
  | `]2.6,2.5['   | 'x > 2.6.0 || x < 2.5' |
  | `1.0,2.0'     | 'x in (2.0, 1.0)' |
  | `[2.6.0-2.8[' | '2.6.0 >= x < 2.8' |

## Build

create build directory
> mkdir build
change to this directory
> cd build
build with cmake
> cmake -DUNITTESTCPP_DIR="C:/Users/Jimmy/Workspaces/Cpp/unittest-cpp-master" .. && cmake --build . && msbuild t_version.sln
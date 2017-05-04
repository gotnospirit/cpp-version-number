mkdir build
cd build
cmake -DUNITTESTCPP_DIR="C:/Users/Jimmy/Workspaces/Cpp/unittest-cpp-master" .. && cmake --build . && msbuild t_version.sln

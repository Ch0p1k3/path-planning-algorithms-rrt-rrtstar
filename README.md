# PathPlanningAlgorithms(RRT, RRT*)


## Getting started
### Linux and Mac
Release building
```bash
cd PathPlanningAlgorithms-RRT-RRTstar-
cmake . -DCMAKE_BUILD_TYPE="Release" -Bbuild
cd build
make install
```

Debug building
```bash
cd PathPlanningAlgorithms-RRT-RRTstar-
cmake . -DCMAKE_BUILD_TYPE="Debug" -Bbuild
cd build
make install
```

### Windows
Release building
```cmd
cd PathPlanningAlgorithms-RRT-RRTstar-
cmake . -DCMAKE_BUILD_TYPE="Release" -Bbuild -G"MinGW Makefiles"
cd build
mingw32-make install
```

Debug building
```cmd
cd PathPlanningAlgorithms-RRT-RRTstar-
cmake . -DCMAKE_BUILD_TYPE="Debug" -Bbuild -G"MinGW Makefiles"
cd build
mingw32-make install
```
## Visualizer
# RRTAlgorithm::launchWithVirt
![dao_arena](./images/dao_arena.png)
![dao_arena2](./images/dao_arena2.png)
# RRTAlgorithm::launchWithVirtAfter
![moscow_0_1024_after](./images/moscow_0_1024_after.png)
# RRTAlgorithm::launchWithVirtAfterWithoutTree
![moscow_0_1024_after_without_tree](./images/moscow_0_1024_after_without_tree.png)
# README #

## General structure ##

* `doc` is for documentation, report, etc.
    * `doc/examples` contains example reports for similar projects.
    * `doc/related-work` contains related documentation for our project.
    * `doc/report/project-report.tex` is the most documented example template in LaTeX, single column, more readable. 
    * `doc/team6/team6_report.pdf` the pdf document for the final report containing the design and documentation. 
* `src` contains the source code for the project
    * `src/Project` contains the source code and solution for the project
    * `src/Project/Cloth Simulation/` contains a sample project of a cloth simulation made by Dayeol Lee (more on this in our report)
    * `src/Project/x64/Debug` contains a compiled executable of our project

### Summary

This animation project simulates a mesh being burned away by a fire particle system which spreads from point to point on a grid.

### Set Up ###

* `src/Dependencies` containts the necessary libraries to link to the project
* `src/Project/base-code.sln` is the Visual Studio 2017 solution for the project (Windows SDK 10)
    * `freeglut\include` must be added to the additional include directories of the project in the properties (C/C++ settings)
    * `freeglut\lib\x64` must be added to the additional library directories of the project in the properties (Linker settings)
    * `glm` header is also needed but already included in the code as `#include "../Dependencies/glm/glm.hpp"`
* `src/Project/x64/Debug/base-code.exe` is an executable file of the project compiled for a 64 bits system (requires freeglut.dll in the same directory)

### Contributions ###

* Erik Smith, e_ith - 40002747
* Mark-Zackary Rodrigues-Gillich, Mark_rod - 27037155
* Cynthia cherrier, c_herrie - 40005808
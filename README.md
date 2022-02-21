# modelviewer-mf6
Model Viewer for MODFLOW 6 Graphical User Interface

This is the development repository for the USGS Model Viewer for MODFLOW 6 Graphical User Interface. The official USGS distribution is available at [USGS Release Page](https://www.usgs.gov/software/model-viewer-program-three-dimensional-visualization-ground-water-model-results).

# Building Model Viewer for MODFLOW 6

## Prerequisite Software

* 7-Zip 
* CMake 3.20 or newer
* Git 2.31 or newer
* Microsoft Visual Studio 2019

## Building

Open a command prompt window and download the precompiled VTK 6.3.0 libraries
      
      cd %USERPROFILE%\Downloads
      curl -L -O https://github.com/MODFLOW-USGS/modelviewer-mf6/releases/download/vtk-6.3.0-vs2017-x64/vtk-6.3.0-vs2017-x64.7z

Using 7-zip unzip the vtk libraries

      cd /D C:\
      7z x %USERPROFILE%\Downloads\vtk-6.3.0-vs2017-x64.7z
      
Clone your fork of modelviewer-mf6

      mkdir %USERPROFILE%\source\repos
      cd %USERPROFILE%\source\repos
      git clone git@github.com:<YOUR-GITHUB-USER-NAME>/modelviewer-mf6.git

Use CMake to configure

      cd modelviewer-mf6
      cmake --preset vs2019
      
Use CMake to build

      cmake --build --preset vs2019

Use CMake to open the project in visual studio

      cmake --open _vs2019
      
Disclaimer
----------

This software is preliminary or provisional and is subject to revision. It is
being provided to meet the need for timely best science. The software has not
received final approval by the U.S. Geological Survey (USGS). No warranty,
expressed or implied, is made by the USGS or the U.S. Government as to the
functionality of the software and related material nor shall the fact of release
constitute any such warranty. The software is provided on the condition that
neither the USGS nor the U.S. Government shall be held liable for any damages
resulting from the authorized or unauthorized use of the software.

# modelviewer-mf6
Model Viewer for MODFLOW 6 Graphical User Interface

This is the development repository for the USGS Model Viewer for MODFLOW 6 Graphical User Interface. The official USGS distribution is available at [USGS Release Page](https://www.usgs.gov/software/model-viewer-program-three-dimensional-visualization-ground-water-model-results).

# Building Model Viewer for MODFLOW 6

## Prerequisites

* CMake 3.21 or newer
* Qt 5.15.2
* VTK 9.1.0
* Ninja build system
* Visual Studio 2019 64-bit (windows)
* gcc 64-bit (linux)
* clang 64-bit (macOS)

## Building

Install Qt 5.15.2 for open source use (https://www.qt.io/download)

Download the VTK 9.1.0 Source (https://github.com/Kitware/VTK/releases/tag/v9.1.0)

Download the modelviewer-mf6 source version 1.1 or later

Copy the misc/CMakePresets-vtk-9.1.0-qt-5.15.2.json file to the top-level
of the VTK source tree and rename it to CMakePresets.json.

Set the following environment variables or manually edit
the default presets in the CMakePresets.json file:

      $env{RUNNER_WORKSPACE}  = <location to build vtk>
      $env{VTK_VER}           = 9.1.0
      $env{AQT_VER}           = 5.15.2

On Linux run

      cmake --preset gcc_64
      cmake --build --preset gcc_64 --config Release
      cmake --install $RUNNER_WORKSPACE/qt5-vtk-$VTK_VER-build --config Release

On macOS run

      cmake --preset clang_64
      cmake --build --preset clang_64 --config Release
      cmake --install $RUNNER_WORKSPACE/qt5-vtk-$VTK_VER-build --config Release

On Windows run

      cmake --preset win64_msvc2019_64
      cmake --build --preset win64_msvc2019_64 --config Release
      cmake --install %RUNNER_WORKSPACE%/qt5-vtk-%VTK_VER%-build --config Release

Download (or clone) the modelviewer-mf6 source

Within the top-level directory of modelviewer-mf6 run

      cmake -S . -B ./_ninja_multi -G "Ninja Multi-Config"
      cmake --build ./_ninja_multi --config Release


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

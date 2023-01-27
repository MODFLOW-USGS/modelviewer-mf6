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
* curl

## Building

Install Qt 5.15.2 for open source use (https://www.qt.io/download)

Download the VTK 9.1.0 Source (https://github.com/Kitware/VTK/releases/tag/v9.1.0)

Download the modelviewer-mf6 source version 1.1 or later

Copy the misc/CMakePresets-vtk-9.1.0-qt-5.15.2.json file to the top-level
of the VTK source tree and rename it to CMakePresets.json.

Set the following environment variables or manually edit
the default presets in the CMakePresets.json file:

```cmake
$env{RUNNER_WORKSPACE}  = <location to build vtk>
$env{VTK_VER}           = 9.1.0
$env{AQT_VER}           = 5.15.2
$env{MV_VER}            = 1.1.x
```

On Linux run

```Bash
# set environment vars
export RUNNER_WORKSPACE=${HOME}/work/modelviewer-mf6
export VTK_VER=9.1.0
export AQT_VER=5.15.2
export MV_VER=1.1.0   # greater than or equal to 1.1.0

# setup work space
mkdir -p ${RUNNER_WORKSPACE}
cd ${RUNNER_WORKSPACE}

# download and extract mvmf6 source
curl -fSLO https://github.com/MODFLOW-USGS/modelviewer-mf6/archive/refs/tags/${MV_VER}.tar.gz
tar xzf ./${MV_VER}.tar.gz

# download and extract VTK source
curl -fSLO https://github.com/Kitware/VTK/archive/refs/tags/v${VTK_VER}.tar.gz
tar xzf ./v${VTK_VER}.tar.gz

# copy cmake preset file to VTK directory
cp ./modelviewer-mf6-${MV_VER}/misc/CMakePresets-vtk-${VTK_VER}-qt-${AQT_VER}.json ./VTK-${VTK_VER}/CMakePresets.json
cd ./VTK-${VTK_VER}

# use cmake to build VTK with Qt
cmake --preset gcc_64
cmake --build --preset gcc_64 --config Release
cmake --install ${RUNNER_WORKSPACE}/qt5-vtk-${VTK_VER}-build --config Release
```

On macOS run

```Bash
# set environment vars
export RUNNER_WORKSPACE=${HOME}/work/modelviewer-mf6
export VTK_VER=9.1.0
export AQT_VER=5.15.2
export MV_VER=1.1.0   # greater than or equal to 1.1.0

# setup work space
mkdir -p ${RUNNER_WORKSPACE}
cd ${RUNNER_WORKSPACE}

# download and extract mvmf6 source
curl -fSLO https://github.com/MODFLOW-USGS/modelviewer-mf6/archive/refs/tags/${MV_VER}.tar.gz
tar xzf ./${MV_VER}.tar.gz

# download and extract VTK source
curl -fSLO https://github.com/Kitware/VTK/archive/refs/tags/v${VTK_VER}.tar.gz
tar xzf ./v${VTK_VER}.tar.gz

# copy cmake preset file to VTK directory
cp ./modelviewer-mf6-${MV_VER}/misc/CMakePresets-vtk-${VTK_VER}-qt-${AQT_VER}.json ./VTK-${VTK_VER}/CMakePresets.json
cd ./VTK-${VTK_VER}

# use cmake to build VTK with Qt
cmake --preset clang_64
cmake --build --preset clang_64 --config Release
cmake --install $RUNNER_WORKSPACE/qt5-vtk-$VTK_VER-build --config Release
```

On Windows run

PowerShell

```PowerShell
# set environment vars
${env:RUNNER_WORKSPACE}="${env:USERPROFILE}\work\modelviewer-mf6"
${env:VTK_VER}="9.1.0"
${env:AQT_VER}="5.15.2"
${env:MV_VER}="1.1.0"   # greater than or equal to 1.1.0

# setup work space
mkdir ${env:RUNNER_WORKSPACE}
cd ${env:RUNNER_WORKSPACE}

# download and extract source codes
curl -fSLO https://github.com/MODFLOW-USGS/modelviewer-mf6/archive/refs/tags/${env:MV_VER}.zip
7z x "${env:MV_VER}.zip"
curl -fSLO https://github.com/Kitware/VTK/archive/refs/tags/v${env:VTK_VER}.zip
7z x "v${env:VTK_VER}.zip"

# copy cmake preset file to VTK directory
cd .\VTK-${env:VTK_VER}
cp ..\modelviewer-mf6-${env:MV_VER}\misc\CMakePresets-vtk-${env:VTK_VER}-qt-${env:AQT_VER}.json .\CMakePresets.json

# use cmake to build VTK with Qt
cmake --preset win64_msvc2019_64
cmake --build --preset win64_msvc2019_64 --config Release
cmake --install ${env:RUNNER_WORKSPACE}/qt5-vtk-${env:VTK_VER}-build --config Release

# build mvmf6
cmake --preset win64_msvc2019_64
cmake --build --preset win64_msvc2019_64 --config Release
cmake --install _ninja --config Release
```

CMD

```dos
:: set environment vars
set RUNNER_WORKSPACE=%USERPROFILE%\work\modelviewer-mf6
set VTK_VER=9.1.0
set AQT_VER=5.15.2
set MV_VER=1.1.0

:: copy cmake preset file to VTK directory
cd .\VTK-%VTK_VER%
cp .\modelviewer-mf6-%MV_VER%\misc\CMakePresets-vtk-%VTK_VER%-qt-%AQT_VER%.json .\CMakePresets.json

:: use cmake to build VTK with Qt
cmake --preset win64_msvc2019_64
cmake --build --preset win64_msvc2019_64 --config Release
cmake --install %RUNNER_WORKSPACE%\qt5-vtk-%VTK_VER%-build --config Release
```

Build mvmf6

For all OSs set the following environment variables

      $env{VTK_DIR} = $env{RUNNER_WORKSPACE}/qt5-vtk-$env{VTK_VER}/lib/cmake
      $env{Qt5_DIR} = /home/runner/work/modelviewer-mf6/Qt/5.15.2/gcc_64

      Linux
      export Qt5_DIR=$RUNNER_WORKSPACE/Qt/$AQT_VER/gcc_64


Download (or clone) the modelviewer-mf6 source

Within the top-level directory of modelviewer-mf6 run

On Linux

      cmake --preset gcc_64
      cmake --build --preset gcc_64 --config Release
      cmake --install _ninja --config Release

## Installing precompiled versions

On Linux run

```bash
cd ~/Downloads
export MV_VER=1.1.0
curl https://github.com/MODFLOW-USGS/modelviewer-mf6/releases/download/1.1.0/mvmf6-${MV_VER}-Linux.tar.gz
cd ~/.local
tar xzf ~/Downloads/mvmf6-${MV_VER}-Linux.tar.gz --strip-components=1
# Note
# may need to install libxcb-xinerama0
# sudo apt-get install libxcb-xinerama0      
```

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

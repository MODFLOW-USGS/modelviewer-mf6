#ifndef __mvDefine_h
#define __mvDefine_h

#define MV_VERSION "MF6"

enum class SolidDisplayType {
    MV_SOLID_NOT_DEFINED = -1,
    MV_SOLID_SMOOTH      = 0,
    MV_SOLID_BLOCKY      = 1,
    MV_SOLID_BANDED      = 2
};

#define MV_GRID_INTERPOLATED 0
#define MV_GRID_STAIRSTEPPED 1

#define MV_PATHLINE_TUBE     0
#define MV_PATHLINE_LINE     1

enum class ScalarMode {
    MV_CELL_SCALARS  = 0,
    MV_POINT_SCALARS = 1
};

enum class GridType {
    MV_GRID_NOT_DEFINED  = 0,
    MV_STRUCTURED_GRID   = 4, // DIS6
    MV_LAYERED_GRID      = 5, // DISV6
    MV_UNSTRUCTURED_GRID = 6  // DISU6
};

enum class OverlayType {
    otShapeFile = 0,            // MV_ESRI_SHAPEFILE_OVERLAY
    otDXFFile   = 1             // MV_DXF_ASCII_OVERLAY
};

#define MV_INITIAL_DISPLAY_BOUNDING_BOX 0
#define MV_INITIAL_DISPLAY_GRID_OUTLINE 1

#define MV_USE_CELL_DATA_FOR_RANGE      0
#define MV_USE_POINT_DATA_FOR_RANGE     1

#define MP_TRAVEL_TIME                  0
#define MP_MIN_TRAVEL_TIME              1
#define MP_MAX_TRAVEL_TIME              2

#endif // __mvDefine_h

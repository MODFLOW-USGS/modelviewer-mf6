#ifndef __mvGUISettings_h
#define __mvGUISettings_h

//#include "mvUtil.h"
#include "mvHeader.h"
#include <fstream>

using std::ofstream;

class mvHashTable;

class MV_EXPORT mvGUISettings
{
public:
    // Crop
    double cropBoundsXDelta;
    double cropBoundsYDelta;
    double cropBoundsZDelta;

    // Animation
    double animationRotate;
    double animationElevate;
    double animationDelay;

    // Lighting - lights
    int    headlightOn;
    int    auxiliaryLightOn;
    double headlightIntensity;
    double auxiliaryLightIntensity;
    double auxiliaryLightDirection[3];

    // Lighting -- background
    int    customBackground;
    double background[3];

    // Camera
    double cameraPosition[3];
    double focalPoint[3];
    double viewUp[3];
    int    parallelProjection;
    double parallelScale;

public:
    mvGUISettings();

    int Serialize(ofstream *out) const;
    int Deserialize(mvHashTable *hashTable);
};

#endif

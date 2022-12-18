#include "mvGUISettings.h"
#include "mvHashTable.h"

using std::endl;

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvGUISettings::mvGUISettings()
{
    // Crop
    cropBoundsXDelta           = 0.1;
    cropBoundsYDelta           = 0.1;
    cropBoundsZDelta           = 0.1;

    // Animation
    animationRotate            = 0;
    animationElevate           = 0;
    animationDelay             = 0;

    // Lights
    headlightOn                = 1;
    auxiliaryLightOn           = 0;
    headlightIntensity         = 1;
    auxiliaryLightIntensity    = 0;
    auxiliaryLightDirection[0] = 0;
    auxiliaryLightDirection[1] = 0;
    auxiliaryLightDirection[2] = 1;

    // Background
    customBackground           = 0;
    background[0]              = 1;
    background[1]              = 1;
    background[2]              = 1;

    // Camera
    cameraPosition[0]          = 0;
    cameraPosition[1]          = 0;
    cameraPosition[2]          = 1;
    focalPoint[0]              = 0;
    focalPoint[1]              = 0;
    focalPoint[2]              = 0;
    viewUp[0]                  = 0;
    viewUp[1]                  = 1;
    viewUp[2]                  = 0;
    parallelProjection         = 0;
    parallelScale              = 1;
}

int mvGUISettings::Serialize(ofstream *out) const
{
    // Crop
    (*out) << "Crop bounds x inc = " << cropBoundsXDelta << endl;
    (*out) << "Crop bounds y inc = " << cropBoundsYDelta << endl;
    (*out) << "Crop bounds z inc = " << cropBoundsZDelta << endl;

    // Animation
    (*out) << "Animation rotate = " << animationRotate << endl;
    (*out) << "Animation elevate = " << animationElevate << endl;
    (*out) << "Animation delay = " << animationDelay << endl;

    // Lights
    (*out) << "Headlight on = " << headlightOn << endl;
    (*out) << "Headlight intensity = " << headlightIntensity << endl;
    (*out) << "Auxiliary light on = " << auxiliaryLightOn << endl;
    (*out) << "Auxiliary light intensity = " << auxiliaryLightIntensity << endl;
    (*out) << "Auxiliary light direction x = " << auxiliaryLightDirection[0] << endl;
    (*out) << "Auxiliary light direction y = " << auxiliaryLightDirection[1] << endl;
    (*out) << "Auxiliary light direction z = " << auxiliaryLightDirection[2] << endl;

    // background
    (*out) << "Background custom = " << customBackground << endl;
    (*out) << "Background red = " << background[0] << endl;
    (*out) << "Background green = " << background[1] << endl;
    (*out) << "Background blue = " << background[2] << endl;

    // Camera -- write these with double precision
    int oldprecision = (int)out->precision(12);
    (*out) << "Camera position x = " << cameraPosition[0] << endl;
    (*out) << "Camera position y = " << cameraPosition[1] << endl;
    (*out) << "Camera position z = " << cameraPosition[2] << endl;
    (*out) << "Focal point x = " << focalPoint[0] << endl;
    (*out) << "Focal point y = " << focalPoint[1] << endl;
    (*out) << "Focal point z = " << focalPoint[2] << endl;
    (*out) << "View up x = " << viewUp[0] << endl;
    (*out) << "View up y = " << viewUp[1] << endl;
    (*out) << "View up z = " << viewUp[2] << endl;
    (*out) << "Parallel projection = " << parallelProjection << endl;
    (*out) << "Parallel scale = " << parallelScale << endl;
    out->precision(oldprecision);

    return 1;
}

int mvGUISettings::Deserialize(mvHashTable *hashTable)
{
    hashTable->GetHashTableValue("Crop bounds x inc", cropBoundsXDelta);
    hashTable->GetHashTableValue("Crop bounds y inc", cropBoundsYDelta);
    hashTable->GetHashTableValue("Crop bounds z inc", cropBoundsZDelta);

    // Animation
    hashTable->GetHashTableValue("Animation rotate", animationRotate);
    hashTable->GetHashTableValue("Animation elevate", animationElevate);
    hashTable->GetHashTableValue("Animation delay", animationDelay);

    // Lights
    hashTable->GetHashTableValue("Headlight on", headlightOn);
    hashTable->GetHashTableValue("Headlight intensity", headlightIntensity);
    hashTable->GetHashTableValue("Auxiliary light on", auxiliaryLightOn);
    hashTable->GetHashTableValue("Auxiliary light intensity", auxiliaryLightIntensity);
    double f1, f2, f3;
    if (hashTable->GetHashTableValue("Auxiliary light direction x", f1) && hashTable->GetHashTableValue("Auxiliary light direction y", f2) && hashTable->GetHashTableValue("Auxiliary light direction z", f3))
    {
        auxiliaryLightDirection[0] = f1;
        auxiliaryLightDirection[1] = f2;
        auxiliaryLightDirection[2] = f3;
    }

    // background
    hashTable->GetHashTableValue("Background custom", customBackground);
    if (hashTable->GetHashTableValue("Background red", f1) && hashTable->GetHashTableValue("Background green", f2) && hashTable->GetHashTableValue("Background blue", f3))
    {
        background[0] = f1;
        background[1] = f2;
        background[2] = f3;
    }

    // Camera
    double d1, d2, d3;
    if (hashTable->GetHashTableValue("Camera position x", d1) && hashTable->GetHashTableValue("Camera position y", d2) && hashTable->GetHashTableValue("Camera position z", d3))
    {
        cameraPosition[0] = d1;
        cameraPosition[1] = d2;
        cameraPosition[2] = d3;
    }
    if (hashTable->GetHashTableValue("Focal point x", d1) && hashTable->GetHashTableValue("Focal point y", d2) && hashTable->GetHashTableValue("Focal point z", d3))
    {
        focalPoint[0] = d1;
        focalPoint[1] = d2;
        focalPoint[2] = d3;
    }
    if (hashTable->GetHashTableValue("View up x", d1) && hashTable->GetHashTableValue("View up y", d2) && hashTable->GetHashTableValue("View up z", d3))
    {
        viewUp[0] = d1;
        viewUp[1] = d2;
        viewUp[2] = d3;
    }
    hashTable->GetHashTableValue("Parallel projection", parallelProjection);
    hashTable->GetHashTableValue("Parallel scale", parallelScale);

    return 1;
}

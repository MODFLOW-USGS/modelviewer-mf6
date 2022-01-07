#ifndef __mvAxes_h
#define __mvAxes_h

#include "mvDisplayObject.h"

class vtkAxes;
class vtkTubeFilter;
class vtkLookupTable;

/**
 * Encapsulates a cartesian (x-y-z) axis system. The axes are
 * represented by either tubes or line colored according to
 * red = x, green = y, blue = z)
 */
class MV_EXPORT mvAxes : public mvDisplayObject  
{
public:
	mvAxes();
	virtual ~mvAxes();

	void SetDefaultPositions(const double *p);
	void SetDefaultSize(double s);
	void SetDefaultTubeDiameter(double d);
	void SetNormalizedPosition(double x, double y, double z);
	void SetNormalizedSize(double size);
	void SetNormalizedTubeDiameter(double d);
	void SetRepresentationToTube();
	void SetRepresentationToLine();
	double GetNormalizedSize() const;
	double GetNormalizedTubeDiameter() const;
	const double *GetNormalizedPosition() const;
	int GetRepresentation() const;

private:
	vtkAxes *m_Axes;
	vtkTubeFilter *m_Tube;
	vtkLookupTable *m_LookupTable;

	double m_DefaultPositions[6];
	double m_NormalizedPosition[3];
	double m_DefaultTubeDiameter;
	double m_DefaultSize;
};

#endif

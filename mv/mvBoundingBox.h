#ifndef __mvBoundingBox_h
#define __mvBoundingBox_h

#include "mvDisplayObject.h"

class vtkOutlineSource;

class MV_EXPORT mvBoundingBox : public mvDisplayObject  
{
public:
	mvBoundingBox();
	virtual ~mvBoundingBox();

	void SetBounds(const double *bounds);
	const double *GetBounds() const;
	void GetBounds(double *bounds) const;

protected:
	vtkOutlineSource *m_Outline;
};

#endif

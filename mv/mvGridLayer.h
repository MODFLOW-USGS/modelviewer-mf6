#ifndef __mvGridLayer_h
#define __mvGridLayer_h

#include "mvDisplayObject.h"

class vtkExtractEdges;
class vtkPolyData;
class mvDataSource;
class vtkThreshold;

class MV_EXPORT mvGridLayer : public mvDisplayObject
{
public:
	mvGridLayer();
	virtual ~mvGridLayer();
   void SetDataSource(mvDataSource *dataSource);
   void Build();
   void SetLayerPosition(int layerPosition);
   int GetLayerPosition() const;


protected:
   vtkExtractEdges *m_GridLayer;
   mvDataSource *m_DataSource;
   vtkThreshold *m_Threshold;
   int m_LayerPosition;
};

#endif

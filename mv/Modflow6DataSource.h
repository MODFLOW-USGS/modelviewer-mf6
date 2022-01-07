#ifndef __Modflow6DataSource_h
#define __Modflow6DataSource_h

#include "mvDataSource.h"
#include "mvModelFeatures.h"
#include "mvLinkList.h"
#include "mvDefine.h"

using namespace std;

class MV_EXPORT Modflow6DataSource : public mvDataSource
{
public:
   Modflow6DataSource();
   virtual ~Modflow6DataSource();

   static char *GetNameStatic() {return "Modflow 6";}
   virtual char *GetName() {return GetNameStatic();}
   virtual int GetPrimaryScalarMode() { return MV_CELL_SCALARS; }
   virtual int GetGridType() { return m_GridType; }
//   virtual int AreAllCellsActive() { return 0; }
   virtual char *LoadData(char *dataFileList);
   virtual void AdvanceOneTimePoint() { SetTimePointTo(-1); }
   virtual void SetTimePointTo(int timePointIndex);
   virtual void SetScalarDataTypeTo(int dataTypeIndex) {/*Not used*/}
   virtual int GetModelFeatureDisplayMode() {return MV_DISPLAY_MODEL_FEATURES_AS_CELLS;}
   virtual void GetDefaultModelFeatureColor(int i, double *rgba);

   virtual int GetNumVTKPoints();
   virtual int GetNumVTKCells();
   virtual int GetNumModelCells();

   virtual int GetNumberOfCellLayers() {return m_NumberOfCellLayers;}
   virtual int GetNumberOfVTKCellsInLayer(int k);

   virtual int *GetConnectivityArray() {return m_ConnectivityArray;}
   virtual int *GetStairsteppedGridConnectivity() {return m_StairsteppedGridConnectivity;}
   virtual int *GetLayerConnectivity() {return m_LayerConnectivity;}
   virtual int GetStairsteppedGridCoordinatesSize() {return 3*m_NumberOfVTKPointsForStairsteppedGrid;}

protected:

   int m_NumberOfVTKPoints;
   int m_NumberOfVTKCells;
   int m_NumberOfCellColumns;
   int m_NumberOfCellRows;
   int m_NumberOfCellLayers;
   int m_NumberOfModflowCells;
   int m_ModelFeatureArraySize;
   int m_GridType;
   int m_HasSpecificDischargeData;
   ifstream m_IfHead;
   ifstream m_IfBudget;
   int *m_Icelltype;
   int *m_Idomain;
   int *m_Iavert;
   int *m_Javert;
   int *m_NumCell2dVert;
   int *m_Ia;
   int *m_Ja;
   int *m_NumFlowConn;
   int m_Nvert;
   int m_Ncpl;
   int m_NumberOfBudgetItems;
   int **m_MfCell2VtkCells;
   int *m_ConnectivityArray;
   int *m_LayerConnectivity;
   int *m_StairsteppedGridConnectivity;
   double **m_Rgba;
   double m_DryCellValue;
   mvLinkList *m_BudgetText;
   mvLinkList *m_PeriodStep;
   int m_StairsteppedGridCoordinatesSize;
   int *m_NumberOfVTKCellsInLayer;

   int m_NumberOfVTKPointsForStairsteppedGrid;

	double *m_Delr;
	double *m_Delc_revdir;
	double *m_Elev;

   char *ExtractModflowOutputFileNames(char *nameFile, char *gridFile, char *headFile, char *budgetFile);
   char *CreateDisGrid(char *gridFile);
   char * CreateDisvGrid(char *gridFile);
   char *CreateDisuGrid(char *gridFile);
   char *CountHead(char* dataType);
   void GetTimePoints(double *timePoints, int *periods, int *steps);
   char *CountBudgetAndFeatures();
   int PracticallyEqual(double value1, double value2);
   int IsModelFeature(char *flowType);
   void ExtractFileName(char *aString);

};

#endif
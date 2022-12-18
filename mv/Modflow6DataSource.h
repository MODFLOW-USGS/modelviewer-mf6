#ifndef __Modflow6DataSource_h
#define __Modflow6DataSource_h

#include "mvDataSource.h"
#include "mvModelFeatures.h"
#include "mvLinkList.h"
#include "mvDefine.h"

class MV_EXPORT Modflow6DataSource : public mvDataSource
{
public:
    Modflow6DataSource();
    ~Modflow6DataSource() override;

    static const char *GetNameStatic() { return "Modflow 6"; }
    const char        *GetName() override { return GetNameStatic(); }
    ScalarMode         GetPrimaryScalarMode() override { return ScalarMode::MV_CELL_SCALARS; }
    GridType           GetGridType() override { return m_GridType; }
    //   virtual int AreAllCellsActive() { return 0; }
    const char *LoadData(char *dataFileList) override;
    void        AdvanceOneTimePoint() override { SetTimePointTo(-1); }
    void        SetTimePointTo(int timePointIndex) override;
    void        SetScalarDataTypeTo(int dataTypeIndex) override { /* Not used */ }
    int  GetModelFeatureDisplayMode() override { return MV_DISPLAY_MODEL_FEATURES_AS_CELLS; }
    void GetDefaultModelFeatureColor(int i, double *rgba) override;

    int  GetNumVTKPoints() override;
    int  GetNumVTKCells() override;
    int  GetNumModelCells() override;

    double       GetXOrigin() const;
    double       GetYOrigin() const;
    double       GetAngRot() const;

    int  GetNumberOfCellLayers() override { return m_NumberOfCellLayers; }
    int  GetNumberOfVTKCellsInLayer(int k) override;

    int *GetConnectivityArray() override { return m_ConnectivityArray; }
    int *GetStairsteppedGridConnectivity() override { return m_StairsteppedGridConnectivity; }
    int *GetLayerConnectivity() override { return m_LayerConnectivity; }
    int  GetStairsteppedGridCoordinatesSize() override { return 3 * m_NumberOfVTKPointsForStairsteppedGrid; }

protected:
    int           m_NumberOfVTKPoints;
    int           m_NumberOfVTKCells;
    int           m_NumberOfCellColumns;
    int           m_NumberOfCellRows;
    int           m_NumberOfCellLayers;
    int           m_NumberOfModflowCells;
    int           m_ModelFeatureArraySize;
    GridType      m_GridType;
    int           m_HasSpecificDischargeData;
    std::ifstream m_IfHead;
    std::ifstream m_IfBudget;
    int *         m_Icelltype;
    int *         m_Idomain;
    int *         m_Iavert;
    int *         m_Javert;
    int *         m_NumCell2dVert;
    int *         m_Ia;
    int *         m_Ja;
    int *         m_NumFlowConn;
    int           m_Nvert;
    int           m_Ncpl;
    int           m_NumberOfBudgetItems;
    int **        m_MfCell2VtkCells;
    int *         m_ConnectivityArray;
    int *         m_LayerConnectivity;
    int *         m_StairsteppedGridConnectivity;
    double **     m_Rgba;
    double        m_DryCellValue;
    mvLinkList *  m_BudgetText;
    mvLinkList *  m_PeriodStep;
    int           m_StairsteppedGridCoordinatesSize;
    int *         m_NumberOfVTKCellsInLayer;

    int           m_NumberOfVTKPointsForStairsteppedGrid;

    double *      m_Delr;
    double *      m_Delc_revdir;
    double *      m_Elev;

    double        m_xorigin;
    double        m_yorigin;
    double        m_angrot;

    const char *  ExtractModflowOutputFileNames(char *nameFile, char *gridFile, char *headFile, char *budgetFile);
    const char *  CreateDisGrid(char *gridFile);
    const char *  CreateDisvGrid(char *gridFile);
    const char *  CreateDisuGrid(char *gridFile);
    const char *  CountHead(char *dataType);
    void          GetTimePoints(double *timePoints, int *periods, int *steps);
    const char *  CountBudgetAndFeatures();
    int           PracticallyEqual(double value1, double value2);
    int           IsModelFeature(char *flowType);
    void          ExtractFileName(char *aString);
};

#endif

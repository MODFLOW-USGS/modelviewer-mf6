// MvDoc.h : interface of the CMvDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MVDOC_H__D3A07DDA_F3BF_11D3_8105_00C04F61038F__INCLUDED_)
#define AFX_MVDOC_H__D3A07DDA_F3BF_11D3_8105_00C04F61038F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "vtkSmartPointer.h"

class mvManager;
class mvGUISettings;
class vtkPropCollection;

class CDataDlg;
class CColorBarDlg;
class CLightingDlg;
class CGridDlg;
class CGeometryDlg;
class CSolidDlg;
class CIsosurfaceDlg;
class CVectorDlg;
class CPathlinesDlg;
class CModelFeaturesDlg;
class CCropDlg;
class CAnimationDlg;
class COverlayDlg;

enum AnimationType {atTime, atSpace};

class CMvDoc : public CDocument
{
protected: // create from serialization only
	CMvDoc();
	DECLARE_DYNCREATE(CMvDoc)

// Attributes
public:

// Operations
public:

	// Methods call by CMvView
	void CreateToolDialogs();
	vtkSmartPointer<vtkPropCollection> GetPropCollection();
	int GetInteractorStyle() {return m_InteractorStyle;}
	BOOL IsAnimating() {return m_IsAnimating;}

	// Get methods
	char **GetTimePointLabels();
	int GetNumberOfTimePoints();
	void GetScalarDataRange(double *range);
	void GetVectorMagnitudeRange(double *range);
	char *GetModelName();
	char *GetDataName();
   int GetGridType();

	// Callback functions for modeless dialog boxes (tools)

	// Data
	void SetScalarDataTypeTo(int index);

	// Time Label
	void SetTimeLabelFontSize(int size, BOOL update = TRUE);
	int GetTimeLabelFontSize() const;
	void SetTimeLabelPosition(double x, double y, BOOL update = TRUE);
	const double *GetTimeLabelPosition() const;

	// Color Bar
	void SetColorBarEndPoints(double valueBlue, double valueRed);
	void UseLinearColorBar();
	void UseLogColorBar();
	void SetColorBarSize(int width, int height, int offset, BOOL update = TRUE);
	void SetColorBarFontSize(int fontSize, BOOL update = TRUE);
	void SetColorBarNumberOfLabels(int numLabels, BOOL update = TRUE);
	void SetColorBarLabelPrecision(int precision, BOOL update = TRUE);
	void SetColorBarTextColor(double red, double green, double blue, BOOL update = TRUE);
	void SetColorBarColorScheme(int Value);
	int GetColorBarWidth();
	int GetColorBarHeight();
	int GetColorBarOffset();
	int GetColorBarFontSize();
	int GetColorBarColorScheme();
	unsigned long GetColorBarFirstCustomColor();
	unsigned long GetColorBarLastCustomColor();
	void SetColorBarFirstCustomColor(unsigned long value);
	void SetColorBarLastCustomColor(unsigned long value);
	int GetColorBarSource();
	void SetColorBarSource(int value);
	double GetColorBarValueBlue() const;
	double GetColorBarValueRed() const;

	// Lighting
	void SetDiffuseLighting(double diffuse);
	void SetAmbientLighting(double ambient);
	void SetSpecularLighting(double specular);
	void SetSpecularPower(double specularPower);
	void SwitchOnHeadlight(BOOL switchOn);
	void SetHeadlightIntensity(double intensity);
	void SwitchOnAuxiliaryLight(BOOL switchOn);
	void SetAuxiliaryLightIntensity(double intensity);
	void SetAuxiliaryLightPosition(double x, double y, double z);
	void SetBackgroundColor(double red, double green, double blue);

	// Grid
	void ActivateGridLines(int slice, BOOL b);
	void SetGridLinePositions(int row, int col, int lay);
	void SetGridLineColor(double red, double green, double blue);
	void SetGridShellColor(double red, double green, double blue);
	void SetGridShellOpacity(double opacity);

   void SetGridLayerPosition(int layerNumber);
   void SetGridDisplayToInterpolated();
   void SetGridDisplayToStairstepped();

	// Geometry
	void SetScale(double xScale, double yScale, double zScale);
	void SetAxesRepresentationToLine();
	void SetAxesRepresentationToTube();
	void SetAxesProperties(double xPos, double yPos, double zPos, 
					double axesSize, double tubeDiameter);
	void SetBoundingBoxColor(double red, double green, double blue);
	void ApplySubgrid(int col_min, int col_max, int row_min, int row_max, int lay_min, int lay_max);
	void SubgridOff();

	// Solid
	void SetSolidDisplayToBlocky();
	void SetSolidDisplayToSmooth();
	void SetSolidDisplayToBanded();
	void ApplySolidControl(BOOL threshold, double minValue, double maxValue, int numberOfColorBands);

	// Isosurfaces
	void SetRegularIsosurfaces(int count, double valueMin, double valueMax);
	void SetCustomIsosurfaces(int count, double *values);

	// Vector
	void SetVectorScaleFactor(double scaleFactor);
	double GetVectorScaleFactor();
	void SetVectorSizeToOptimal();
	void SubsampleVectors(int col_min, int col_max, int col_rate, int row_min, int row_max, int row_rate, 
			int lay_min, int lay_max, int lay_rate);
	void SetVectorColor(double red, double green, double blue);
	void ActivateVectorGlyph(BOOL b);
	void EnlargeVectorGlyph();
	void ShrinkVectorGlyph();
	void ApplyVectorThreshold(double minValue, double maxValue);
	void VectorThresholdOff();
	int GetLogTransformVector();
	void SetLogTransformVector(int Value);
	void SetVectorLineWidth(double width);

	// Pathlines
	void SetPathlineRepresentationToLine();
	void SetPathlineRepresentationToTube();
	void SetPathlineTubeDiameter(double diameter);
	void SetPathlineColorBarEndPoints(double valueBlue, double valueRed);
	void SetPathlineTimeClippingMode(int mode);
	void SetPathlineTimeClippingRange(double minTime, double maxTime);
	void GetPathlineTimeRange(double *range);
	void SetPathlineLogTransform(int Value);
	int HasPathlineData() const;

	// Cropping
	void Crop(double xmin, double xMax, double yMin, double yMax, 
				double zMin, double zMax, double cropAngle);
	void ShowCroppedAwayPieces();
	void HideCroppedAwayPieces();
	void SetCroppedAwayPiecesColor(double red, double green, double blue);
	void SetCroppedAwayPiecesOpacity(double opacity);

	// Animation
	void StartAnimation(double delay);
	void Animate();
	void AdvanceOneTimePoint();
	void SetTimePointTo(int timePointIndex);
	void StopAnimation();
	void UpdateAnimationWithSameTime();
	void SetAnimationType(AnimationType value);
	void SetAnimationSteps(int value);

	// Model Feature
	void SetModelFeatureDisplayOrder(int *displayOrder);
	void EnlargeModelFeatureGlyphs();
	void ShrinkModelFeatureGlyphs();
	void SetModelFeatureColor(char *featureName, double *rgba);
	void GetModelFeatureColor(char *featureName, double *rgba);

	// Overlay
	void ApplyOverlayControl(char *filename, int overlayType, double xorig, double yorig,
		double scale, double angle, int drape, int trim, int crop, double elev, double drapeGap);
	void RemoveOverlay();
	int HasOverlay();

	void PrepareToClose();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMvDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	int GetAnimationSteps();
	AnimationType GetAnimationType();
	void CropVectors(double xmin, double xmax, 
		double ymin, double ymax, double zmin, double zmax, int cropangle);
	void UpdateColorBarDlg();
	void UpdatePathlinesDlg();
	virtual ~CMvDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	AnimationType m_AnimationType;
	int m_AnimationSteps;
	int m_InteractorStyle;
	int m_ProjectionMode;
	int m_NumberOfModels;
	int m_ReadyToClose;
	char **m_ModelNames;
	CString m_DefaultModel;
	BOOL m_Startup;
	BOOL m_IsAnimating;
	mvGUISettings *m_GUI;

	// The visualization pipeline manager
	mvManager *m_Manager;

	// Modeless dialog boxes
	CGridDlg *m_GridDlg;
	CColorBarDlg *m_ColorBarDlg;
	CLightingDlg *m_LightingDlg;
	CGeometryDlg *m_GeometryDlg;
	CDataDlg *m_DataDlg;
	CSolidDlg *m_SolidDlg;
	CIsosurfaceDlg *m_IsosurfaceDlg;
	CCropDlg *m_CropDlg;
	CAnimationDlg *m_AnimationDlg;
	CVectorDlg *m_VectorDlg;
	CPathlinesDlg *m_PathlinesDlg;
	CModelFeaturesDlg *m_ModelFeaturesDlg;
	COverlayDlg *m_OverlayDlg;

	// Protected methods;
	void LoadPreviousAppSettings();
	void SaveCurrentAppSettings();
	void UpdateToolDialogs(mvGUISettings *gui);
	void UpdateSolidDlg();
	void UpdateIsosurfaceDlg();
	void UpdateLightingDlg(mvGUISettings *gui);
	void UpdateGeometryDlg();
	void UpdateGridDlg();
	void UpdateVectorDlg();
	void UpdateCropDlg(mvGUISettings *gui);
	void UpdateDataDlg();
	void UpdateModelFeaturesDlg();
	void UpdateAnimationDlg(mvGUISettings *gui);
	void UpdateOverlayDlg();
	void UpdateAnimation();
	void UpdateAnimationPosition();

// Generated message map functions
protected:
	//{{AFX_MSG(CMvDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnParallelProjection();
	afx_msg void OnUpdateParallelProjection(CCmdUI* pCmdUI);
	afx_msg void OnPerspectiveProjection();
	afx_msg void OnUpdatePerspectiveProjection(CCmdUI* pCmdUI);
	afx_msg void OnPreferences();
	afx_msg void OnShowBoundingBox();
	afx_msg void OnUpdateShowBoundingBox(CCmdUI* pCmdUI);
	afx_msg void OnShowGridLines();
	afx_msg void OnUpdateShowGridLines(CCmdUI* pCmdUI);
	afx_msg void OnShowGridShell();
	afx_msg void OnUpdateShowGridShell(CCmdUI* pCmdUI);
	afx_msg void OnShowIsosurfaces();
	afx_msg void OnUpdateShowIsosurfaces(CCmdUI* pCmdUI);
	afx_msg void OnShowNone();
	afx_msg void OnUpdateShowNone(CCmdUI* pCmdUI);
	afx_msg void OnShowSolid();
	afx_msg void OnUpdateShowSolid(CCmdUI* pCmdUI);
	afx_msg void OnShowTime();
	afx_msg void OnUpdateShowTime(CCmdUI* pCmdUI);
	afx_msg void OnShowTitle();
	afx_msg void OnUpdateShowTitle(CCmdUI* pCmdUI);
	afx_msg void OnShowAxes();
	afx_msg void OnUpdateShowAxes(CCmdUI* pCmdUI);
	afx_msg void OnLightingTool();
	afx_msg void OnUpdateLightingTool(CCmdUI* pCmdUI);
	afx_msg void OnGridTool();
	afx_msg void OnUpdateGridTool(CCmdUI* pCmdUI);
	afx_msg void OnGeometryTool();
	afx_msg void OnUpdateGeometryTool(CCmdUI* pCmdUI);
	afx_msg void OnSolidTool();
	afx_msg void OnUpdateSolidTool(CCmdUI* pCmdUI);
	afx_msg void OnIsosurfaceTool();
	afx_msg void OnUpdateIsosurfaceTool(CCmdUI* pCmdUI);
	afx_msg void OnCropTool();
	afx_msg void OnUpdateCropTool(CCmdUI* pCmdUI);
	afx_msg void OnAnimationTool();
	afx_msg void OnUpdateAnimationTool(CCmdUI* pCmdUI);
	afx_msg void OnColorBarTool();
	afx_msg void OnUpdateColorBarTool(CCmdUI* pCmdUI);
	afx_msg void OnShowColorBar();
	afx_msg void OnUpdateShowColorBar(CCmdUI* pCmdUI);
	afx_msg void OnLoadData();
	afx_msg void OnShowVectors();
	afx_msg void OnUpdateShowVectors(CCmdUI* pCmdUI);
	afx_msg void OnVectorTool();
	afx_msg void OnUpdateVectorTool(CCmdUI* pCmdUI);
	afx_msg void OnDataTool();
	afx_msg void OnUpdateDataTool(CCmdUI* pCmdUI);
//	afx_msg void OnAllActiveCells();
	afx_msg void OnUpdateAllActiveCells(CCmdUI* pCmdUI);
	afx_msg void OnShowModelFeatures();
	afx_msg void OnUpdateShowModelFeatures(CCmdUI* pCmdUI);
	afx_msg void OnModelFeaturesTool();
	afx_msg void OnUpdateModelFeaturesTool(CCmdUI* pCmdUI);
	afx_msg void OnShowPathlines();
	afx_msg void OnUpdateShowPathlines(CCmdUI* pCmdUI);
	afx_msg void OnPathlinesTool();
	afx_msg void OnUpdatePathlinesTool(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCustomFileClose(CCmdUI* pCmdUI);
	afx_msg void OnShowOverlay();
	afx_msg void OnUpdateShowOverlay(CCmdUI* pCmdUI);
	afx_msg void OnOverlayTool();
	afx_msg void OnUpdateOverlayTool(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MVDOC_H__D3A07DDA_F3BF_11D3_8105_00C04F61038F__INCLUDED_)

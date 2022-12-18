#ifndef __mvColorBar_h
#define __mvColorBar_h

#include "mvHeader.h"
#include "vtkActor2D.h"
#include "vtkLookupTable.h"
#include "vtkPolyDataMapper2D.h"
#include "vtkTextMapper.h"

#include <cstdint>

#define MV_DEFAULT_COLOR_SCHEME  0
#define MV_MODIFIED_COLOR_SCHEME 1
#define MV_CUSTOM_COLOR_SCHEME   2

class MV_EXPORT mvColorBar : public vtkActor2D
{
public:
    vtkTypeMacro(mvColorBar, vtkActor2D);
    static mvColorBar *New();

    virtual void       SetLookupTable(vtkLookupTable *lut);
    vtkLookupTable    *GetLookupTable() const { return m_LookupTable; }

    void               SetWidth(int w);
    int                GetWidth() const { return m_BarWidth; }
    void               SetHeight(int h);
    int                GetHeight() const { return m_BarHeight; }
    void               SetFontSize(int f);
    int                GetFontSize() const { return m_FontSize; }
    void               SetOffset(int f);
    int                GetOffset() const { return m_BarOffset; }
    void               SetLabelPrecision(int d);
    int                GetLabelPrecision() const { return m_Precision; }
    void               SetNumberOfLabels(int n);
    int                GetNumberOfLabels() const { return m_NumberOfLabels; }
    void               SetTextColor(double r, double g, double b);
    double            *GetTextColor() { return m_TextColor; }

    void               SetColorScheme(int value);
    int                GetColorScheme() const { return m_ColorScheme; }

    int                RenderOpaqueGeometry(vtkViewport *viewport) override;
    int                RenderTranslucentPolygonalGeometry(vtkViewport *) override { return 0; };
    int                RenderOverlay(vtkViewport *viewport) override;
    int                HasTranslucentPolygonalGeometry() override;
    void               ReleaseGraphicsResources(vtkWindow *) override;
    int                GetColorBarColorScheme() const;

    void               SetScalarRange(double min, double max);
    void               SetDataSourceIndex(int DataSourceIndex);

protected:
    mvColorBar();
    ~mvColorBar();
    mvColorBar(const mvColorBar &){};
    void                 operator=(const mvColorBar &){};

    int                  m_BarWidth;
    int                  m_BarHeight;
    int                  m_BarOffset;
    int                  m_FontSize;
    int                  m_NumberOfLabels;
    int                  m_Precision;
    int                  m_NumberOfColors;
    int                  m_NumberOfLabelsBuilt;
    int                  m_ColorScheme;
    int                  m_DataSourceIndex;
    char                *m_LabelFormat;
    double               m_minColor;
    double               m_maxColor;
    std::uint32_t        m_FirstCustomColor;
    std::uint32_t        m_LastCustomColor;
    double               m_TextColor[3];

    vtkLookupTable      *m_LookupTable;
    vtkPolyData         *m_ColorBar;
    vtkPolyDataMapper2D *m_ColorBarMapper;
    vtkActor2D          *m_ColorBarActor;
    vtkTextMapper      **m_TextMappers;
    vtkActor2D         **m_TextActors;
    vtkTimeStamp         m_BuildTime;
};

#endif

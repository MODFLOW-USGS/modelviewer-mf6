#include "mvColorBar.h"
#include "vtkPolyData.h"
#include "vtkProperty2D.h"
#include "vtkViewport.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkWindow.h"
#include "vtkTextMapper.h"
#include "vtkTextProperty.h"

#include <vtkObjectFactory.h>

vtkStandardNewMacro(mvColorBar);

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvColorBar::mvColorBar()
{
    m_BarWidth            = 20;
    m_BarHeight           = 200;
    m_BarOffset           = 100;
    m_FontSize            = 14;
    m_NumberOfLabels      = 5;
    m_Precision           = 3;
    m_NumberOfColors      = 64;
    m_NumberOfLabelsBuilt = 0;
    m_LabelFormat         = new char[30];
    sprintf(m_LabelFormat, "%s%d.%d%c", "%-#", m_Precision + 8, m_Precision, 'g');
    m_LookupTable    = 0;
    m_ColorBar       = vtkPolyData::New();
    m_ColorBarMapper = vtkPolyDataMapper2D::New();
    m_ColorBarMapper->SetInputData(m_ColorBar);
    m_ColorBarActor = vtkActor2D::New();
    m_ColorBarActor->SetMapper(m_ColorBarMapper);
    m_TextMappers  = 0;
    m_TextActors   = 0;
    m_ColorScheme  = MV_DEFAULT_COLOR_SCHEME;
    m_TextColor[0] = 0.;
    m_TextColor[1] = 0.;
    m_TextColor[2] = 0.;
    VisibilityOff();
}

void mvColorBar::ReleaseGraphicsResources(vtkWindow *win)
{
    if (m_TextMappers != 0)
    {
        for (int i = 0; i < m_NumberOfLabelsBuilt; i++)
        {
            m_TextActors[i]->ReleaseGraphicsResources(win);
        }
    }
    m_ColorBarActor->ReleaseGraphicsResources(win);
}

mvColorBar::~mvColorBar()
{
    if (m_LabelFormat)
    {
        delete[] m_LabelFormat;
        m_LabelFormat = 0;
    }
    if (m_TextMappers != 0)
    {
        for (int i = 0; i < m_NumberOfLabelsBuilt; i++)
        {
            m_TextMappers[i]->Delete();
            m_TextActors[i]->Delete();
        }
        delete[] m_TextMappers;
        delete[] m_TextActors;
    }

    m_ColorBar->Delete();
    m_ColorBarMapper->Delete();
    m_ColorBarActor->Delete();
    this->SetLookupTable(0);
}

void mvColorBar::SetColorScheme(int value)
{
    m_ColorScheme = value;
    this->Modified();
}

void mvColorBar::SetLookupTable(vtkLookupTable *lut)
{
    m_LookupTable = lut;
    this->Modified();
}

void mvColorBar::SetWidth(int w)
{
    m_BarWidth = w;
    this->Modified();
}

void mvColorBar::SetHeight(int h)
{
    m_BarHeight = h;
    this->Modified();
}

void mvColorBar::SetFontSize(int f)
{
    m_FontSize = f;
    this->Modified();
}

void mvColorBar::SetOffset(int r)
{
    m_BarOffset = r;
    this->Modified();
}

void mvColorBar::SetNumberOfLabels(int n)
{
    if (n < 0)
    {
        n = 0;
    }
    m_NumberOfLabels = n;
    this->Modified();
}

void mvColorBar::SetLabelPrecision(int d)
{
    m_Precision = d;
    sprintf(m_LabelFormat, "%s%d.%dg", "%-#", m_Precision + 8, m_Precision);
    this->Modified();
}

int mvColorBar::RenderOverlay(vtkViewport *viewport)
{
    if (!this->GetVisibility())
    {
        return 0;
    }
    int renderedSomething = 0;
    int i;

    m_ColorBarActor->RenderOverlay(viewport);
    if (m_TextActors)
    {
        for (i = 0; i < m_NumberOfLabelsBuilt; i++)
        {
            renderedSomething += m_TextActors[i]->RenderOverlay(viewport);
        }
        renderedSomething = (renderedSomething > 0) ? (1) : (0);
    }

    return renderedSomething;
}

int mvColorBar::RenderOpaqueGeometry(vtkViewport *viewport)
{
    int renderedSomething = 0;
    int i;

    if (!m_LookupTable)
    {
        return 0;
    }

    if (viewport->GetMTime() > m_BuildTime ||
        (viewport->GetVTKWindow() &&
         viewport->GetVTKWindow()->GetMTime() > m_BuildTime))
    {
        this->Modified();
    }

    if (this->GetMTime() > m_BuildTime ||
        m_LookupTable->GetMTime() > m_BuildTime)
    {
        if (m_TextMappers != 0)
        {
            for (i = 0; i < m_NumberOfLabelsBuilt; i++)
            {
                m_TextMappers[i]->Delete();
                m_TextActors[i]->Delete();
            }
            delete[] m_TextMappers;
            delete[] m_TextActors;
        }
        double *range = m_LookupTable->GetRange();
        if (m_DataSourceIndex == 1)
        {
            range[0] = m_minColor;
            range[1] = m_maxColor;
        }

        int        numPts = 2 * (m_NumberOfColors + 1);
        vtkPoints *pts    = vtkPoints::New();
        pts->SetNumberOfPoints(numPts);
        vtkCellArray *polys = vtkCellArray::New();
        polys->Allocate(polys->EstimateSize(m_NumberOfColors, 4));
        vtkUnsignedCharArray *colors = vtkUnsignedCharArray::New();
        colors->SetNumberOfComponents(3);
        colors->SetNumberOfTuples(m_NumberOfColors);

        m_ColorBarActor->SetProperty(this->GetProperty());
        m_ColorBar->Initialize();
        m_ColorBar->SetPoints(pts);
        m_ColorBar->SetPolys(polys);
        m_ColorBar->GetCellData()->SetScalars(colors);
        pts->Delete();
        polys->Delete();
        colors->Delete();

        if (m_NumberOfLabels > 0)
        {
            m_TextMappers = new vtkTextMapper *[m_NumberOfLabels];
            m_TextActors  = new vtkActor2D *[m_NumberOfLabels];
            char   string[512];
            double val;
            for (i = 0; i < m_NumberOfLabels; i++)
            {
                m_TextMappers[i] = vtkTextMapper::New();
                if ((strcmp(m_LookupTable->GetClassName(), "vtkLogLookupTable") == 0) || (strcmp(m_LookupTable->GetClassName(), "mvLogColorTable") == 0))
                {
                    double logRange0 = log(range[0]) / log(10.0);
                    double logRange1 = log(range[1]) / log(10.0);
                    double e         = logRange0 + (double)i / (m_NumberOfLabels - 1) * (logRange1 - logRange0);
                    val              = pow(10.0, e);
                }
                else
                {
                    val = range[0] + (double)i / (m_NumberOfLabels - 1) * (range[1] - range[0]);
                }
                sprintf(string, m_LabelFormat, val);
                m_TextMappers[i]->SetInput(string);
                m_TextMappers[i]->GetTextProperty()->SetFontSize(m_FontSize);
                m_TextMappers[i]->GetTextProperty()->SetColor(m_TextColor);
                m_TextActors[i] = vtkActor2D::New();
                m_TextActors[i]->SetMapper(m_TextMappers[i]);
            }
        }

        m_NumberOfLabelsBuilt = m_NumberOfLabels;

        // generate points
        double x[3];
        x[2]         = 0.0;
        double delta = ((double)m_BarHeight) / m_NumberOfColors;
        for (i = 0; i < numPts / 2; i++)
        {
            x[0] = 0;
            x[1] = i * delta;
            pts->SetPoint(2 * i, x);
            x[0] = m_BarWidth;
            pts->SetPoint(2 * i + 1, x);
        }

        // polygons & cell colors
        vtkIdType      ptIds[4];
        for (i = 0; i < m_NumberOfColors; i++)
        {
            ptIds[0] = 2 * (vtkIdType)i;
            ptIds[1] = ptIds[0] + 1;
            ptIds[2] = ptIds[1] + 2;
            ptIds[3] = ptIds[0] + 2;
            polys->InsertNextCell(4, ptIds);

            unsigned char *rgb = colors->GetPointer(3 * i); // write into array directly
            if ((strcmp(m_LookupTable->GetClassName(), "vtkLogLookupTable") == 0) || (strcmp(m_LookupTable->GetClassName(), "mvLogColorTable") == 0))
            {
                double logRange0 = log(range[0]) / log(10.0);
                double logRange1 = log(range[1]) / log(10.0);
                double e         = logRange0 + (logRange1 - logRange0) *
                                           ((double)i / (m_NumberOfColors - 1.0));
                const unsigned char *rgba = m_LookupTable->MapValue(pow(10.0, e));
                rgb[0]                    = rgba[0];
                rgb[1]                    = rgba[1];
                rgb[2]                    = rgba[2];
            }
            else
            {
                const unsigned char *rgba = m_LookupTable->MapValue(range[0] + (range[1] - range[0]) *
                                                              ((double)i / (m_NumberOfColors - 1.0)));
                rgb[0]                    = rgba[0];
                rgb[1]                    = rgba[1];
                rgb[2]                    = rgba[2];
            }
        }

        // Now position everything properly
        int *winSize   = viewport->GetVTKWindow()->GetSize();
        int  barBottom = (winSize[1] - m_BarHeight) / 2;
        int  barRight  = winSize[0] - m_BarOffset;
        int  barLeft   = barRight - m_BarWidth;
        m_ColorBarActor->SetPosition(barLeft, barBottom);
        if (m_NumberOfLabels > 0)
        {
            int textBottom = barBottom - m_TextMappers[0]->GetHeight(viewport) / 2;
            for (i = 0; i < m_NumberOfLabels; i++)
            {
                m_TextMappers[i]->GetTextProperty()->SetJustificationToLeft();
                m_TextActors[i]->SetPosition(barRight + m_BarWidth / 3,
                                             (double)i / (m_NumberOfLabels - 1) * m_BarHeight + textBottom);
            }
        }
        else
        {
            int textBottom = barBottom;
        }

        m_BuildTime.Modified();
    }

    m_ColorBarActor->RenderOpaqueGeometry(viewport);
    for (i = 0; i < m_NumberOfLabels; i++)
    {
        renderedSomething += m_TextActors[i]->RenderOpaqueGeometry(viewport);
    }

    renderedSomething = (renderedSomething > 0) ? (1) : (0);

    return renderedSomething;
}

int mvColorBar::HasTranslucentPolygonalGeometry()
{
    return 0;
}

void mvColorBar::SetScalarRange(double min, double max)
{
    if (min < max)
    {
        m_minColor = min;
        m_maxColor = max;
    }
    else
    {
        m_minColor = max;
        m_maxColor = min;
    }
}

void mvColorBar::SetDataSourceIndex(int DataSourceIndex)
{
    m_DataSourceIndex = DataSourceIndex;
}

void mvColorBar::SetTextColor(double r, double g, double b)
{
    m_TextColor[0] = r;
    m_TextColor[1] = g;
    m_TextColor[2] = b;
    this->Modified();
}

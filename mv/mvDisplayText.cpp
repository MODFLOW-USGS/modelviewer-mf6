#include "mvDisplayText.h"
#include "vtkTextProperty.h"
#include "vtkProperty2D.h"

// This must be below vtkStandardNewMacro
#if defined(_MSC_VER) && defined(_DEBUG) && defined(MV_DEBUG_MEMORY_LEAKS)
#include <afx.h>
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

mvDisplayText::mvDisplayText()
{
    m_TextMapper = vtkTextMapper::New();
    m_TextMapper->GetTextProperty()->SetFontFamilyToArial();
    m_TextMapper->GetTextProperty()->SetFontSize(14);
    m_TextMapper->GetTextProperty()->SetColor(0, 0, 0);
    m_TextActor = vtkActor2D::New();
    m_TextActor->SetMapper(m_TextMapper);
    m_TextActor->SetPosition(10, 10);
    m_TextActor->VisibilityOff();
}

mvDisplayText::~mvDisplayText()
{
    m_TextMapper->Delete();
    m_TextActor->Delete();
}

void mvDisplayText::VisibilityOn()
{
    m_TextActor->VisibilityOn();
}

void mvDisplayText::VisibilityOff()
{
    m_TextActor->VisibilityOff();
}

void mvDisplayText::SetVisibility(int v)
{
    m_TextActor->SetVisibility(v);
}

int mvDisplayText::GetVisibility()
{
    return m_TextActor->GetVisibility();
}

void mvDisplayText::SetColor(double red, double green, double blue)
{
    m_TextMapper->GetTextProperty()->SetColor(red, green, blue);
}

const double *mvDisplayText::GetColor()
{
    return m_TextMapper->GetTextProperty()->GetColor();
}

void mvDisplayText::SetText(char *text)
{
    m_TextMapper->SetInput(text);
}

void mvDisplayText::SetPosition(double x, double y)
{
    m_TextActor->SetPosition(x, y);
}

const double *mvDisplayText::GetPosition()
{
    return m_TextActor->GetPosition();
}

void mvDisplayText::SetFontSize(int size)
{
    m_TextMapper->GetTextProperty()->SetFontSize(size);
}

int mvDisplayText::GetFontSize()
{
    return m_TextMapper->GetTextProperty()->GetFontSize();
}

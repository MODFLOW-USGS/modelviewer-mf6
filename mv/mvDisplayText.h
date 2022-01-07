#ifndef __mvDisplayText_h
#define __mvDisplayText_h

#include "mvHeader.h"
#include "vtkActor2D.h"
#include "vtkTextMapper.h"

class MV_EXPORT mvDisplayText  
{
public:
	mvDisplayText();
	virtual ~mvDisplayText();

	vtkActor2D *GetActor2D() {return m_TextActor;}
	void VisibilityOn();
	void VisibilityOff();
	void SetVisibility(int v);
	int GetVisibility();
	void SetColor(double red, double green, double blue);
	const double *GetColor();
	void SetText(char *text);
	void SetPosition(double x, double y);
	const double *GetPosition();
	void SetFontSize(int size);
	int GetFontSize();

protected:

	vtkTextMapper *m_TextMapper;
	vtkActor2D *m_TextActor;
};

#endif

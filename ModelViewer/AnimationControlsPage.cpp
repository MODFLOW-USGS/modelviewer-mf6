// AnimationControlsPage.cpp : implementation file
//

#include "ModelViewer.h"
#include "AnimationControlsPage.h"
#include "AnimationOptionsPage.h"
#include "MvDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimationControlsPage property page

IMPLEMENT_DYNCREATE(CAnimationControlsPage, CPropertyPage)

CAnimationControlsPage::CAnimationControlsPage() : CPropertyPage(CAnimationControlsPage::IDD)
{
	//{{AFX_DATA_INIT(CAnimationControlsPage)
	m_SetToIndex = -1;
	m_NumberOfSteps = 0;
	//}}AFX_DATA_INIT
	m_NumberOfTimePoints = 0;
	m_TimePointLabels = 0;
	m_CurrentTimePointIndex = -1;
	m_NumberOfSteps = 1;
}

CAnimationControlsPage::~CAnimationControlsPage()
{

}

void CAnimationControlsPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimationControlsPage)
	DDX_CBIndex(pDX, IDC_TIME_POINT_LABELS, m_SetToIndex);
	DDX_Text(pDX, IDC_EDIT_NUMBER_OF_STEPS, m_NumberOfSteps);
	DDV_MinMaxInt(pDX, m_NumberOfSteps, 1, 10000000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimationControlsPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAnimationControlsPage)
	ON_BN_CLICKED(IDC_RUN_ANIMATION, OnRunAnimation)
	ON_BN_CLICKED(IDC_STOP_ANIMATION, OnStopAnimation)
	ON_BN_CLICKED(IDC_ADVANCE_ANIMATION, OnAdvanceAnimation)
	ON_BN_CLICKED(IDC_SET_TO_TIME_POINT, OnSetToTimePoint)
	ON_CBN_SELCHANGE(IDC_COMBO_ANIMATION_TYPE, OnSelchangeComboAnimationType)
	ON_EN_CHANGE(IDC_EDIT_NUMBER_OF_STEPS, OnChangeEditNumberOfSteps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimationControlsPage message handlers

BOOL CAnimationControlsPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	Reset();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnimationControlsPage::Reinitialize()
{
	m_NumberOfTimePoints = 0;
	m_TimePointLabels = 0;
	GetDlgItem(IDC_CURRENT_TIME)->SetWindowText("");
	Reset();
}

void CAnimationControlsPage::Reset()
{
	CComboBox *cb = (CComboBox *) GetDlgItem(IDC_TIME_POINT_LABELS);
	cb->ResetContent();
	for (int i=0; i<m_NumberOfTimePoints; i++)
	{
		cb->AddString(m_TimePointLabels[i]);
	}
	if (m_NumberOfTimePoints > 0) 
	{
		cb->SetCurSel(0);
		m_SetToIndex = 0;
	}

	if (m_NumberOfTimePoints <= 1)
	{
		GetDlgItem(IDC_RUN_ANIMATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADVANCE_ANIMATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_STOP_ANIMATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_SET_TO_TIME_POINT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_ANIMATION_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NUMBER_OF_STEPS)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_STOP_ANIMATION)->EnableWindow(FALSE);
		GetDlgItem(IDC_SET_TO_TIME_POINT)->EnableWindow(TRUE);
		BOOL b = (m_CurrentTimePointIndex < m_NumberOfTimePoints-1);
		GetDlgItem(IDC_RUN_ANIMATION)->EnableWindow(b);
		GetDlgItem(IDC_ADVANCE_ANIMATION)->EnableWindow(b);
		GetDlgItem(IDC_COMBO_ANIMATION_TYPE)->EnableWindow(TRUE);
	}
	int AnimationType = ((CComboBox *) GetDlgItem(IDC_COMBO_ANIMATION_TYPE))
						->GetCurSel();
	switch (AnimationType)
	{
		case 0:
			GetDlgItem(IDC_EDIT_NUMBER_OF_STEPS)->EnableWindow(FALSE);
			break;
		case 1:
			GetDlgItem(IDC_EDIT_NUMBER_OF_STEPS)->EnableWindow(TRUE);
			break;
		default:
			GetDlgItem(IDC_EDIT_NUMBER_OF_STEPS)->EnableWindow(FALSE);
			break;
	}
}

void CAnimationControlsPage::OnRunAnimation() 
{
	GetDlgItem(IDC_RUN_ANIMATION)->EnableWindow(FALSE);
	GetDlgItem(IDC_ADVANCE_ANIMATION)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_ANIMATION)->EnableWindow(TRUE);
	GetDlgItem(IDC_SET_TO_TIME_POINT)->EnableWindow(FALSE);
	GetDlgItem(IDC_TIME_POINT_LABELS)->EnableWindow(FALSE);
	m_OptionsPage->Activate(FALSE);
	m_pDoc->StartAnimation(m_OptionsPage->m_Delay);
}

void CAnimationControlsPage::OnAdvanceAnimation() 
{
	AnimationType at = m_pDoc->GetAnimationType();
	switch (at)
	{
		case atTime:
			m_pDoc->AdvanceOneTimePoint();
			break;
		case atSpace:
			m_pDoc->UpdateAnimationWithSameTime();
			break;
	}
}

void CAnimationControlsPage::OnStopAnimation() 
{
	GetDlgItem(IDC_STOP_ANIMATION)->EnableWindow(FALSE);
	m_pDoc->StopAnimation();		
}

void CAnimationControlsPage::OnSetToTimePoint() 
{
	int timePointIndex = ((CComboBox *) GetDlgItem(IDC_TIME_POINT_LABELS))
						->GetCurSel();
	m_pDoc->SetTimePointTo(timePointIndex);	
}

void CAnimationControlsPage::SetAndDisplayCurrentTime(int timePointIndex)
{
	m_CurrentTimePointIndex = timePointIndex;
	GetDlgItem(IDC_CURRENT_TIME)->SetWindowText(m_TimePointLabels[timePointIndex]);
}

void CAnimationControlsPage::OnSelchangeComboAnimationType() 
{
	int AnimationType = ((CComboBox *) GetDlgItem(IDC_COMBO_ANIMATION_TYPE))
						->GetCurSel();
	BOOL CanRun = (m_CurrentTimePointIndex < m_NumberOfTimePoints-1);
	switch (AnimationType)
	{
		case 0:
			m_pDoc->SetAnimationType(atTime);
			GetDlgItem(IDC_EDIT_NUMBER_OF_STEPS)->EnableWindow(FALSE);
			break;
		case 1:
			m_pDoc->SetAnimationType(atSpace);
			CanRun = TRUE;
			GetDlgItem(IDC_EDIT_NUMBER_OF_STEPS)->EnableWindow(TRUE);
			break;
		default:
			m_pDoc->SetAnimationType(atTime);
			GetDlgItem(IDC_EDIT_NUMBER_OF_STEPS)->EnableWindow(FALSE);
			break;
	}
	GetDlgItem(IDC_RUN_ANIMATION)->EnableWindow(CanRun);
	GetDlgItem(IDC_ADVANCE_ANIMATION)->EnableWindow(CanRun);
	
}

void CAnimationControlsPage::SetAnimationType(int Value) 
{
	((CComboBox *) GetDlgItem(IDC_COMBO_ANIMATION_TYPE))
						->SetCurSel(Value);
	
}

void CAnimationControlsPage::OnChangeEditNumberOfSteps() 
{
	UpdateData(TRUE);
	m_pDoc->SetAnimationSteps(m_NumberOfSteps);
}

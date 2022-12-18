#include "bitmapresolutiondialog.h"
#include "ui_bitmapresolutiondialog.h"

BitmapResolutionDialog::BitmapResolutionDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::BitmapResolutionDialog)
{
    ui->setupUi(this);

    ui->radioButtonWidth->setChecked(true);
    ui->radioButtonScreen->setChecked(true);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->radioButtonScreen, &QAbstractButton::clicked, this, &BitmapResolutionDialog::onResolutionScreen);
    connect(ui->radioButton150ppi, &QAbstractButton::clicked, this, &BitmapResolutionDialog::onResolution150);
    connect(ui->radioButton300ppi, &QAbstractButton::clicked, this, &BitmapResolutionDialog::onResolution300);

    connect(ui->radioButtonWidth, &QAbstractButton::clicked, this, &BitmapResolutionDialog::onWidthOption);
    connect(ui->radioButtonHeight, &QAbstractButton::clicked, this, &BitmapResolutionDialog::onHeightOption);


    //enableSize(resolutionOption != ResolutionType::rtScreen);
    initDialog();
}

BitmapResolutionDialog::~BitmapResolutionDialog()
{
    delete ui;
}

void BitmapResolutionDialog::onResolutionScreen()
{
    //this->enableSize(false);
    /*
    GetDlgItem(IDC_BMP_WIDTH_OPTION)->EnableWindow(FALSE);
    GetDlgItem(IDC_BMP_HEIGHT_OPTION)->EnableWindow(FALSE);
    GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(FALSE);
    GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(FALSE);
    GetDlgItem(IDC_BMP_WIDTH)->SetWindowText(_T(""));
    GetDlgItem(IDC_BMP_HEIGHT)->SetWindowText(_T(""));
    */

    ui->radioButtonWidth->setEnabled(false);
    ui->radioButtonHeight->setEnabled(false);
    ui->lineEditWidth->setEnabled(false);
    ui->lineEditHeight->setEnabled(false);
    ui->labelInchesWidth->setEnabled("");
    ui->labelInchesHeight->setEnabled("");
}

void BitmapResolutionDialog::onResolution150()
{
    //this->enableSize(true);
    /*
    GetDlgItem(IDC_BMP_WIDTH_OPTION)->EnableWindow(TRUE);
    GetDlgItem(IDC_BMP_HEIGHT_OPTION)->EnableWindow(TRUE);
    if (((CButton*)GetDlgItem(IDC_BMP_WIDTH_OPTION))->GetCheck())
    {
        GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(TRUE);
    }
    */

    ui->radioButtonWidth->setEnabled(true);
    ui->radioButtonHeight->setEnabled(true);
    if (ui->radioButtonWidth->isChecked())
    {
        ui->lineEditWidth->setEnabled(true);
    }
    else
    {
        ui->lineEditHeight->setEnabled(true);
    }
}

void BitmapResolutionDialog::onResolution300()
{
    //this->enableSize(true);
    /*
    GetDlgItem(IDC_BMP_WIDTH_OPTION)->EnableWindow(TRUE);
    GetDlgItem(IDC_BMP_HEIGHT_OPTION)->EnableWindow(TRUE);
    if (((CButton *)GetDlgItem(IDC_BMP_WIDTH_OPTION))->GetCheck())
    {
        GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(TRUE);
    }
    else
    {
        GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(TRUE);
    }
    */

    ui->radioButtonWidth->setEnabled(true);
    ui->radioButtonHeight->setEnabled(true);
    if (ui->radioButtonWidth->isChecked())
    {
        ui->lineEditWidth->setEnabled(true);
    }
    else
    {
        ui->lineEditHeight->setEnabled(true);
    }
}

void BitmapResolutionDialog::enableSize(bool enable)
{
    assert(false);
    ui->groupBoxSize->setEnabled(enable);
    ui->radioButtonWidth->setEnabled(enable);
    ui->radioButtonHeight->setEnabled(enable);
    ui->lineEditWidth->setEnabled(enable);
    ui->lineEditHeight->setEnabled(enable);
    ui->labelInchesWidth->setEnabled(enable);
    ui->labelInchesHeight->setEnabled(enable);
}

void BitmapResolutionDialog::onWidthOption()
{
    ui->lineEditHeight->setText("");
    ui->lineEditWidth->setEnabled(true);
    ui->lineEditHeight->setEnabled(false);
}

void BitmapResolutionDialog::onHeightOption()
{
    ui->lineEditWidth->setText("");
    ui->lineEditWidth->setEnabled(false);
    ui->lineEditHeight->setEnabled(true);
}

void BitmapResolutionDialog::initDialog()
{
    /*
    if (m_ResolutionOption == 0)
    {
        GetDlgItem(IDC_BMP_WIDTH_OPTION)->EnableWindow(FALSE);
        GetDlgItem(IDC_BMP_HEIGHT_OPTION)->EnableWindow(FALSE);
        GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(FALSE);
        GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(FALSE);
        GetDlgItem(IDC_BMP_HEIGHT)->SetWindowText(_T(""));
        GetDlgItem(IDC_BMP_WIDTH)->SetWindowText(_T(""));
    }
    else
    {
        GetDlgItem(IDC_BMP_WIDTH_OPTION)->EnableWindow(TRUE);
        GetDlgItem(IDC_BMP_HEIGHT_OPTION)->EnableWindow(TRUE);
        if (m_SideOption == 0)
        {
            GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(TRUE);
            if (m_WidthInInches == 0)
            {
                GetDlgItem(IDC_BMP_WIDTH)->SetWindowText(_T(""));
            }
            GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(FALSE);
            GetDlgItem(IDC_BMP_HEIGHT)->SetWindowText(_T(""));
        }
        else
        {
            GetDlgItem(IDC_BMP_HEIGHT)->EnableWindow(TRUE);
            if (m_HeightInInches == 0)
            {
                GetDlgItem(IDC_BMP_HEIGHT)->SetWindowText(_T(""));
            }
            GetDlgItem(IDC_BMP_WIDTH)->EnableWindow(FALSE);
            GetDlgItem(IDC_BMP_WIDTH)->SetWindowText(_T(""));
        }
    }
    */
    if (resolutionOption == ResolutionType::rtScreen)
    {
        ui->radioButtonWidth->setEnabled(false);
        ui->radioButtonHeight->setEnabled(false);
        ui->lineEditWidth->setEnabled(false);
        ui->lineEditHeight->setEnabled(false);
        ui->lineEditWidth->setText("");
        ui->lineEditHeight->setText("");
    }
    else
    {
        ui->radioButtonWidth->setEnabled(true);
        ui->radioButtonHeight->setEnabled(true);
        if (this->sideOption == SideType::stWidth)
        {
            ui->lineEditWidth->setEnabled(true);
            if (this->widthInInches == 0)
            {
                ui->lineEditWidth->setText("");
            }
            ui->lineEditHeight->setEnabled(false);
            ui->lineEditHeight->setText("");
        }
        else
        {
            ui->lineEditHeight->setEnabled(true);
            if (this->heightInInches == 0)
            {
                ui->lineEditHeight->setText("");
            }
            ui->lineEditWidth->setEnabled(false);
            ui->lineEditWidth->setEnabled("");
        }
    }
}
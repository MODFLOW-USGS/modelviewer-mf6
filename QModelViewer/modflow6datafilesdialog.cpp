#include "modflow6datafilesdialog.h"
#include "ui_modflow6datafilesdialog.h"

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>

Modflow6DataFilesDialog::Modflow6DataFilesDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::Modflow6DataFilesDialog)
{
    ui->setupUi(this);

    specifyNameFile         = true;
    gwfModelNameFile        = "";
    headOrConcFile          = "";
    gridFile                = "";
    budgetFile              = "";

    ui->radioButtonSpecifyModflow6NameFile->setChecked(true);
    onSpecifyModflow6NameFile();

    connect(ui->radioButtonSpecifyModflow6DataFiles, &QAbstractButton::clicked, this, &Modflow6DataFilesDialog::onSpecifyModflow6DataFiles);
    connect(ui->radioButtonSpecifyModflow6NameFile, &QAbstractButton::clicked, this, &Modflow6DataFilesDialog::onSpecifyModflow6NameFile);

    connect(ui->pushButtonBrowseModelNameFile, &QAbstractButton::clicked, this, &Modflow6DataFilesDialog::onBrowseModelNameFile);
    connect(ui->pushButtonBrowseModelGridFile, &QAbstractButton::clicked, this, &Modflow6DataFilesDialog::onBrowseModelGridFile);
    connect(ui->pushButtonBrowseModelHeadFile, &QAbstractButton::clicked, this, &Modflow6DataFilesDialog::onBrowseModelHeadFile);
    connect(ui->pushButtonBrowseModelBudgetFile, &QAbstractButton::clicked, this, &Modflow6DataFilesDialog::onBrowseModelBudgetFile);
}

Modflow6DataFilesDialog::~Modflow6DataFilesDialog()
{
    delete ui;
}

// https://www.qtcentre.org/threads/8048-Validate-Data-in-QDialog
//
void Modflow6DataFilesDialog::done(int r)
{
    if (r == QDialog::Accepted)
    {
        if (!validate()) return;
    }

    // if here everythings ok
    QDialog::done(r);
}

bool Modflow6DataFilesDialog::validate()
{
    //CDialog::DoDataExchange(pDX);
    //DDX_Text(pDX, IDC_GWF_MODEL_NAME_FILE, m_GwfModelNameFile);
    //DDX_Text(pDX, IDC_GWF_MODEL_HEAD_FILE, m_HeadOrConcFile);
    //DDX_Text(pDX, IDC_GWF_MODEL_GRID_FILE, m_GridFile);
    //DDX_Text(pDX, IDC_GWF_MODEL_BUDGET_FILE, m_BudgetFile);
    //DDX_Radio(pDX, IDC_SPECIFY_MODFLOW6_NAME_FILE, m_FileSpecificationMethod);

    //if (m_FileSpecificationMethod == 0)
    //{
    //    m_GwfModelNameFile.TrimLeft();
    //    m_GwfModelNameFile.TrimRight();

    //    pDX->PrepareEditCtrl(IDC_NAME_FILE);
    //    if (pDX->m_bSaveAndValidate)
    //    {
    //        if (m_GwfModelNameFile.GetLength() == 0)
    //        {
    //            AfxMessageBox("Please specify a Groundwater Flow Model Name File.", MB_ICONEXCLAMATION);
    //            pDX->Fail();
    //            return;
    //        }
    //    }
    //    CString nameFile = m_GwfModelNameFile.Right(9);
    //    if (!nameFile.CompareNoCase("mfsim.nam"))
    //    {
    //        AfxMessageBox("\"" + nameFile + "\" is a Simulation Name File. Please specify a Groundwater Flow Model Name File.", MB_ICONEXCLAMATION);
    //        pDX->Fail();
    //        return;
    //    }
    //}
    //else
    //{
    //}

    if (ui->radioButtonSpecifyModflow6NameFile->isChecked())
    {
        // specifyNameFile
        specifyNameFile  = true;

        // gwfModelNameFile
        gwfModelNameFile = QDir::toNativeSeparators(ui->lineEditModelNameFile->text().trimmed());
        if (gwfModelNameFile.isEmpty())
        {
            QMessageBox::warning(this,
                                 tr("GWF Model Name File"),
                                 tr("Please specify a Groundwater Flow Model Name File."));
            ui->lineEditModelNameFile->setFocus();
            return false;
        }
        QString nameFile = gwfModelNameFile.right(9);
        
        if (!nameFile.compare("mfsim.nam", Qt::CaseInsensitive))
        {
            QString text = QString(tr("\"%1\" is a Simulation Name File. Please specify a Groundwater Flow Model Name File.")).arg(nameFile);
            QMessageBox::warning(this, "", text);
            ui->lineEditModelNameFile->setFocus();
            return false;
        }
    }
    else
    {
        // specifyNameFile
        specifyNameFile = false;

        // gridFile
        gridFile        = QDir::toNativeSeparators(ui->lineEditModelGridFile->text().trimmed());
        if (gridFile.isEmpty())
        {
            QMessageBox::warning(this,
                                 tr("GWF Model Grid File"),
                                 tr("Please specify a Groundwater Flow Model Grid File."));
            ui->lineEditModelNameFile->setFocus();
            return false;
        }
        else
        {
            QFileInfo checkGrid(gridFile);
            if (!(checkGrid.exists() && checkGrid.isFile()))
            {
                QMessageBox::warning(this,
                                     tr("GWF Model Grid File"),
                                     QString(tr("%1\nFile not found.\nCheck the file name and try again.")).arg(checkGrid.fileName()));
                ui->lineEditModelNameFile->setFocus();
                return false;
            }
        }

        // headOrConcFile
        headOrConcFile  = QDir::toNativeSeparators(ui->lineEditModelHeadOrConcFile->text().trimmed());
        if (!headOrConcFile.isEmpty())
        {
            QFileInfo checkheadOrConc(gridFile);
            if (!(checkheadOrConc.exists() && checkheadOrConc.isFile()))
            {
                QMessageBox::warning(this,
                                     tr("GWF Model Head or GWT Model Concentration File"),
                                     QString(tr("%1\nFile not found.\nCheck the file name and try again.")).arg(checkheadOrConc.fileName()));
                ui->lineEditModelNameFile->setFocus();
                return false;
            }
        }

        // budgetFile
        budgetFile      = QDir::toNativeSeparators(ui->lineEditModelBudgetFile->text().trimmed());
        if (!headOrConcFile.isEmpty())
        {
            QFileInfo checkBudget(gridFile);
            if (!(checkBudget.exists() && checkBudget.isFile()))
            {
                QMessageBox::warning(this,
                                     tr("GWF Model Budget File"),
                                     QString(tr("%1\nFile not found.\nCheck the file name and try again.")).arg(checkBudget.fileName()));
                ui->lineEditModelNameFile->setFocus();
                return false;
            }
        }
    }

    return true;
}

void Modflow6DataFilesDialog::onSpecifyModflow6DataFiles()
{
    ui->lineEditModelNameFile->setEnabled(false);
    ui->lineEditModelGridFile->setEnabled(true);
    ui->lineEditModelHeadOrConcFile->setEnabled(true);
    ui->lineEditModelBudgetFile->setEnabled(true);

    ui->pushButtonBrowseModelNameFile->setEnabled(false);
    ui->pushButtonBrowseModelGridFile->setEnabled(true);
    ui->pushButtonBrowseModelHeadFile->setEnabled(true);
    ui->pushButtonBrowseModelBudgetFile->setEnabled(true);
}

void Modflow6DataFilesDialog::onSpecifyModflow6NameFile()
{
    ui->lineEditModelNameFile->setEnabled(true);
    ui->lineEditModelGridFile->setEnabled(false);
    ui->lineEditModelHeadOrConcFile->setEnabled(false);
    ui->lineEditModelBudgetFile->setEnabled(false);

    ui->pushButtonBrowseModelNameFile->setEnabled(true);
    ui->pushButtonBrowseModelGridFile->setEnabled(false);
    ui->pushButtonBrowseModelHeadFile->setEnabled(false);
    ui->pushButtonBrowseModelBudgetFile->setEnabled(false);
}

void Modflow6DataFilesDialog::onBrowseModelNameFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("GWF Model Name File"),
                                                    QString(),
                                                    tr("Name Files (*.nam *.name *.fil);;All files (*.*)"));
    if (fileName.isEmpty())
    {
        return;
    }
    QString nameFile = fileName.right(9);
    if (!nameFile.compare("mfsim.nam", Qt::CaseInsensitive))
    {
        QString text = QString(tr("\"%1\" is a Simulation Name File. Please specify a Groundwater Flow Model Name File.")).arg(nameFile);
        QMessageBox::warning(this, "", text);
        return;
    }
    this->ui->lineEditModelNameFile->setText(QDir::toNativeSeparators(fileName));
}

void Modflow6DataFilesDialog::onBrowseModelGridFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("GWF Model Grid File"),
                                                    QString(),
                                                    tr("Grid Files (*.grb);;All files (*.*)"));
    if (fileName.isEmpty())
    {
        return;
    }
    this->ui->lineEditModelGridFile->setText(QDir::toNativeSeparators(fileName));
}

void Modflow6DataFilesDialog::onBrowseModelHeadFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("GWF Model Head or GWT Model Concentration File"),
                                                    QString(),
                                                    tr("Head & Concentration Files (*.hds; *.bhd; *.hdb; *.hed; *.ucn);;Concentration Files (*.ucn);;Head Files (*.hds; *.bhd; *.hdb; *.hed);;All files (*.*)"));
    if (fileName.isEmpty())
    {
        return;
    }
    this->ui->lineEditModelHeadOrConcFile->setText(QDir::toNativeSeparators(fileName));
}

void Modflow6DataFilesDialog::onBrowseModelBudgetFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("GWF Model Budget File"),
                                                    QString(),
                                                    tr("Budget Files (*.cbc; *.cbb; *.ccf; *.bud; *.bin);;All files (*.*)"));
    if (fileName.isEmpty())
    {
        return;
    }
    this->ui->lineEditModelBudgetFile->setText(QDir::toNativeSeparators(fileName));
}

#include "exportanimationdialog.h"
#include "ui_exportanimationdialog.h"

#include <QMessageBox>
#include <QFileDialog>

#include "mvdoc.h"  // enum class AnimationType

ExportAnimationDialog::ExportAnimationDialog(QWidget *parent)
    : QDialog(parent, Qt::WindowCloseButtonHint),
    ui(new Ui::ExportAnimationDialog)
{
    ui->setupUi(this);

    elevate                     = 0.0;
    //endIndex        = -1;
    endIndex                    = 0;
    filePrefix                  = "";
    fileStartNumber             = "";
    outputFolder                = "";
    rotate                      = 0.0;
    //startIndex      = -1;
    startIndex                  = 0;
    numberOfSteps               = 0;
    animationType               = AnimationType::atTime;
    preview                     = false;

    //// validators
    //QIntValidator *intValidator = new QIntValidator(this);
    //intValidator->setRange(1, 1000000);
    //ui->lineEditNumberOfSteps->setValidator(intValidator);

    QDoubleValidator *doubleValidator = new QDoubleValidator(this);
    ui->lineEditRotate->setValidator(doubleValidator);

    // connections
    connect(ui->pushButtonExport, &QAbstractButton::clicked, this, &QDialog::accept);
    connect(ui->pushButtonCancel, &QAbstractButton::clicked, this, &QDialog::reject);
}

ExportAnimationDialog::~ExportAnimationDialog()
{
    delete ui;
}

void ExportAnimationDialog::onInitDialog()
{
    ui->comboBoxAnimationType->setCurrentIndex((int)animationType);

    doAnimationTypeChange();

    assert(ui->comboBoxStartIndex->count() == 0);
    assert(ui->comboBoxEndIndex->count() == 0);
    ui->comboBoxStartIndex->insertItems(0, timePointLabels);
    ui->comboBoxEndIndex->insertItems(0, timePointLabels);

    ui->comboBoxStartIndex->setCurrentIndex(startIndex);
    ui->comboBoxEndIndex->setCurrentIndex(endIndex);

    int numFiles = endIndex - startIndex + 1;
    ui->labelNumberOfFiles->setText(QString(tr("Number of files = %1")).arg(numFiles));

    ui->labelDiskSpaceRequired->setText(QString(tr("Space required ≈ %1 MB")).arg(numFiles * MBPerFile, 0, 'g', 3));

    // numberOfSteps
    //ui->lineEditNumberOfSteps->setText(QString("%1").arg(numberOfSteps));
    ui->spinBoxNumberOfSteps->setValue(numberOfSteps);

    // rotate
    ui->lineEditRotate->setText(QString("%1").arg(rotate));

    // elevate
    ui->lineEditElevate->setText(QString("%1").arg(elevate));

    // filePrefix
    ui->lineEditFilePrefix->setText(filePrefix);

    // fileStartNumber
    ui->lineEditFileStartNumber->setText(fileStartNumber);

    // outputFolder
    ui->lineEditOutputFolder->setText(outputFolder);

    // connections
    //
    // to block signals use QSignalBlocker since Qt 5.3
    //
    connect(ui->pushButtonPreview, &QAbstractButton::clicked, this, &ExportAnimationDialog::onPreview);
    connect(ui->pushButtonBrowse, &QAbstractButton::clicked, this, &ExportAnimationDialog::onBrowse);
    connect(ui->comboBoxAnimationType, QOverload<int>::of(&QComboBox::currentIndexChanged), this, QOverload<int>::of(&ExportAnimationDialog::onChangeComboBoxAnimationType));
    connect(ui->comboBoxStartIndex, QOverload<int>::of(&QComboBox::currentIndexChanged), this, QOverload<int>::of(&ExportAnimationDialog::onChangeStartIndex));
    connect(ui->comboBoxEndIndex, QOverload<int>::of(&QComboBox::currentIndexChanged), this, QOverload<int>::of(&ExportAnimationDialog::onChangeEndIndex));

    // QSpinBox
    connect(ui->spinBoxNumberOfSteps, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &ExportAnimationDialog::onChangeNumberOfSteps);
}


void ExportAnimationDialog::showEvent(QShowEvent *event)
{
    onInitDialog();
    QDialog::showEvent(event);
}

void ExportAnimationDialog::onBrowse()
{
    QString dir = QFileDialog::getExistingDirectory(this,
                                                    tr("Browse for Folder"),
                                                    defaultOutputFolder,
                                                    QFileDialog::ShowDirsOnly /* | QFileDialog::DontResolveSymlinks */);
    if (!dir.isEmpty())
    {
        ui->lineEditOutputFolder->setText(QDir::toNativeSeparators(dir));
    }
}

void ExportAnimationDialog::onChangeComboBoxAnimationType(int index)
{
    animationType = static_cast<AnimationType>(index);    
    doAnimationTypeChange();
}

void ExportAnimationDialog::doAnimationTypeChange()
{
    switch (animationType)
    {
    case AnimationType::atTime:
        //ui->lineEditNumberOfSteps->setEnabled(false);
        ui->spinBoxNumberOfSteps->setEnabled(false);
        ui->comboBoxStartIndex->setEnabled(true);
        ui->comboBoxEndIndex->setEnabled(true);
        break;
    case AnimationType::atSpace:
        //ui->lineEditNumberOfSteps->setEnabled(true);
        ui->spinBoxNumberOfSteps->setEnabled(true);
        ui->comboBoxStartIndex->setEnabled(false);
        ui->comboBoxEndIndex->setEnabled(false);
        //onChangeNumberOfSteps(ui->spinBoxNumberOfSteps->value());
        break;
    default:
        assert(false);
        break;
    }
    updateTimeSpan();
}

void ExportAnimationDialog::onChangeNumberOfSteps(int value)
{
    numberOfSteps = value;
    //updateFiles(value);
    updateTimeSpan();
}

void ExportAnimationDialog::onChangeStartIndex(int index)
{
    if (index > endIndex)
    {
        QMessageBox::warning(this, "", tr("Please ensure that the starting time step is less than or equal to the ending time step."));
        assert(startIndex <= endIndex);
        ui->comboBoxStartIndex->setCurrentIndex(startIndex);
        return;
    }
    startIndex   = index;
    updateTimeSpan();
}

void ExportAnimationDialog::onChangeEndIndex(int index)
{
    if (startIndex > index)
    {
        QMessageBox::warning(this, "", tr("Please ensure that the starting time step is less than or equal to the ending time step."));
        assert(startIndex <= endIndex);
        ui->comboBoxEndIndex->setCurrentIndex(endIndex);
        return;
    }
    endIndex     = index;
    updateTimeSpan();
}

void ExportAnimationDialog::updateTimeSpan()
{
    switch (animationType)
    {
    case AnimationType::atTime:
        updateFiles(endIndex - startIndex + 1);
        break;
    case AnimationType::atSpace:
        updateFiles(numberOfSteps);
        break;
    default:
        assert(false);
        break;
    }
}

void ExportAnimationDialog::updateFiles(int numFiles)
{
    assert(numFiles > 0);
    ui->labelNumberOfFiles->setText(QString(tr("Number of files = %1")).arg(numFiles));
    ui->labelDiskSpaceRequired->setText(QString(tr("Space required ≈ %1 MB")).arg(numFiles * MBPerFile, 0, 'g', 3));
}

void ExportAnimationDialog::onPreview()
{
    if (validate())
    {
        preview = true;
        this->accept();
    }
}

bool ExportAnimationDialog::validate()
{
    bool    ok;

    // validate animationType
    // no-op

    // numberOfSteps  -- DDX_Text(pDX, IDC_EDIT_EXPORT_NUMBER_OF_STEPS, m_NumberOfSteps);
    //QString steps = ui->lineEditNumberOfSteps->text();
    //if (steps.isEmpty())
    //{
    //    QMessageBox::warning(this, "", tr("Enter an integer."));
    //    ui->lineEditNumberOfSteps->setFocus();
    //    return false;
    //}
    numberOfSteps = ui->spinBoxNumberOfSteps->value();
    assert(0 < numberOfSteps && numberOfSteps < 1000000);

    // validate rotate
    QString rot = ui->lineEditRotate->text();
    if (rot.isEmpty())
    {
        QMessageBox::warning(this, "", tr("Enter a number."));
        ui->lineEditRotate->setFocus();
        return false;
    }
    double dbl      = rot.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::warning(this, "", tr("Enter a number."));
        ui->lineEditRotate->setFocus();
        return false;
    }
    rotate          = dbl;

    // validate elevate
    QString elev = ui->lineEditElevate->text();
    if (elev.isEmpty())
    {
        QMessageBox::warning(this, "", tr("Enter a number."));
        ui->lineEditElevate->setFocus();
        return false;
    }
    dbl = elev.toDouble(&ok);
    if (!ok)
    {
        QMessageBox::warning(this, "", tr("Enter a number."));
        ui->lineEditElevate->setFocus();
        return false;
    }
    elevate        = dbl;

    // validate filePrefix
    filePrefix     = ui->lineEditFilePrefix->text();

    //// validate fileStartNumber
    //QString start = ui->lineEditFileStartNumber->text();
    //if (start.isEmpty())
    //{
    //    QMessageBox::warning(this, "", tr("Enter a number."));
    //    ui->lineEditFileStartNumber->setFocus();
    //    return false;
    //}
    //int startNumber = start.toInt(&ok);
    //if (!ok)
    //{
    //    QMessageBox::warning(this, "", tr("Enter a number."));
    //    ui->lineEditFileStartNumber->setFocus();
    //    return false;
    //}
    //fileStartNumber = startNumber;

    // validate output folder
    QString folder = ui->lineEditOutputFolder->text();
    //if (folder.isEmpty())
    //{
    //    ui->lineEditOutputFolder->setFocus();
    //    QMessageBox::warning(this, "", tr("Please specify the output folder."));
    //    return false;
    //}
    if (!folder.isEmpty() && !QDir(folder).exists())
    {
        ui->lineEditOutputFolder->setFocus();
        QMessageBox::warning(this, "", tr("Output folder does not exist."));
        return false;
    }
    outputFolder = folder;
    return true;
}


// https://www.qtcentre.org/threads/8048-Validate-Data-in-QDialog
//
void ExportAnimationDialog::done(int r)
{
    if (r == QDialog::Accepted)
    {
        //// validate output folder
        //QString folder = ui->lineEditOutputFolder->text();
        //if (folder.isEmpty())
        //{
        //    ui->lineEditOutputFolder->setFocus();
        //    QMessageBox::warning(this, "", tr("Please specify the output folder."));
        //    return;
        //}
        //if (!QDir(folder).exists())
        //{
        //    ui->lineEditOutputFolder->setFocus();
        //    QMessageBox::warning(this, "", tr("Output folder does not exist."));
        //    return;
        //}
        //outputFolder = folder;
        if (!validate()) return;
    }

    // if here everythings ok
    QDialog::done(r);
}

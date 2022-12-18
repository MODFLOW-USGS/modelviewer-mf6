#include "cropdialog.h"
#include "ui_cropdialog.h"

#include "mvdoc.h"

#include <QDebug>
#include <QMessageBox>
#include <QDoubleValidator>

CropDialog::CropDialog(QWidget* parent, MvDoc* doc)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::CropDialog)
    , doc(doc)
    , _geometry{nullptr}
{
    ui->setupUi(this);
    resize(minimumSizeHint());

    QDoubleValidator* doubleValidator = new QDoubleValidator(this);

    // Controls (CCropControlsPage)
    defaultControlValues();

    ui->lineEditXDelta->setValidator(doubleValidator);
    ui->lineEditYDelta->setValidator(doubleValidator);
    ui->lineEditZDelta->setValidator(doubleValidator);

    connect(ui->doubleSpinBoxXMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mXMin = val;
                if (mXSync)
                {
                    mXMax = val;
                    ui->doubleSpinBoxXMax->setValue(val);
                }
                else
                {
                    ui->doubleSpinBoxXMax->setMinimum(val);
                }
                doc->crop(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });
    connect(ui->doubleSpinBoxXMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mXMax = val;
                if (mXSync)
                {
                    mXMin = val;
                    ui->doubleSpinBoxXMin->setValue(val);
                }
                else
                {
                    ui->doubleSpinBoxXMin->setMaximum(val);
                }
                doc->crop(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });
    connect(ui->checkBoxXSync, QOverload<int>::of(&QCheckBox::stateChanged),
            [=](int val) {
                mXSync = (val == Qt::Checked);
                if (mXSync)
                {
                    ui->doubleSpinBoxXMin->setRange(0., 1.);
                    ui->doubleSpinBoxXMax->setRange(0., 1.);
                    ui->doubleSpinBoxXMax->setValue(ui->doubleSpinBoxXMin->value());
                }
                else
                {
                    assert(ui->doubleSpinBoxXMin->value() == ui->doubleSpinBoxXMax->value());
                    ui->doubleSpinBoxXMin->setMaximum(ui->doubleSpinBoxXMin->value());
                    ui->doubleSpinBoxXMax->setMinimum(ui->doubleSpinBoxXMax->value());
                }
            });

    connect(ui->doubleSpinBoxYMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mYMin = val;
                if (mYSync)
                {
                    mYMax = val;
                    ui->doubleSpinBoxYMax->setValue(val);
                }
                else
                {
                    ui->doubleSpinBoxYMax->setMinimum(val);
                }
                doc->crop(mYMin, mYMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });
    connect(ui->doubleSpinBoxYMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mYMax = val;
                if (mYSync)
                {
                    mYMin = val;
                    ui->doubleSpinBoxYMin->setValue(val);
                }
                else
                {
                    ui->doubleSpinBoxYMin->setMaximum(val);
                }
                doc->crop(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });
    connect(ui->checkBoxYSync, QOverload<int>::of(&QCheckBox::stateChanged),
            [=](int val) {
                mYSync = (val == Qt::Checked);
                if (mYSync)
                {
                    ui->doubleSpinBoxYMin->setRange(0., 1.);
                    ui->doubleSpinBoxYMax->setRange(0., 1.);
                    ui->doubleSpinBoxYMax->setValue(ui->doubleSpinBoxYMin->value());
                }
                else
                {
                    assert(ui->doubleSpinBoxYMin->value() == ui->doubleSpinBoxYMax->value());
                    ui->doubleSpinBoxYMin->setMaximum(ui->doubleSpinBoxYMin->value());
                    ui->doubleSpinBoxYMax->setMinimum(ui->doubleSpinBoxYMax->value());
                }
            });

    connect(ui->doubleSpinBoxZMin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mZMin = val;
                if (mZSync)
                {
                    mZMax = val;
                    ui->doubleSpinBoxZMax->setValue(val);
                }
                else
                {
                    ui->doubleSpinBoxZMax->setMinimum(val);
                }
                doc->crop(mZMin, mZMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });
    connect(ui->doubleSpinBoxZMax, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            [=](double val) {
                mZMax = val;
                if (mZSync)
                {
                    mZMin = val;
                    ui->doubleSpinBoxZMin->setValue(val);
                }
                else
                {
                    ui->doubleSpinBoxZMin->setMaximum(val);
                }
                doc->crop(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });
    connect(ui->checkBoxZSync, QOverload<int>::of(&QCheckBox::stateChanged),
            [=](int val) {
                mZSync = (val == Qt::Checked);
                if (mZSync)
                {
                    ui->doubleSpinBoxZMin->setRange(0., 1.);
                    ui->doubleSpinBoxZMax->setRange(0., 1.);
                    ui->doubleSpinBoxZMax->setValue(ui->doubleSpinBoxZMin->value());
                }
                else
                {
                    assert(ui->doubleSpinBoxZMin->value() == ui->doubleSpinBoxZMax->value());
                    ui->doubleSpinBoxZMin->setMaximum(ui->doubleSpinBoxZMin->value());
                    ui->doubleSpinBoxZMax->setMinimum(ui->doubleSpinBoxZMax->value());
                }
            });

    connect(ui->spinBoxCropAngle, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                mCropAngle = val;
                doc->crop(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
            });


    // Pieces Page (Options)
    ui->sliderRed->setRange(0, 100);
    ui->sliderGreen->setRange(0, 100);
    ui->sliderBlue->setRange(0, 100);
    ui->sliderOpacity->setRange(0, 100);

    defaultPiecesValues();                                                               // must setRange before setting values

    connect(ui->checkBoxShow, QOverload<int>::of(&QCheckBox::stateChanged),
            [=](int val) {
                showCroppedAwayPieces = (val == Qt::Checked);
                if (showCroppedAwayPieces)
                {
                    doc->showCroppedAwayPieces();
                }
                else
                {
                    doc->hideCroppedAwayPieces();
                }
                ui->sliderRed->setEnabled(showCroppedAwayPieces);
                ui->sliderGreen->setEnabled(showCroppedAwayPieces);
                ui->sliderBlue->setEnabled(showCroppedAwayPieces);
                ui->sliderOpacity->setEnabled(showCroppedAwayPieces);
            });

    connect(ui->sliderRed, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                red = val;
                doc->setCroppedAwayPiecesColor(red * 0.01, green * 0.01, blue * 0.01);
                updateLabelsPieces();
            });
    connect(ui->sliderGreen, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                green = val;
                doc->setCroppedAwayPiecesColor(red * 0.01, green * 0.01, blue * 0.01);
                updateLabelsPieces();
            });
    connect(ui->sliderBlue, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                blue = val;
                doc->setCroppedAwayPiecesColor(red * 0.01, green * 0.01, blue * 0.01);
                updateLabelsPieces();
            });
    connect(ui->sliderOpacity, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                opacity = val;
                doc->setCroppedAwayPiecesOpacity(opacity * 0.01);
                updateLabelsPieces();
            });

    // tab
    connect(ui->tabWidget, QOverload<int>::of(&QTabWidget::currentChanged), this, &CropDialog::onTabChanged);

    // buttons
    connect(ui->pushButtonApply, &QAbstractButton::clicked, this, &CropDialog::onApply);
    connect(ui->pushButtonDefault, &QAbstractButton::clicked, this, &CropDialog::onDefault);
    connect(ui->pushButtonDone, &QAbstractButton::clicked, this, &QWidget::hide);
}

CropDialog::~CropDialog()
{
    delete ui;
    delete _geometry;
}

void CropDialog::hideEvent(QHideEvent* e)
{
    delete _geometry;
    _geometry = new QRect(geometry());
}

void CropDialog::showEvent(QShowEvent* e)
{
    if (_geometry)
    {
        setGeometry(*_geometry);
    }
}

void CropDialog::onApply()
{
    int n = this->ui->tabWidget->currentIndex();
    switch (n)
    {
    case 0: // Controls (CCropControlsPage)
        applyControls();
        break;
    case 1: // Pieces Page (Options)
        // no-op
        break;
    default:
        assert(false);
        break;
    }
}

void CropDialog::onDefault()
{
    const QMessageBox::StandardButton ret = QMessageBox::question(this,
                                                                  tr("Model Viewer for Modflow 6"),
                                                                  tr("Do you want to use default settings?"),
                                                                  QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::No)
    {
        return;
    }

    switch (ui->tabWidget->currentIndex())
    {
    case 0:
        // Controls (CCropControlsPage)
        defaultControls();
        break;
    case 1:
        // Pieces (CCropOptionsPage)
        defaultPieces();
        break;
    default:
        assert(false);
        break;
    }
}

void CropDialog::reinitialize()
{
    // Controls (CCropControlsPage)
    reinitializeControls();

    // Pieces (CCropOptionsPage)
    reinitializePieces();

    ui->pushButtonApply->setEnabled(false);
    ui->pushButtonDefault->setEnabled(false);
}

void CropDialog::activate(bool b)
{
    // Controls (CCropControlsPage)
    activateControls(b);

    // Pieces (CCropOptionsPage)
    activatePieces(b);

    ui->pushButtonApply->setEnabled(b);
    ui->pushButtonDefault->setEnabled(b);
}

void CropDialog::activateControls(bool b)
{
#if !defined(DONT_DISABLE_LABELS)
    ui->labelX->setEnabled(b);
    ui->labelY->setEnabled(b);
    ui->labelZ->setEnabled(b);
    ui->labelMin->setEnabled(b);
    ui->labelMax->setEnabled(b);
    ui->labelDelta->setEnabled(b);
    ui->labelSync->setEnabled(b);
    ui->labelCropAngle->setEnabled(b);
#endif

    ui->doubleSpinBoxXMin->setEnabled(b);
    ui->doubleSpinBoxXMax->setEnabled(b);
    ui->doubleSpinBoxYMin->setEnabled(b);
    ui->doubleSpinBoxYMax->setEnabled(b);
    ui->doubleSpinBoxZMin->setEnabled(b);
    ui->doubleSpinBoxZMax->setEnabled(b);
    ui->lineEditXDelta->setEnabled(b);
    ui->lineEditYDelta->setEnabled(b);
    ui->lineEditZDelta->setEnabled(b);
    ui->checkBoxXSync->setEnabled(b);
    ui->checkBoxYSync->setEnabled(b);
    ui->checkBoxZSync->setEnabled(b);
    ui->spinBoxCropAngle->setEnabled(b);
}

void CropDialog::activatePieces(bool b)
{
#if !defined(DONT_DISABLE_LABELS)
    ui->labelRedHeading->setEnabled(b);
    ui->labelGreenHeading->setEnabled(b);
    ui->labelBlueHeading->setEnabled(b);
    ui->labelOpacityHeading->setEnabled(b);

    ui->labelRed->setEnabled(b);
    ui->labelGreen->setEnabled(b);
    ui->labelBlue->setEnabled(b);
    ui->labelOpacity->setEnabled(b);
#endif

    ui->checkBoxShow->setEnabled(b);
    ui->sliderRed->setEnabled(b);
    ui->sliderGreen->setEnabled(b);
    ui->sliderBlue->setEnabled(b);
    ui->sliderOpacity->setEnabled(b);
}

void CropDialog::applyControls()
{
    if (updateDataControls(true))
    {
        doc->crop(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
    }
}

void CropDialog::defaultControls()
{
    defaultControlValues();
    doc->crop(mXMin, mXMax, mYMin, mYMax, mZMin, mZMax, mCropAngle);
}

void CropDialog::defaultPieces()
{
    defaultPiecesValues();
    doc->hideCroppedAwayPieces();
    doc->setCroppedAwayPiecesColor(red * 0.01, green * 0.01, blue * 0.01);
    doc->setCroppedAwayPiecesOpacity(opacity * 0.01);
}

void CropDialog::defaultControlValues()
{
    mXMin      = 0.0;
    mXMax      = 1.0;
    mXDelta    = 0.1;
    mXSync     = false;

    mYMin      = 0.0;
    mYMax      = 1.0;
    mYDelta    = 0.1;
    mYSync     = false;

    mZMin      = 0.0;
    mZMax      = 1.0;
    mZDelta    = 0.1;
    mZSync     = false;

    mCropAngle = 0;

    ui->checkBoxXSync->setChecked(false);
    ui->checkBoxYSync->setChecked(false);
    ui->checkBoxZSync->setChecked(false);

    ui->doubleSpinBoxXMin->setRange(0., 1.);
    ui->doubleSpinBoxXMax->setRange(0., 1.);
    ui->doubleSpinBoxYMin->setRange(0., 1.);
    ui->doubleSpinBoxYMax->setRange(0., 1.);
    ui->doubleSpinBoxZMin->setRange(0., 1.);
    ui->doubleSpinBoxZMax->setRange(0., 1.);
    ui->spinBoxCropAngle->setRange(-45, 45);

    ui->doubleSpinBoxXMin->setValue(mXMin);
    ui->doubleSpinBoxXMax->setValue(mXMax);
    ui->doubleSpinBoxYMin->setValue(mYMin);
    ui->doubleSpinBoxYMax->setValue(mYMax);
    ui->doubleSpinBoxZMin->setValue(mZMin);
    ui->doubleSpinBoxZMax->setValue(mZMax);

    ui->doubleSpinBoxXMin->setDecimals(std::ceil(std::abs(std::log10(mXDelta))));
    ui->doubleSpinBoxXMax->setDecimals(std::ceil(std::abs(std::log10(mXDelta))));
    ui->doubleSpinBoxYMin->setDecimals(std::ceil(std::abs(std::log10(mYDelta))));
    ui->doubleSpinBoxYMax->setDecimals(std::ceil(std::abs(std::log10(mYDelta))));
    ui->doubleSpinBoxZMin->setDecimals(std::ceil(std::abs(std::log10(mZDelta))));
    ui->doubleSpinBoxZMax->setDecimals(std::ceil(std::abs(std::log10(mZDelta))));

    ui->doubleSpinBoxXMin->setSingleStep(mXDelta);
    ui->doubleSpinBoxXMax->setSingleStep(mXDelta);
    ui->doubleSpinBoxYMin->setSingleStep(mYDelta);
    ui->doubleSpinBoxYMax->setSingleStep(mYDelta);
    ui->doubleSpinBoxZMin->setSingleStep(mZDelta);
    ui->doubleSpinBoxZMax->setSingleStep(mZDelta);

    ui->lineEditXDelta->setText(QString(tr("%1")).arg(mXDelta, 0, 'g'));
    ui->lineEditYDelta->setText(QString(tr("%1")).arg(mYDelta, 0, 'g'));
    ui->lineEditZDelta->setText(QString(tr("%1")).arg(mZDelta, 0, 'g'));

    ui->spinBoxCropAngle->setValue(mCropAngle);

    updateDataControls(false);
}

void CropDialog::defaultPiecesValues()
{
    showCroppedAwayPieces = false;
    red                   = 100;
    green                 = 80;
    blue                  = 60;
    opacity               = 20;

    ui->checkBoxShow->setChecked(showCroppedAwayPieces);
    ui->sliderRed->setValue(red);
    ui->sliderGreen->setValue(green);
    ui->sliderBlue->setValue(blue);
    ui->sliderOpacity->setValue(opacity);
}

void CropDialog::reinitializeControls()
{
    defaultControlValues();
    activateControls(false);
}

void CropDialog::reinitializePieces()
{
    defaultPiecesValues();
    activatePieces(false);
}

bool CropDialog::updateDataControls(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        QString s;
        double  val;
        bool    ok;

        // mXDelta
        s   = ui->lineEditXDelta->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditXDelta->setFocus();
            return false;
        }
        if (val < 0 || val > 1)
        {
            QMessageBox::warning(this, "", tr("Enter a number between 0 and 1."));
            ui->lineEditXDelta->setFocus();
            return false;
        }
        mXDelta = val;
        ui->doubleSpinBoxXMin->setSingleStep(mXDelta);
        ui->doubleSpinBoxXMax->setSingleStep(mXDelta);
        ui->doubleSpinBoxXMin->setDecimals(std::ceil(std::abs(std::log10(mXDelta))));
        ui->doubleSpinBoxXMax->setDecimals(std::ceil(std::abs(std::log10(mXDelta))));

        // mYDelta
        s   = ui->lineEditYDelta->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditYDelta->setFocus();
            return false;
        }
        if (val < 0 || val > 1)
        {
            QMessageBox::warning(this, "", tr("Enter a number between 0 and 1."));
            ui->lineEditYDelta->setFocus();
            return false;
        }
        mYDelta = val;
        ui->doubleSpinBoxYMin->setSingleStep(mYDelta);
        ui->doubleSpinBoxYMax->setSingleStep(mYDelta);
        ui->doubleSpinBoxYMin->setDecimals(std::ceil(std::abs(std::log10(mYDelta))));
        ui->doubleSpinBoxYMax->setDecimals(std::ceil(std::abs(std::log10(mYDelta))));

        // mZDelta
        s   = ui->lineEditZDelta->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditZDelta->setFocus();
            return false;
        }
        if (val < 0 || val > 1)
        {
            QMessageBox::warning(this, "", tr("Enter a number between 0 and 1."));
            ui->lineEditZDelta->setFocus();
            return false;
        }
        mZDelta = val;
        ui->doubleSpinBoxZMin->setSingleStep(mZDelta);
        ui->doubleSpinBoxZMax->setSingleStep(mZDelta);
        ui->doubleSpinBoxZMin->setDecimals(std::ceil(std::abs(std::log10(mZDelta))));
        ui->doubleSpinBoxZMax->setDecimals(std::ceil(std::abs(std::log10(mZDelta))));
    }
    else
    {
        ui->doubleSpinBoxXMin->setValue(mXMin);
        ui->doubleSpinBoxXMax->setValue(mXMax);
        ui->doubleSpinBoxYMin->setValue(mYMin);
        ui->doubleSpinBoxYMax->setValue(mYMax);
        ui->doubleSpinBoxZMin->setValue(mZMin);
        ui->doubleSpinBoxZMax->setValue(mZMax);

        ui->lineEditXDelta->setText(QString("%1").number(mXDelta, 'g'));
        ui->lineEditYDelta->setText(QString("%1").number(mYDelta, 'g'));
        ui->lineEditZDelta->setText(QString("%1").number(mZDelta, 'g'));

        ui->doubleSpinBoxXMin->setSingleStep(mXDelta);
        ui->doubleSpinBoxXMax->setSingleStep(mXDelta);
        ui->doubleSpinBoxXMin->setDecimals(abs(log10(mXDelta)));
        ui->doubleSpinBoxXMax->setDecimals(abs(log10(mXDelta)));

        ui->doubleSpinBoxYMin->setSingleStep(mYDelta);
        ui->doubleSpinBoxYMax->setSingleStep(mYDelta);
        ui->doubleSpinBoxYMin->setDecimals(abs(log10(mYDelta)));
        ui->doubleSpinBoxYMax->setDecimals(abs(log10(mYDelta)));

        ui->doubleSpinBoxZMin->setSingleStep(mZDelta);
        ui->doubleSpinBoxZMax->setSingleStep(mZDelta);
        ui->doubleSpinBoxZMin->setDecimals(abs(log10(mZDelta)));
        ui->doubleSpinBoxZMax->setDecimals(abs(log10(mZDelta)));

        ui->spinBoxCropAngle->setValue(mCropAngle);
    }
    return true;
}

bool CropDialog::updateDataPieces(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        assert(ui->checkBoxShow->isChecked() == showCroppedAwayPieces);
        assert(red     == ui->sliderRed->value());
        assert(green   == ui->sliderGreen->value());
        assert(blue    == ui->sliderBlue->value());
        assert(opacity == ui->sliderOpacity->value());
    }
    else
    {
        assert(0 <= red     && red     <= 100);
        assert(0 <= green   && green   <= 100);
        assert(0 <= blue    && blue    <= 100);
        assert(0 <= opacity && opacity <= 100);

        if (showCroppedAwayPieces)
        {
            ui->checkBoxShow->setChecked(true);
        }
        else
        {
            ui->checkBoxShow->setChecked(false);
        }
        ui->sliderRed->setValue(red);
        ui->sliderGreen->setValue(green);
        ui->sliderBlue->setValue(blue);
        ui->sliderOpacity->setValue(opacity);

        updateLabelsPieces();
    }
    return true;
}

void CropDialog::setCurrentTabIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

void CropDialog::onTabChanged(int idx)
{
    switch (idx)
    {
    case 0:
        ui->pushButtonApply->setEnabled(ui->spinBoxCropAngle->isEnabled());
        if (ui->pushButtonApply->isEnabled()) ui->pushButtonApply->setDefault(true);
        break;
    case 1:
        ui->pushButtonDone->setDefault(true);
        ui->pushButtonApply->setEnabled(false);
        break;
    default:
        assert(false);
        break;
    }
}

void CropDialog::updateLabelsPieces()
{
    ui->labelRed->setText(QString(tr("%1")).arg(red / 100.0, 0, 'f', 2));
    ui->labelGreen->setText(QString(tr("%1")).arg(green / 100.0, 0, 'f', 2));
    ui->labelBlue->setText(QString(tr("%1")).arg(blue / 100.0, 0, 'f', 2));
    ui->labelOpacity->setText(QString(tr("%1")).arg(opacity / 100.0, 0, 'f', 2));
}

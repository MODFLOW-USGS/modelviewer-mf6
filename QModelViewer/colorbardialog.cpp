#include "colorbardialog.h"
#include "ui_colorbardialog.h"

#include <QColorDialog>
#include <QMessageBox>

#include "mvdoc.h"

#include <limits>

#define MV_DATASET  0
#define MV_PATHLINE 1


ColorBarDialog::ColorBarDialog(QWidget *parent, MvDoc *doc)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::ColorBarDialog)
    , doc(doc)
    , _geometry(nullptr)
    , dataSourceIndex{0}
{
    ui->setupUi(this);

    // Source
    // @todo PATHLINES need pathlines to test
    // ui->comboBoxSource

    // Limits
    connect(ui->pushButtonReverse, &QAbstractButton::clicked, this, &ColorBarDialog::onReverse);

    // Logarithmic scale (QCheckBox)
    // void QAbstractButton::toggled(bool checked)  // user interaction and calling setChecked() (https://doc.qt.io/qt-5/qabstractbutton.html#toggled)
    // void QCheckBox::stateChanged(int state)      // user interaction only? (https://doc.qt.io/qt-5/qcheckbox.html#stateChanged)
    connect(ui->checkBoxLogScale, QOverload<int>::of(&QCheckBox::stateChanged), this, &ColorBarDialog::onLogScale);

    // Size
    ui->spinBoxWidth->setMaximum(std::numeric_limits<int>::max());
    ui->spinBoxHeight->setMaximum(std::numeric_limits<int>::max());
    ui->spinBoxOffset->setMaximum(std::numeric_limits<int>::max());
    connect(ui->spinBoxWidth, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                width = val;
                doc->SetColorBarSize(width, height, offset);
            });
    connect(ui->spinBoxHeight, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                height = val;
                doc->SetColorBarSize(width, height, offset);
            });
    connect(ui->spinBoxOffset, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                offset = val;
                doc->SetColorBarSize(width, height, offset);
            });

    // Labels (text)
    ui->spinBoxFontSize->setMaximum(VTK_INT_MAX);
    ui->spinBoxNumLabels->setMaximum(std::numeric_limits<int>::max());
    ui->spinBoxPrecision->setMaximum(std::numeric_limits<double>::digits10); // DBL_DIG
    connect(ui->radioButtonBlack, &QAbstractButton::clicked,
            [=]() {
                doc->SetColorBarTextColor(0, 0, 0);
            });
    connect(ui->radioButtonGray, &QAbstractButton::clicked,
            [=]() {
                doc->SetColorBarTextColor(0.5f, 0.5f, 0.5f);
            });
    connect(ui->radioButtonWhite, &QAbstractButton::clicked,
            [=]() {
                doc->SetColorBarTextColor(1, 1, 1);
            });
    connect(ui->spinBoxFontSize, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                fontSize = val;
                doc->SetColorBarFontSize(val, true);
            });
    connect(ui->spinBoxNumLabels, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                numLabels = val;
                doc->SetColorBarNumberOfLabels(val, true);
            });
    connect(ui->spinBoxPrecision, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                precision = val;
                doc->SetColorBarLabelPrecision(val, true);
            });

    // Colors
    firstColor = QColor(90, 90, 90);
    lastColor  = QColor(240, 240, 240);
    connect(ui->pushButtonLowerColor, &QAbstractButton::clicked, this, &ColorBarDialog::onLowerColor);
    connect(ui->pushButtonUpperColor, &QAbstractButton::clicked, this, &ColorBarDialog::onUpperColor);
    connect(ui->comboBoxColorScheme, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int idx) {
                doc->SetColorBarColorScheme(idx);
                activate(true);
            });

    // buttons
    connect(ui->pushButtonApply, &QAbstractButton::clicked, this, &ColorBarDialog::onApply);
    connect(ui->pushButtonDefault, &QAbstractButton::clicked, this, &ColorBarDialog::onDefault);
    connect(ui->pushButtonDone, &QAbstractButton::clicked, [=]() { hide(); });
}

ColorBarDialog::~ColorBarDialog()
{
    delete ui;
    delete _geometry;
}

void ColorBarDialog::hideEvent(QHideEvent *e)
{
    delete _geometry;
    _geometry = new QRect(geometry());
}

void ColorBarDialog::showEvent(QShowEvent *e)
{
    if (_geometry)
    {
        setGeometry(*_geometry);
    }
}

void ColorBarDialog::reinitialize()
{
    // Source
    ui->comboBoxSource->setCurrentIndex(MV_DATASET);

    // Limits
    ui->lineEditValueRed->setText("");
    ui->lineEditValueBlue->setText("");
    ui->checkBoxLogScale->setChecked(false);

    // Size
    onDefaultSize();

    // Labels (Text)
    onDefaultLabels();

    // Colors
    onDefaultColors();

    activate(false);
}

bool ColorBarDialog::updateData(bool saveAndValidate)
{
    int n = ui->tabWidget->currentIndex();
    switch (n)
    {
    case 0: // Source
        return updateDataSource(saveAndValidate);
        break;
    case 1: // Limits
        return updateDataLimits(saveAndValidate);
        break;
    case 2: // Size
        return updateDataSize(saveAndValidate);
        break;
    case 3: // Labels (Text)
        return updateDataLabels(saveAndValidate);
        break;
    case 4: // Colors
        return updateDataColors(saveAndValidate);
        break;
    default:
        break;
    }
    return true;
}

bool ColorBarDialog::updateDataSource(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        // Source
        dataSourceIndex = ui->comboBoxSource->currentIndex();
        assert(dataSourceIndex == 0); // @todo PATHLINES
    }
    else
    {
        // Source
        assert(dataSourceIndex == 0); // @todo PATHLINES
        ui->comboBoxSource->setCurrentIndex(dataSourceIndex);
    }
    return true;
}

bool ColorBarDialog::updateDataLimits(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        QString s;
        double  val;
        bool    ok;

        // valueBlue
        s   = ui->lineEditValueBlue->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Please enter a number."));
            ui->lineEditValueBlue->setFocus();
            return false;
        }
        valueBlue = val;
        if (ui->checkBoxLogScale->isChecked() && valueBlue <= 0)
        {
            QMessageBox::warning(this, "", tr("Please ensure that \"Value at blue limit\" is positive for logarithmic scaling."));
            ui->lineEditValueRed->setFocus();
            return false;
        }


        // valueRed
        s         = ui->lineEditValueRed->text();
        val       = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Please enter a number."));
            ui->lineEditValueRed->setFocus();
            return false;
        }
        valueRed = val;
        if (ui->checkBoxLogScale->isChecked() && valueRed <= 0)
        {
            QMessageBox::warning(this, "", tr("Please ensure that \"Value at red limit\" is positive for logarithmic scaling."));
            ui->lineEditValueRed->setFocus();
            return false;
        }

        if (valueRed == valueBlue)
        {
            QMessageBox::warning(this, "", tr("Please ensure that \"Value at red limit\" is different than \"Value at blue limit\"."));
            ui->lineEditValueRed->setFocus();
            return false;
        }

        isColorBarLinear = !ui->checkBoxLogScale->isChecked();
    }
    else
    {
        ui->lineEditValueBlue->setText(QString("%1").arg(valueBlue));
        ui->lineEditValueRed->setText(QString("%1").arg(valueRed));
        ui->checkBoxLogScale->setChecked(!isColorBarLinear);
    }
    return true;
}

bool ColorBarDialog::updateDataSize(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        // no-op
    }
    else
    {
        ui->spinBoxWidth->setValue(width);
        ui->spinBoxHeight->setValue(height);
        ui->spinBoxOffset->setValue(offset);
    }
    return true;
}

bool ColorBarDialog::updateDataLabels(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        // no-op
    }
    else
    {
        switch (colorOption)
        {
        case 0:
            ui->radioButtonBlack->setChecked(true);
            break;
        case 1:
            ui->radioButtonGray->setChecked(true);
            break;
        case 2:
            ui->radioButtonWhite->setChecked(true);
            break;
        default:
            assert(false);
            break;
        }

        ui->spinBoxFontSize->setValue(fontSize);
        ui->spinBoxNumLabels->setValue(numLabels);
        ui->spinBoxPrecision->setValue(precision);
    }
    return true;
}

bool ColorBarDialog::updateDataColors(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        // no-op
    }
    else
    {
        ui->comboBoxColorScheme->setCurrentIndex((doc->GetColorBarColorScheme()));
    }
    return true;
}

void ColorBarDialog::onApply()
{
    if (updateData(true))
    {
        int n = this->ui->tabWidget->currentIndex();

        switch (n)
        {
        case 0: // source
            applyDataSource();
            break;
        case 1: // Limits
            applyDataLimits();
            break;
        case 2: // Size
            applyDataSize();
            break;
        case 3: // Labels (Text) -- CColorBarTextPage::Apply()
            applyDataLabels();
            break;
        case 4: // Colors
            applyDataColors();
            break;
        default:
            assert(false);
            break;
        }
    }
}

void ColorBarDialog::onDefault()
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
    case 0: // Source
        onDefaultSource();
        break;
    case 1: // Limits
        onDefaultLimits();
        break;
    case 2: // Size
        onDefaultSize();
        break;
    case 3: // Labels (Text)
        onDefaultLabels();
        break;
    case 4: // Colors
        onDefaultColors();
        break;
    default:
        assert(false);
        break;
    }
}

void ColorBarDialog::onDefaultSource()
{
    dataSourceIndex = MV_DATASET;
    doc->setColorBarSource(dataSourceIndex);
    double valueBlue = doc->getColorBarValueBlue();
    double valueRed  = doc->getColorBarValueRed();
    doc->setColorBarEndPoints(valueBlue, valueRed);
    doc->updateColorBarDialog();
    doc->updatePathlinesDialog();
}

void ColorBarDialog::onDefaultLimits()
{
    double range[2];

    if (doc->getColorBarSource() == MV_DATASET)
    {
        doc->getScalarDataRange(range);
    }
    else
    {
        doc->getPathlineTimeRange(range);
    }

    if (valueBlue < valueRed)
    {
        if (range[0] == range[1])
        {
            valueBlue = range[0] - 1;
            valueRed  = range[1] + 1;
        }
        else
        {
            valueBlue = range[0];
            valueRed  = range[1];
        }
    }
    else
    {
        if (range[0] == range[1])
        {
            valueBlue = range[1] + 1;
            valueRed  = range[0] - 1;
        }
        else
        {
            valueBlue = range[1];
            valueRed  = range[0];
        }
    }
    isColorBarLinear = true;
    updateDataLimits(false);
    doc->UseLinearColorBar();
    doc->setColorBarEndPoints(valueBlue, valueRed);
}

void ColorBarDialog::onDefaultSize()
{
    ui->spinBoxWidth->setValue(20);
    ui->spinBoxHeight->setValue(200);
    ui->spinBoxOffset->setValue(100);
}

void ColorBarDialog::onDefaultLabels()
{
    ui->spinBoxFontSize->setValue(14);
    ui->spinBoxNumLabels->setValue(5);
    ui->spinBoxPrecision->setValue(2);
}

void ColorBarDialog::onDefaultColors()
{
    ui->comboBoxColorScheme->setCurrentIndex(0);
    doc->SetColorBarColorScheme(0);

    firstColor   = QColor(90, 90, 90);
    lastColor    = QColor(240, 240, 240);

    ui->listWidgetFirst->setStyleSheet(QString("background-color: %1").arg(firstColor.name()));
    doc->SetColorBarFirstCustomColor(firstColor);

    ui->listWidgetLast->setStyleSheet(QString("background-color: %1").arg(lastColor.name()));
    doc->SetColorBarLastCustomColor(lastColor);

    activate(true);
}

void ColorBarDialog::activate(bool b)
{
    // Source
    ui->labelSource->setEnabled(b && doc->hasPathlineData());
    ui->comboBoxSource->setEnabled(b && doc->hasPathlineData());

    // Limits
    ui->labelValueBlue->setEnabled(b);
    ui->labelValueRed->setEnabled(b);

    ui->lineEditValueBlue->setEnabled(b);
    ui->lineEditValueRed->setEnabled(b);
    ui->pushButtonReverse->setEnabled(b);
    ui->checkBoxLogScale->setEnabled(b);

    // Size
    ui->labelHeight->setEnabled(b);
    ui->labelWidth->setEnabled(b);
    ui->labelOffset->setEnabled(b);

    ui->spinBoxHeight->setEnabled(b);
    ui->spinBoxWidth->setEnabled(b);
    ui->spinBoxOffset->setEnabled(b);

    // Labels (Text)
    ui->labelFontSize->setEnabled(b);
    ui->labelNumLabels->setEnabled(b);
    ui->labelPrecision->setEnabled(b);

    ui->spinBoxFontSize->setEnabled(b);
    ui->spinBoxNumLabels->setEnabled(b);
    ui->spinBoxPrecision->setEnabled(b);
    ui->radioButtonBlack->setEnabled(b);
    ui->radioButtonGray->setEnabled(b);
    ui->radioButtonWhite->setEnabled(b);

    // Colors
    b = b && (doc->getColorBarSource() == 0);
    ui->labelColorScheme->setEnabled(b);
    ui->comboBoxColorScheme->setEnabled(b);
    if (b)
    {
        int selection = ui->comboBoxColorScheme->currentIndex();
        ui->pushButtonLowerColor->setEnabled(selection == 2);
        ui->pushButtonUpperColor->setEnabled(selection == 2);
    }
    else
    {
        ui->pushButtonLowerColor->setEnabled(false);
        ui->pushButtonUpperColor->setEnabled(false);
    }

    // buttons
    ui->pushButtonApply->setEnabled(b);
    ui->pushButtonDefault->setEnabled(b);
}

void ColorBarDialog::setCurrentIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

void ColorBarDialog::applyDataSource()
{
    // @todo PATHLINES need pathlines
}

void ColorBarDialog::applyDataLimits()
{
    if (updateDataLimits(true))
    {
        doc->setColorBarEndPoints(valueBlue, valueRed);
        doc->updatePathlinesDialog();
    }
}

void ColorBarDialog::applyDataSize()
{
    // no-op
}

void ColorBarDialog::applyDataLabels()
{
    // no-op
}

void ColorBarDialog::applyDataColors()
{
    // no-op
}

void ColorBarDialog::onLowerColor()
{
    QColor first = doc->GetColorBarFirstCustomColor();
    QColor color = QColorDialog::getColor(first, this, tr("Color"));
    if (color.isValid())
    {
        ui->listWidgetFirst->setStyleSheet(QString("background-color: %1").arg(color.name()));
        doc->SetColorBarFirstCustomColor(color);
    }
}

void ColorBarDialog::onUpperColor()
{
    QColor last = doc->GetColorBarLastCustomColor();
    QColor color = QColorDialog::getColor(last, this, tr("Color"));
    if (color.isValid())
    {
        ui->listWidgetLast->setStyleSheet(QString("background-color: %1").arg(color.name()));
        doc->SetColorBarLastCustomColor(color);
    }
}

void ColorBarDialog::onReverse()
{
    if (updateDataLimits(true))
    {
        double temp = valueRed;
        valueRed  = valueBlue;
        valueBlue = temp;
        updateDataLimits(false);
        doc->setColorBarEndPoints(valueBlue, valueRed);
    }
}

void ColorBarDialog::onLogScale(int state)
{
    if (updateDataLimits(true))
    {
        doc->setColorBarEndPoints(valueBlue, valueRed);
        if (doc->getColorBarSource() == MV_DATASET)
        {
            if (isColorBarLinear)
            {
                assert(state == Qt::Unchecked);
                doc->UseLinearColorBar();
            }
            else
            {
                assert(state == Qt::Checked);
                doc->UseLogColorBar();
            }
        }
        else
        {
            // doc->SetPathlineLogTransform(!isColorBarLinear);  @todo PATHLINES
            doc->updatePathlinesDialog();
        }
    }
    else
    {
        ui->checkBoxLogScale->setChecked(false);
    }
}

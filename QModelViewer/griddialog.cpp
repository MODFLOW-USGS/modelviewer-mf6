#include "griddialog.h"
#include "ui_griddialog.h"

#include "mvdoc.h"
#include "mvDefine.h"

#include <vtkColor.h>
#include <vtkNamedColors.h>

#include <QMessageBox>


GridDialog::GridDialog(QWidget * parent, MvDoc * doc)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::GridDialog)
    , doc(doc)
    , _geometry{nullptr}
    , structuredGrid{true}
{
    ui->setupUi(this);
    reinitialize();
    resize(minimumSizeHint());

    // Lines
    vtkNew<vtkNamedColors> colors;
    vtkColor3d             Black = colors->GetColor3d("Black");
    vtkColor3d             Gray  = colors->GetColor3d("Gray");
    vtkColor3d             White = colors->GetColor3d("White");
    connect(ui->radioButtonBlack, &QAbstractButton::clicked, [=]() { doc->setGridLineColor(Black); });
    connect(ui->radioButtonGray,  &QAbstractButton::clicked, [=]() { doc->setGridLineColor(Gray); });
    connect(ui->radioButtonWhite, &QAbstractButton::clicked, [=]() { doc->setGridLineColor(White); });

    connect(ui->checkBoxActivateX, QOverload<int>::of(&QCheckBox::stateChanged), this, &GridDialog::onActivateX);
    connect(ui->checkBoxActivateY, QOverload<int>::of(&QCheckBox::stateChanged), this, &GridDialog::onActivateY);
    connect(ui->checkBoxActivateZ, QOverload<int>::of(&QCheckBox::stateChanged), this, &GridDialog::onActivateZ);

    // Size
    connect(ui->spinBoxPositionX, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                Q_ASSERT(structuredGrid);
                Q_ASSERT(XMin <= val && val <= XMax);
                positionX = val;
                doc->setGridLinePositions(positionX, positionY, positionZ);
            });
    connect(ui->spinBoxPositionY, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                assert(structuredGrid);
                Q_ASSERT(YMin <= val && val <= YMax);
                positionY = val;
                doc->setGridLinePositions(positionX, positionY, positionZ);
            });
    connect(ui->spinBoxPositionZ, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                Q_ASSERT(ZMin <= val && val <= ZMax);
                positionZ = val;
                if (structuredGrid)
                    doc->setGridLinePositions(positionX, positionY, positionZ);
                else
                    doc->setGridLayerPosition(positionZ);
            });

    // Shell
    ui->sliderRed->setRange(0, 100);
    ui->sliderGreen->setRange(0, 100);
    ui->sliderBlue->setRange(0, 100);
    ui->sliderOpacity->setRange(0, 100);

    connect(ui->sliderRed, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                red = val;
                doc->setGridShellColor(red * 0.01, green * 0.01, blue * 0.01);
                updateLabelsShell();
            });
    connect(ui->sliderGreen, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                green = val;
                doc->setGridShellColor(red * 0.01, green * 0.01, blue * 0.01);
                updateLabelsShell();
            });
    connect(ui->sliderBlue, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                blue = val;
                doc->setGridShellColor(red * 0.01, green * 0.01, blue * 0.01);
                updateLabelsShell();
            });
    connect(ui->sliderOpacity, QOverload<int>::of(&QAbstractSlider::valueChanged),
            [=](int val) {
                opacity = val;
                doc->setGridShellOpacity(opacity * 0.01);
                updateLabelsShell();
            });

    // Subgrid
    connect(ui->checkBoxSubgrid, QOverload<int>::of(&QCheckBox::stateChanged), this, &GridDialog::onCheckBoxSubgrid);
    connect(ui->spinBoxLowI, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                col_min = val;
                if (col_min > col_max) ui->spinBoxHighI->setValue(col_min);
                if (isSubgridActivated) applySubgrid();
            });
    connect(ui->spinBoxHighI, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                col_max = val;
                if (col_min > col_max) ui->spinBoxLowI->setValue(col_max);
                if (isSubgridActivated) applySubgrid();
            });

    connect(ui->spinBoxLowJ, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                row_min = val;
                if (row_min > row_max) ui->spinBoxHighJ->setValue(row_min);
                if (isSubgridActivated) applySubgrid();
            });
    connect(ui->spinBoxHighJ, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                row_max = val;
                if (row_min > row_max) ui->spinBoxLowJ->setValue(row_max);
                if (isSubgridActivated) applySubgrid();
            });

    connect(ui->spinBoxLowK, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                lay_min = val;
                if (lay_min > lay_max) ui->spinBoxHighK->setValue(lay_min);
                if (isSubgridActivated) applySubgrid();
            });
    connect(ui->spinBoxHighK, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                lay_max = val;
                if (lay_min > lay_max) ui->spinBoxLowK->setValue(lay_max);
                if (isSubgridActivated) applySubgrid();
            });

    // buttons
    ui->pushButtonApply->setVisible(false);
    connect(ui->pushButtonDone, &QAbstractButton::clicked, [=]() { hide(); });
}

GridDialog::~GridDialog()
{
    delete ui;
    delete _geometry;
}

void GridDialog::hideEvent(QHideEvent *e)
{
    delete _geometry;
    _geometry = new QRect(geometry());
}

void GridDialog::showEvent(QShowEvent *e)
{
    if (_geometry)
    {
        setGeometry(*_geometry);
    }
}

bool GridDialog::updateDataLines(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        assert(positionX == ui->spinBoxPositionX->value());
        assert(positionY == ui->spinBoxPositionY->value());
        assert(positionZ == ui->spinBoxPositionZ->value());
    }
    else
    {
        QSignalBlocker blockSpinX(ui->spinBoxPositionX);
        QSignalBlocker blockSpinY(ui->spinBoxPositionY);
        QSignalBlocker blockSpinZ(ui->spinBoxPositionZ);

        // these could emit QSpinBox::valueChanged signals
        ui->spinBoxPositionX->setRange(XMin, XMax);
        ui->spinBoxPositionY->setRange(YMin, YMax);
        ui->spinBoxPositionZ->setRange(ZMin, ZMax);

        ui->checkBoxActivateX->setChecked(isActiveX);
        ui->checkBoxActivateY->setChecked(isActiveY);
        ui->checkBoxActivateZ->setChecked(isActiveZ);

        // these could emit QSpinBox::valueChanged signals
        ui->spinBoxPositionX->setValue(positionX);
        ui->spinBoxPositionY->setValue(positionY);
        ui->spinBoxPositionZ->setValue(positionZ);

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
    }
    return true;
}

bool GridDialog::updateDataShell(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        assert(red == ui->sliderRed->value());
        assert(green == ui->sliderGreen->value());
        assert(blue == ui->sliderBlue->value());
        assert(opacity == ui->sliderOpacity->value());
    }
    else
    {
        ui->sliderRed->setValue(red);
        ui->sliderGreen->setValue(green);
        ui->sliderBlue->setValue(blue);
        ui->sliderOpacity->setValue(opacity);
    }
    return true;
}

bool GridDialog::updateDataSubgrid(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        assert(isSubgridActivated == ui->checkBoxSubgrid->isChecked());

        if (doc->gridType() == GridType::MV_STRUCTURED_GRID)
        {
            assert(col_min == ui->spinBoxLowI->value());
            assert(col_max == ui->spinBoxHighI->value());
            assert(row_min == ui->spinBoxLowJ->value());
            assert(row_max == ui->spinBoxHighJ->value());
        }
        if (doc->gridType() == GridType::MV_STRUCTURED_GRID || doc->gridType() == GridType::MV_LAYERED_GRID)
        {
            assert(lay_min == ui->spinBoxLowK->value());
            assert(lay_max == ui->spinBoxHighK->value());
        }
    }
    else
    {
        ui->checkBoxSubgrid->setChecked(isSubgridActivated);

        if (doc->gridType() == GridType::MV_STRUCTURED_GRID)
        {
            {
                QSignalBlocker blockLow(ui->spinBoxLowI);
                QSignalBlocker blockHigh(ui->spinBoxHighI);
                ui->spinBoxLowI->setRange(1, col_upper_limit);
                ui->spinBoxHighI->setRange(1, col_upper_limit);
            }

            {
                QSignalBlocker blockLow(ui->spinBoxLowJ);
                QSignalBlocker blockHigh(ui->spinBoxHighJ);
                ui->spinBoxLowJ->setRange(1, row_upper_limit);
                ui->spinBoxHighJ->setRange(1, row_upper_limit);
            }

            // Why not still block here?
            ui->spinBoxLowI->setValue(col_min);
            ui->spinBoxHighI->setValue(col_max);
            ui->spinBoxLowJ->setValue(row_min);
            ui->spinBoxHighJ->setValue(row_max);
        }
        if (doc->gridType() == GridType::MV_STRUCTURED_GRID || doc->gridType() == GridType::MV_LAYERED_GRID)
        {
            {
                QSignalBlocker blockLow(ui->spinBoxLowK);
                QSignalBlocker blockHigh(ui->spinBoxHighK);
                ui->spinBoxLowK->setRange(1, lay_upper_limit);
                ui->spinBoxHighK->setRange(1, lay_upper_limit);
            }

            ui->spinBoxLowK->setValue(lay_min);
            ui->spinBoxHighK->setValue(lay_max);
        }
    }
    return true;
}

void GridDialog::reinitialize()
{
    // Lines
    structuredGrid = true;
    XMax           = 0;
    YMax           = 0;
    ZMax           = 0;
    positionX      = 0;
    positionY      = 0;
    positionZ      = 0;

    ui->checkBoxActivateX->setChecked(false);
    ui->checkBoxActivateY->setChecked(false);
    ui->checkBoxActivateZ->setChecked(false);

    colorOption = 0;

    // TODO may need to block signals here
    // or possibly in the updateData routines

    updateDataLines(false);
    activateLines(false);


    // Shell
    red     = 100;
    green   = 80;
    blue    = 60;
    opacity = 20;
    updateDataShell(false);
    updateLabelsShell();
    activateShell(false);


    // Subgrid
    isSubgridActivated = false;
    col_min = 1;
    col_max = 1;
    row_min = 1;
    row_max = 1;
    lay_min = 1;
    lay_max = 1;
    col_upper_limit = 1;
    row_upper_limit = 1;
    lay_upper_limit = 1;

    updateDataSubgrid(false);
    activateSubgrid(false);
}

void GridDialog::activate(bool b)
{
    // Lines
    activateLines(b);

    // Shell
    activateShell(b);

    // Subgrid
    activateSubgrid(b);
}

int GridDialog::currentTabIndex() const
{
    return ui->tabWidget->currentIndex();
}

void GridDialog::enableApplyButton(bool enable)
{
    ui->pushButtonApply->setEnabled(enable);
}

void GridDialog::activateLines(bool activate)
{
    bool bx = ui->checkBoxActivateX->isChecked();
    bool by = ui->checkBoxActivateY->isChecked();
    bool bz = ui->checkBoxActivateZ->isChecked();

    ui->spinBoxPositionX->setEnabled(activate && structuredGrid && bx);
    ui->spinBoxPositionY->setEnabled(activate && structuredGrid && by);
    ui->spinBoxPositionZ->setEnabled(activate && bz);

    bool activateZ = structuredGrid;
    ui->checkBoxActivateX->setEnabled(activate && structuredGrid);
    ui->checkBoxActivateY->setEnabled(activate && structuredGrid);
    ui->checkBoxActivateZ->setEnabled(activate && activateZ);

    ui->radioButtonBlack->setEnabled(activate);
    ui->radioButtonGray->setEnabled(activate);
    ui->radioButtonWhite->setEnabled(activate);
    
    isActive = activate;
    if (ui->tabWidget->currentIndex() == 0)
    {
        ui->pushButtonApply->setEnabled(activate);
    }
}

void GridDialog::activateShell(bool b)
{
    ui->labelRed->setEnabled(b);
    ui->labelGreen->setEnabled(b);
    ui->labelBlue->setEnabled(b);
    ui->labelOpacity->setEnabled(b);

    ui->sliderRed->setEnabled(b);
    ui->sliderGreen->setEnabled(b);
    ui->sliderBlue->setEnabled(b);
    ui->sliderOpacity->setEnabled(b);
}

void GridDialog::activateSubgrid(bool b)
{
    ui->checkBoxSubgrid->setEnabled(b && doc->gridType() != GridType::MV_UNSTRUCTURED_GRID);
    bool bb  = ui->checkBoxSubgrid->isChecked();
    bool bbb = b && bb && doc->gridType() == GridType::MV_STRUCTURED_GRID;

    ui->spinBoxLowI->setEnabled(bbb);
    ui->spinBoxHighI->setEnabled(bbb);
    ui->spinBoxLowJ->setEnabled(bbb);
    ui->spinBoxHighJ->setEnabled(bbb);

    bbb = b && bb && doc->gridType() != GridType::MV_UNSTRUCTURED_GRID;
    ui->spinBoxLowK->setEnabled(bbb);
    ui->spinBoxHighK->setEnabled(bbb);

    ui->pushButtonApply->setEnabled(b && bb);
}

void GridDialog::onActivateX()
{
    bool b = ui->checkBoxActivateX->isChecked();
    ui->spinBoxPositionX->setEnabled(b);
    doc->activateGridLines(0, b);
}

void GridDialog::onActivateY()
{
    bool b = ui->checkBoxActivateY->isChecked();
    ui->spinBoxPositionY->setEnabled(b);
    doc->activateGridLines(1, b);
}

void GridDialog::onActivateZ()
{
    bool b = ui->checkBoxActivateZ->isChecked();
    ui->spinBoxPositionZ->setEnabled(b);
    doc->activateGridLines(2, b);
}

void GridDialog::onCheckBoxSubgrid()
{
    isSubgridActivated = ui->checkBoxSubgrid->isChecked();

    bool b             = ui->checkBoxSubgrid->isChecked();    
    bool bb = b && doc->gridType() == GridType::MV_STRUCTURED_GRID;

    ui->spinBoxLowI->setEnabled(bb);
    ui->spinBoxHighI->setEnabled(bb);
    ui->spinBoxLowJ->setEnabled(bb);
    ui->spinBoxHighJ->setEnabled(bb);

    if (doc->gridType() == GridType::MV_STRUCTURED_GRID || doc->gridType() == GridType::MV_LAYERED_GRID)
    {
        ui->spinBoxLowK->setEnabled(b);
        ui->spinBoxHighK->setEnabled(b);
    }
    ui->pushButtonApply->setEnabled(b);

    if (b)
    {
        applySubgrid();
    }
    else
    {
        doc->subgridOff();
    }
}

void GridDialog::updateLabelsShell()
{
    ui->labelRed->setText(QString("%1").arg(red / 100.0, 0, 'f', 2));
    ui->labelGreen->setText(QString("%1").arg(green / 100.0, 0, 'f', 2));
    ui->labelBlue->setText(QString("%1").arg(blue / 100.0, 0, 'f', 2));
    ui->labelOpacity->setText(QString("%1").arg(opacity / 100.0, 0, 'f', 2));
}

void GridDialog::applySubgrid()
{
    if (updateDataSubgrid(true) && isSubgridActivated)
    {
        if (doc->gridType() == GridType::MV_STRUCTURED_GRID)
        {
            if (col_min < 1)
            {
                col_min = 1;
            }
            if (col_max > col_upper_limit)
            {
                col_max = col_upper_limit;
            }
            if (row_min < 1)
            {
                row_min = 1;
            }
            if (row_max > row_upper_limit)
            {
                row_max = row_upper_limit;
            }
        }
        if (doc->gridType() == GridType::MV_STRUCTURED_GRID || doc->gridType() == GridType::MV_LAYERED_GRID)
        {
            if (lay_min < 1)
            {
                lay_min = 1;
            }
            if (lay_max > lay_upper_limit)
            {
                lay_max = lay_upper_limit;
            }
        }
        updateDataSubgrid(false);
        doc->applySubgrid(col_min, col_max, row_min, row_max, lay_min, lay_max);
    }
}

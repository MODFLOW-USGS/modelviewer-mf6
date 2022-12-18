#include "overlaydialog.h"
#include "ui_overlaydialog.h"

#include "mvdoc.h"
#include "mvDefine.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

#include <limits>

OverlayDialog::OverlayDialog(QWidget* parent, MvDoc* doc)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::OverlayDialog)
    , doc(doc)
    , _geometry{nullptr}
{
    ui->setupUi(this);

    // Controls
    QDoubleValidator* doubleValidator = new QDoubleValidator(this);
    ui->lineEditXOrig->setValidator(doubleValidator);
    ui->lineEditYOrig->setValidator(doubleValidator);
    ui->lineEditScale->setValidator(doubleValidator);
    ui->lineEditAngle->setValidator(doubleValidator);
    ui->lineEditElev->setValidator(doubleValidator);
    ui->lineEditDrapeGap->setValidator(doubleValidator);

    //connect(ui->radioButtonFlat, &QAbstractButton::pressed, this, &OverlayDialog::onFlat);
    //connect(ui->radioButtonDrape, &QAbstractButton::pressed, this, &OverlayDialog::onDrape);

    // QAbstractButton::clicked(bool checked = false)       // left-mouse-button-down and left-mouse-button-up                  // user action or click()/animateClick is called NOT setChecked
    // QAbstractButton::pressed()                           // left-mouse-button-down
    // QAbstractButton::released()                          // left-mouse-button-up
    // QAbstractButton::toggled(bool checked)               // changed state                                                    // user action or (called click() or setChecked())

    // the following slots don't get called the QCheckBox::setChecked(bool) is called
    connect(ui->radioButtonFlat, &QAbstractButton::clicked, this, &OverlayDialog::onFlat);
    connect(ui->radioButtonDrape, &QAbstractButton::clicked, this, &OverlayDialog::onDrape);


    //connect(ui->checkBoxCrop, QOverload<int>::of(&QCheckBox::stateChanged), this, &OverlayDialog::onCrop);
    //connect(ui->checkBoxTrim, QOverload<int>::of(&QCheckBox::stateChanged), this, &OverlayDialog::onTrim);

    //connect(ui->checkBoxCrop, &QAbstractButton::clicked, this, &OverlayDialog::onCrop);
    //connect(ui->checkBoxTrim, &QAbstractButton::clicked, this, &OverlayDialog::onTrim);

    // the following slots don't get called the QCheckBox::setChecked(bool) is called
    connect(ui->checkBoxCrop, QOverload<bool>::of(&QCheckBox::clicked), this, &OverlayDialog::onCrop);
    connect(ui->checkBoxTrim, QOverload<bool>::of(&QCheckBox::clicked), this, &OverlayDialog::onTrim);

    // File
    ui->comboBoxOverlayType->addItem(tr("ESRI Shapefile"));
    ui->comboBoxOverlayType->addItem(tr("DXF File"));
    ui->comboBoxOverlayType->setCurrentIndex(0);

    connect(ui->pushButtonBrowse, &QAbstractButton::clicked, this, &OverlayDialog::onBrowse);

    // buttons
    connect(ui->pushButtonApply, &QAbstractButton::clicked, this, &OverlayDialog::onApply);
    connect(ui->pushButtonDone, &QAbstractButton::clicked, [=]() { hide(); });
    connect(ui->pushButtonRemove, &QAbstractButton::clicked, this, &OverlayDialog::onRemove);
}

OverlayDialog::~OverlayDialog()
{
    delete ui;
    delete _geometry;
}

void OverlayDialog::hideEvent(QHideEvent* e)
{
    delete _geometry;
    _geometry = new QRect(geometry());
}

void OverlayDialog::showEvent(QShowEvent* e)
{
    if (_geometry)
    {
        setGeometry(*_geometry);
    }
}

void OverlayDialog::reinitialize()
{
    // Controls
    reinitializeControls();

    // File
    reinitializeFile();

    // Bounds
    reinitializeBounds();

    ui->pushButtonApply->setEnabled(false);
    ui->pushButtonRemove->setEnabled(false);
}

void OverlayDialog::reinitializeControls()
{
    drape    = false;
    trim     = false;
    crop     = false;
    angle    = 0.0;
    xOrig    = 0.0;
    yOrig    = 0.0;
    elev     = 0.0;
    scale    = 1.0;
    drapeGap = 0;
    updateDataControls(false);
    activateControls(false);
}

void OverlayDialog::reinitializeFile()
{
    ui->lineEditFilename->setText("");
    ui->comboBoxOverlayType->setCurrentIndex(0);
    activateFile(false);
}

void OverlayDialog::reinitializeBounds()
{
    ui->lineEditXMin->setText("");
    ui->lineEditXMax->setText("");
    ui->lineEditYMin->setText("");
    ui->lineEditYMax->setText("");
}

void OverlayDialog::activate(bool b)
{
    activateControls(b);
    activateFile(b);
    ui->pushButtonApply->setEnabled(b);
}

void OverlayDialog::activateControls(bool b)
{
    bool flat = ui->radioButtonFlat->isChecked();

    ui->radioButtonFlat->setEnabled(b);
    ui->radioButtonDrape->setEnabled(b && structuredGrid);
    ui->checkBoxTrim->setEnabled(b && flat);
    ui->checkBoxCrop->setEnabled(b);
    ui->lineEditAngle->setEnabled(b);
    ui->lineEditXOrig->setEnabled(b);
    ui->lineEditYOrig->setEnabled(b);
    ui->lineEditElev->setEnabled(b && flat);
    ui->lineEditScale->setEnabled(b);
    ui->lineEditDrapeGap->setEnabled(b && !flat);
}

void OverlayDialog::activateFile(bool b)
{
    ui->pushButtonBrowse->setEnabled(b);
    ui->lineEditFilename->setEnabled(b);
    ui->comboBoxOverlayType->setEnabled(b);
}

bool OverlayDialog::updateDataControls(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        QString s;
        double  val;
        bool    ok;

        // xOrig
        s   = ui->lineEditXOrig->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditXOrig->setFocus();
            return false;
        }
        xOrig = val;

        // yOrig
        s     = ui->lineEditYOrig->text();
        val   = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditYOrig->setFocus();
            return false;
        }
        yOrig = val;

        // scale
        s     = ui->lineEditScale->text();
        val   = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditScale->setFocus();
            return false;
        }
        scale = val;

        // elev
        s     = ui->lineEditElev->text();
        val   = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditElev->setFocus();
            return false;
        }
        elev = val;

        // drape
        assert(drape == ui->radioButtonDrape->isChecked());

        // drapeGap
        s    = ui->lineEditDrapeGap->text();
        val  = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditDrapeGap->setFocus();
            return false;
        }
        drapeGap = val;

        // trim
        assert(trim == ui->checkBoxTrim->isChecked());

        // angle
        s   = ui->lineEditAngle->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditAngle->setFocus();
            return false;
        }
        angle = val;

        // crop
        assert(crop == ui->checkBoxCrop->isChecked());
    }
    else
    {
        if (drape)
        {
            ui->radioButtonDrape->setChecked(true);
        }
        else
        {
            ui->radioButtonFlat->setChecked(true);
        }
        ui->lineEditXOrig->setText(QString("%1").number(xOrig, 'g', std::numeric_limits<double>::digits10));
        ui->lineEditYOrig->setText(QString("%1").number(yOrig, 'g', std::numeric_limits<double>::digits10));
        ui->lineEditScale->setText(QString("%1").arg(scale));
        ui->lineEditElev->setText(QString("%1").arg(elev));
        ui->lineEditDrapeGap->setText(QString("%1").arg(drapeGap));
        ui->checkBoxTrim->setChecked(trim);
        ui->lineEditAngle->setText(QString("%1").arg(angle));
        ui->checkBoxCrop->setChecked(crop);
    }
    return true;
}

bool OverlayDialog::updateDataFile(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        // no-op see onApply
    }
    else
    {
        ui->lineEditFilename->setText(filename);
        switch (overlayType)
        {
        case OverlayType::otShapeFile:
            ui->comboBoxOverlayType->setCurrentIndex(0);
            break;
        case OverlayType::otDXFFile:
            ui->comboBoxOverlayType->setCurrentIndex(1);
            break;
        default:
            assert(false);
            break;
        }
    }
    return true;
}

void OverlayDialog::onFlat()
{
    drape = ui->radioButtonDrape->isChecked();
    qDebug() << "OverlayDialog::onFlat drape " << drape;
    ui->checkBoxTrim->setEnabled(true);
    ui->lineEditElev->setEnabled(true);
    ui->lineEditDrapeGap->setEnabled(false);
    if (doc->hasOverlay())
    {
        onApply();
    }
}

void OverlayDialog::onDrape()
{
    drape = ui->radioButtonDrape->isChecked();
////#if !defined(NDEBUG)
    qDebug() << "OverlayDialog::onDrape drape " << drape;
////#endif
    ui->checkBoxTrim->setEnabled(false);
    ui->lineEditElev->setEnabled(false);
    ui->lineEditDrapeGap->setEnabled(true);
    ui->checkBoxTrim->setChecked(true);
    trim = true;
    if (doc->hasOverlay())
    {
        onApply();
    }
}

//void OverlayDialog::onTrim()
//{
//    trim = ui->checkBoxTrim->isChecked();
//    if (doc->hasOverlay())
//    {
//        onApply();
//    }
//}
//
//void OverlayDialog::onCrop()
//{
//    crop = ui->checkBoxCrop->isChecked();
//    if (doc->hasOverlay())
//    {
//        onApply();
//    }
//}

void OverlayDialog::onTrim(bool checked)
{
    //trim = ui->checkBoxTrim->isChecked();
    trim = checked;
    assert(checked == ui->checkBoxTrim->isChecked());
////#if !defined(NDEBUG)
    qDebug() << "onTrim trim " << trim;
////#endif
    if (doc->hasOverlay())
    {
        onApply();
    }
}

void OverlayDialog::onCrop(bool checked)
{
    //crop = ui->checkBoxCrop->isChecked();
    crop = checked;
    assert(checked == ui->checkBoxCrop->isChecked());
////#if !defined(NDEBUG)
    qDebug() << "onCrop crop " << crop;
////#endif
    if (doc->hasOverlay())
    {
        onApply();
    }
}

void OverlayDialog::onApply()
{
    updateDataFile(true);

    QString fn = ui->lineEditFilename->text().trimmed();
    if (fn.isEmpty())
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->lineEditFilename->setFocus();
        QMessageBox::warning(this, QString(), tr("Please specify the overlay file."));
        return;
    }
    // Test open the overlay file
    std::ifstream in(fn.toLocal8Bit().data(), std::ifstream::in);
    if (!in.is_open())
    {
        ui->tabWidget->setCurrentIndex(1);
        ui->lineEditFilename->setFocus();
        QMessageBox::warning(this, QString(), tr("Unable to open the overlay file"));
        return;
    }
    in.close();

    updateDataControls(true);
    int type = ui->comboBoxOverlayType->currentIndex() + 1;

///#if !defined(NDEBUG)
    qDebug() << "fn " << fn;
    qDebug() << "type " << type;
    qDebug() << "xOrig " << xOrig;
    qDebug() << "yOrig " << yOrig;
    qDebug() << "scale " << scale;
    qDebug() << "angle " << angle;
    qDebug() << "drape " << drape;
    qDebug() << "trim " << trim;
    qDebug() << "crop " << crop;
    qDebug() << "elev " << elev;
    qDebug() << "drapeGap " << drapeGap;
///#endif

    doc->applyOverlayControl(fn.toLocal8Bit().data(), type, xOrig, yOrig,
                             scale, angle, drape,
                             trim, crop, elev, drapeGap);
}

void OverlayDialog::onRemove()
{
    QMessageBox::StandardButton ret = QMessageBox::question(this, QString(), tr("Do you want to remove the overlay?"));
    if (ret == QMessageBox::No)
    {
        return;
    }
    doc->removeOverlay();
}

void OverlayDialog::onBrowse()
{
    QString initialFile = ui->lineEditFilename->text();
    initialFile = initialFile.trimmed();

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    QString(),
                                                    initialFile,
                                                    tr("All Files (*.*);;ESRI Shapefile Files (*.shp);;DXF Files (*.dxf)"));
    if (fileName.isEmpty())
    {
        return;
    }
    ui->lineEditFilename->setText(QDir::toNativeSeparators(fileName));

    if (fileName.right(4).toLower() == ".shp")
    {
        ui->comboBoxOverlayType->setCurrentIndex(0);
    }
    else if (fileName.right(4).toLower() == ".dxf")
    {
        ui->comboBoxOverlayType->setCurrentIndex(1);
    }
}

void OverlayDialog::enableRemoveButton(bool enable)
{
    ui->pushButtonRemove->setEnabled(enable);
}

void OverlayDialog::enableApplyButton(bool enable)
{
    ui->pushButtonApply->setEnabled(enable);
}

void OverlayDialog::setBounds(double xmin, double xmax, double ymin, double ymax)
{
    ui->lineEditXMin->setText(QString("%1").number(xmin));
    ui->lineEditXMax->setText(QString("%1").number(xmax));
    ui->lineEditYMin->setText(QString("%1").number(ymin));
    ui->lineEditYMax->setText(QString("%1").number(ymax));
}

void OverlayDialog::setCurrentIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

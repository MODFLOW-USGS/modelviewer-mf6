#include "isosurfacedialog.h"
#include "ui_isosurfacedialog.h"

#include "mvdoc.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QSignalBlocker>


IsosurfaceDialog::IsosurfaceDialog(QWidget* parent, MvDoc* doc)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::IsosurfaceDialog)
    , doc(doc)
    , _geometry{nullptr}
{
    ui->setupUi(this);
    resize(minimumSizeHint());

    QDoubleValidator* doubleValidator = new QDoubleValidator(this);

    // Regular
    isRegularActive = false;
    ui->lineEditMin->setValidator(doubleValidator);
    ui->lineEditMax->setValidator(doubleValidator);

    ui->spinBoxSurfaces->setRange(0, VTK_INT_MAX);

    connect(ui->pushButtonFull, &QAbstractButton::clicked, this, &IsosurfaceDialog::onFull);
    connect(ui->spinBoxSurfaces, QOverload<int>::of(&QSpinBox::valueChanged),
            [=](int val) {
                mIsosurfaceCount = val;
                doc->setRegularIsosurfaces(mIsosurfaceCount, mValueMin, mValueMax);
            });

    // Custom
    isCustomActive  = false;
    ui->lineEditValue->setValidator(doubleValidator);

    connect(ui->lineEditValue, &QLineEdit::returnPressed, this, &IsosurfaceDialog::onAdd);
    connect(ui->pushButtonAdd, &QAbstractButton::clicked, this, &IsosurfaceDialog::onAdd);
    connect(ui->pushButtonDelete, &QAbstractButton::clicked, this, &IsosurfaceDialog::onDelete);

    connect(ui->listWidget, QOverload<QListWidgetItem*>::of(&QListWidget::itemDoubleClicked), this, &IsosurfaceDialog::itemDoubleClicked);
    connect(ui->listWidget, QOverload<QListWidgetItem*, QListWidgetItem*>::of(&QListWidget::currentItemChanged), this, &IsosurfaceDialog::currentItemChanged);

    // tab
    connect(ui->tabWidget, QOverload<int>::of(&QTabWidget::currentChanged), this, &IsosurfaceDialog::onTabChanged);

    // buttons
    connect(ui->pushButtonApply, &QAbstractButton::clicked, this, &IsosurfaceDialog::onApply);
    connect(ui->pushButtonDone, &QAbstractButton::clicked, [=]() { hide(); });
}

IsosurfaceDialog::~IsosurfaceDialog()
{
    delete ui;
    delete _geometry;
}

void IsosurfaceDialog::hideEvent(QHideEvent* e)
{
    delete _geometry;
    _geometry = new QRect(geometry());
}

void IsosurfaceDialog::showEvent(QShowEvent* e)
{
    if (_geometry)
    {
        setGeometry(*_geometry);
    }
}

bool IsosurfaceDialog::updateDataRegular(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        QString s;
        double  val;
        bool    ok;

        // mValueMin
        s   = ui->lineEditMin->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditMin->setFocus();
            return false;
        }
        mValueMin          = val;

        // mValueMax
        s                  = ui->lineEditMax->text();
        val                = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditMax->setFocus();
            return false;
        }
        mValueMax = val;

        mIsosurfaceCount = ui->spinBoxSurfaces->value();
    }
    else
    {
        ui->spinBoxSurfaces->setValue(mIsosurfaceCount);

        ui->lineEditMin->setText(QString("%1").number(mValueMin, 'g', std::numeric_limits<double>::digits10));
        ui->lineEditMax->setText(QString("%1").number(mValueMax, 'g', std::numeric_limits<double>::digits10));
    }
    return true;
}

bool IsosurfaceDialog::updateDataCustom(bool saveAndValidate)
{
    if (saveAndValidate)
    {
        QString s;
        double  val;
        bool    ok;

        // mValue
        s   = ui->lineEditValue->text();
        val = s.toDouble(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "", tr("Enter a number."));
            ui->lineEditValue->setFocus();
            return false;
        }
        mValueAsString = s;
        mValue = val;

        /*
        mCustomIsosurfaces.clear();
        for (int i = 0; i < ui->listWidget->count(); ++i)
        {
            QListWidgetItem *item =  ui->listWidget->item(i);
            mCustomIsosurfaces.push_back(ui->listWidget->item(i)->data(Qt::UserRole).toDouble());
        }
        */
    }
    else
    {
        ui->listWidget->clear();
        for (auto value : mCustomIsosurfaces)
        {
            QListWidgetItem *item = new QListWidgetItem(QString("%1").number(value, 'g', std::numeric_limits<double>::digits10), ui->listWidget);
            item->setData(Qt::UserRole, value);
        }
    }
    return true;
}

void IsosurfaceDialog::activate(bool b)
{
    // Regular
    activateRegular(b);

    // Custom
    activateCustom(b);

    ui->pushButtonApply->setEnabled(b);
}


void IsosurfaceDialog::reinitialize()
{
    // Regular
    reinitializeRegular();

    // Custom
    reinitializeCustom();

    ui->pushButtonApply->setEnabled(false);
}

void IsosurfaceDialog::reinitializeRegular()
{
    ui->lineEditMin->setText("");
    ui->lineEditMax->setText("");
    QSignalBlocker blocker(ui->spinBoxSurfaces);
    ui->spinBoxSurfaces->setValue(5);
    activateRegular(false);
}

void IsosurfaceDialog::reinitializeCustom()
{
    ui->listWidget->clear();
    ui->lineEditValue->setText("");
    activateCustom(false);
}

void IsosurfaceDialog::activateRegular(bool b)
{
    ui->lineEditMin->setEnabled(b);
    ui->lineEditMax->setEnabled(b);
    ui->spinBoxSurfaces->setEnabled(b);
    ui->pushButtonFull->setEnabled(b);
    isRegularActive = b;
}

void IsosurfaceDialog::activateCustom(bool b)
{
    ui->lineEditValue->setEnabled(b);
    ui->pushButtonAdd->setEnabled(b);
    ui->pushButtonDelete->setEnabled(b);
    ui->listWidget->setEnabled(b);
    isCustomActive = b;
}

void IsosurfaceDialog::onApply()
{
    int idx = ui->tabWidget->currentIndex();
    switch (ui->tabWidget->currentIndex())
    {
    case 0:
        applyRegular();
        break;
    case 1:
        applyCustom();
        break;
    default:
        assert(false);
        break;
    }
}

void IsosurfaceDialog::applyRegular()
{
    qDebug() << "applyRegular";
    if (updateDataRegular(true))
    {
        doc->setRegularIsosurfaces(mIsosurfaceCount, mValueMin, mValueMax);
    }
}

void IsosurfaceDialog::applyCustom()
{
    qDebug() << "applyCustom";
    int count = ui->listWidget->count();
    if (count > 0)
    {
        mCustomIsosurfaces.clear();
        for (int i = 0; i < count; ++i)
        {
            QListWidgetItem* item = ui->listWidget->item(i);
            mCustomIsosurfaces.push_back(ui->listWidget->item(i)->data(Qt::UserRole).toDouble());
        }
        doc->setCustomIsosurfaces(mCustomIsosurfaces);
    }
    else
    {
        QMessageBox::warning(this, "", tr("You must specify at least one isosurface value."));
    }
}

void IsosurfaceDialog::onFull()
{
    const QMessageBox::StandardButton ret = QMessageBox::question(this,
                                                                  "",
                                                                  tr("Do you want to set the max and min values to the full range of the data?"),
                                                                  QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::No)
    {
        return;
    }
    double range[2];
    doc->getScalarDataRange(range);
    mValueMin = range[0];
    mValueMax = range[1];
    updateDataRegular(false);
    applyRegular();
}

void IsosurfaceDialog::setCurrentTabIndex(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

void IsosurfaceDialog::onTabChanged(int idx)
{
    switch (idx)
    {
    case 0:
        if (isRegularActive)
        {
            applyRegular();
        }
        break;
    case 1:
        if (isCustomActive)
        {
            applyCustom();
        }
        break;
    default:
        assert(false);
        break;
    }
}

void IsosurfaceDialog::itemDoubleClicked(QListWidgetItem* item)
{
    qDebug() << "itemDoubleClicked";
    onDelete();
}

void IsosurfaceDialog::currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    qDebug() << "currentItemChanged";
    ui->pushButtonDelete->setEnabled(current != nullptr);
}

void IsosurfaceDialog::onAdd()
{
    if (updateDataCustom(true))
    {
        int count = ui->listWidget->count();
        if (count == 0)
        {
            QListWidgetItem* item = new QListWidgetItem(QString("%1").number(mValue, 'g', std::numeric_limits<double>::digits10));
            item->setData(Qt::UserRole, mValue);
            ui->listWidget->addItem(item);
        }
        else
        {
            int  i        = 0;
            bool exceeded = false;
            while (i < count && !exceeded)
            {
                QListWidgetItem* item = ui->listWidget->item(i);
                if (item->data(Qt::UserRole).toDouble() == mValue)
                {
                    QMessageBox::warning(this, "", tr("This value already exists."));
                    return;
                }
                if (item->data(Qt::UserRole).toDouble() > mValue)
                {
                    exceeded = true;
                }
                else
                {
                    i++;
                }
            }
            QListWidgetItem* item = new QListWidgetItem(QString("%1").number(mValue, 'g', std::numeric_limits<double>::digits10));
            item->setData(Qt::UserRole, mValue);
            ui->listWidget->insertItem(i, item);
            item->setSelected(true);
        }
        applyCustom();
        ui->lineEditValue->setText("");
    }
}

void IsosurfaceDialog::onDelete()
{
    if (QListWidgetItem *item = ui->listWidget->currentItem())
    {
        ui->lineEditValue->setText(item->text());
        delete item;
        applyCustom();
    }
}

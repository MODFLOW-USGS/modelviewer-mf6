#include "modelfeaturesdialog.h"
#include "ui_modelfeaturesdialog.h"

#include "mvdoc.h"

#include <QAbstractItemModel>
#include <QColorDialog>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QStringListModel>

#include <algorithm>

class ColorListModel : public QStringListModel
{
public:
    explicit ColorListModel(QObject *parent, MvDoc *doc);
    explicit ColorListModel(const QStringList &strings, QObject *parent, MvDoc *doc);

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant      data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    MvDoc *doc;
};

ColorListModel::ColorListModel(QObject *parent, MvDoc *doc)
    : QStringListModel(parent)
    , doc(doc)
{
    assert(doc);
}

ColorListModel::ColorListModel(const QStringList &strings, QObject *parent, MvDoc *doc)
    : QStringListModel(strings, parent)
    , doc(doc)
{
}

Qt::ItemFlags ColorListModel::flags(const QModelIndex &index) const
{
    assert(index.isValid());
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant ColorListModel::data(const QModelIndex &index, int role) const
{
    assert(index.isValid());
    switch (role)
    {
    case Qt::DisplayRole:
        return QStringListModel::data(index).toString();
        break;

    case Qt::DecorationRole:
        // pad string to 40 chars for comparison
        double      rgba[4];
        QString     s  = QStringListModel::data(index).toString();
        std::string ss = s.toStdString().append(40ul - static_cast<size_t>(s.size()), ' ');
        doc->modelFeatureColor(ss.c_str(), rgba);
        return QColor(rgba[0] * 255, rgba[1] * 255, rgba[2] * 255);
        break;
    }
    return QVariant();
}

ModelFeaturesDialog::ModelFeaturesDialog(QWidget *parent, MvDoc *doc)
    : QDialog(parent, Qt::WindowCloseButtonHint)
    , ui(new Ui::ModelFeaturesDialog)
    , doc(doc)
    , _geometry(nullptr)
{
    ui->setupUi(this);

    listModelHide = new ColorListModel(this, doc);
    listModelShow = new ColorListModel(this, doc);

    ui->listViewHide->setModel(listModelHide);
    ui->listViewShow->setModel(listModelShow);

    // listViewHide
    //
    connect(ui->listViewHide, QOverload<const QModelIndex &>::of(&QAbstractItemView::activated),
            [=](const QModelIndex &idx) {
                qDebug() << "listViewHide -> QAbstractItemView::activated" << idx.isValid();
            });
    connect(ui->listViewHide, QOverload<const QModelIndex &>::of(&QAbstractItemView::clicked),
            [=](const QModelIndex &idx) {
                qDebug() << "listViewHide -> QAbstractItemView::clicked";
                ui->listViewShow->setCurrentIndex(QModelIndex());                        // Clears selection and currentIndex emits currentChanged

                assert(idx.isValid());
                assert(idx.model() == ui->listViewHide->model());
                ui->listViewHide->setCurrentIndex(idx);

                ui->pushButtonShow->setEnabled(true);
                ui->pushButtonHide->setEnabled(false);
            });
    connect(ui->listViewHide, QOverload<const QModelIndex &>::of(&QAbstractItemView::doubleClicked),
            [=](const QModelIndex &idx) {
                qDebug() << "listViewHide -> QAbstractItemView::doubleClicked";
                assert(idx.isValid());
                onShowFeature();
            });
    connect(ui->listViewHide->selectionModel(), QOverload<const QModelIndex &, const QModelIndex &>::of(&QItemSelectionModel::currentChanged),
            [=](const QModelIndex &current, const QModelIndex &previous) {
                qDebug() << "listViewHide -> QItemSelectionModel::currentChanged";
                onSelchangeHideList(current, previous);
            });

    // listViewShow
    //
    connect(ui->listViewShow, QOverload<const QModelIndex &>::of(&QAbstractItemView::activated),
            [=](const QModelIndex &idx) {
                qDebug() << "listViewShow -> QAbstractItemView::activated" << idx.isValid();
            });
    connect(ui->listViewShow, QOverload<const QModelIndex &>::of(&QAbstractItemView::clicked),
            [=](const QModelIndex &idx) {
                qDebug() << "listViewShow -> QAbstractItemView::clicked";

                ui->listViewHide->setCurrentIndex(QModelIndex());

                assert(idx.isValid());
                assert(idx.model() == ui->listViewShow->model());
                ui->listViewShow->setCurrentIndex(idx);

                ui->pushButtonShow->setEnabled(false);
                ui->pushButtonHide->setEnabled(true);
            });
    connect(ui->listViewShow, QOverload<const QModelIndex &>::of(&QAbstractItemView::doubleClicked),
            [=](const QModelIndex &idx) {
                qDebug() << "listViewShow -> QAbstractItemView::doubleClicked";
                assert(idx.isValid());
                onHideFeature();
            });
    connect(ui->listViewShow->selectionModel(), QOverload<const QModelIndex &, const QModelIndex &>::of(&QItemSelectionModel::currentChanged),
            [=](const QModelIndex &current, const QModelIndex &previous) {
                qDebug() << "listViewShow -> QItemSelectionModel::currentChanged";
                onSelchangeShowList(current, previous);
            });

    // hide button
    connect(ui->pushButtonHide, &QAbstractButton::clicked,
            [=]() {
                qDebug() << "pushButtonHide -> QAbstractButton::clicked";
                onHideFeature();
            });

    // show button
    connect(ui->pushButtonShow, &QAbstractButton::clicked,
            [=]() {
                qDebug() << "pushButtonShow -> QAbstractButton::clicked";
                onShowFeature();
            });

    connect(ui->pushButtonTop,    &QAbstractButton::clicked, this, &ModelFeaturesDialog::onMoveToTop);
    connect(ui->pushButtonBottom, &QAbstractButton::clicked, this, &ModelFeaturesDialog::onMoveToBottom);
    connect(ui->pushButtonUp,     &QAbstractButton::clicked, this, &ModelFeaturesDialog::onMoveUp);
    connect(ui->pushButtonDown,   &QAbstractButton::clicked, this, &ModelFeaturesDialog::onMoveDown);
    connect(ui->pushButtonColor,  &QAbstractButton::clicked, this, &ModelFeaturesDialog::onColor);

    // buttons
    ui->pushButtonDone->setDefault(true);
    connect(ui->pushButtonDone, &QAbstractButton::clicked, [=]() { hide(); });
}

ModelFeaturesDialog::~ModelFeaturesDialog()
{
    delete ui;
    delete _geometry;
}

void ModelFeaturesDialog::hideEvent(QHideEvent *e)
{
    delete _geometry;
    _geometry = new QRect(geometry());
}

void ModelFeaturesDialog::showEvent(QShowEvent *e)
{
    if (_geometry)
    {
        setGeometry(*_geometry);
    }
}

void ModelFeaturesDialog::setDisplayOrder(const std::vector<int> &displayOrder)
{
    mDisplayOrder                   = displayOrder;
    std::vector<std::string> labels = doc->modelFeatureLabels();
    assert(mDisplayOrder.size() == labels.size());

    int                      show = std::count_if(mDisplayOrder.begin(), mDisplayOrder.end(), [](int i) { return i >= 0; });
    int                      hide = std::count_if(mDisplayOrder.begin(), mDisplayOrder.end(), [](int i) { return i == -1; });

    std::vector<QString>     hidden;
    std::vector<QString>     showing(show, "missing");

    int                      lidx = 0;
    for (auto index : mDisplayOrder)
    {
        std::string ss = labels[lidx];
        QString     s  = ss.c_str();
        if (index == -1)
        {
            hidden.push_back(s.trimmed());
        }
        else
        {
            showing[index] = s.trimmed();
        }
        ++lidx;
    }

    listModelHide->setStringList(QStringList(hidden.begin(), hidden.end()));
    listModelShow->setStringList(QStringList(showing.begin(), showing.end()));

    ui->listViewHide->setCurrentIndex(QModelIndex());                                    // Clears selection and currentIndex emits currentChanged
    ui->listViewShow->setCurrentIndex(QModelIndex());                                    // Clears selection and currentIndex emits currentChanged

    ui->pushButtonDone->setFocus();
}

void ModelFeaturesDialog::activate(bool b)
{
    ui->listViewShow->setEnabled(b);
    ui->listViewHide->setEnabled(b);

    // no size buttons

    int  showListCount = ui->listViewShow->model()->rowCount();
    int  showSel       = ui->listViewShow->currentIndex().row();
    int  hideSel       = ui->listViewHide->currentIndex().row();

    bool b1            = showListCount > 1 && showSel > 0;
    bool b2            = showListCount > 1 && showSel < showListCount - 1 && showSel > -1;

    ui->pushButtonShow->setEnabled(b && hideSel > -1);
    ui->pushButtonHide->setEnabled(b && showSel > -1);
    ui->pushButtonTop->setEnabled(b && b1);
    ui->pushButtonUp->setEnabled(b && b1);
    ui->pushButtonDown->setEnabled(b && b2);
    ui->pushButtonBottom->setEnabled(b && b2);
    ui->pushButtonColor->setEnabled(b && (showSel > -1 || hideSel > -1));
}

void ModelFeaturesDialog::reinitialize()
{
}

int ModelFeaturesDialog::showFeaturesCount()
{
    return ui->listViewShow->model()->rowCount();
}

void ModelFeaturesDialog::onSelchangeHideList(const QModelIndex &current, const QModelIndex &previous)
{
    if (!current.isValid()) return;

    Q_UNUSED(previous);
    ui->pushButtonShow->setEnabled(true);
    ui->pushButtonHide->setEnabled(false);

    ui->pushButtonTop->setEnabled(false);
    ui->pushButtonBottom->setEnabled(false);
    ui->pushButtonUp->setEnabled(false);
    ui->pushButtonDown->setEnabled(false);

    ui->pushButtonColor->setEnabled(true);
    ui->listViewShow->setCurrentIndex(QModelIndex());                                    // Clears selection and currentIndex emits currentChanged
}

void ModelFeaturesDialog::onSelchangeShowList(const QModelIndex &current, const QModelIndex &previous)
{
    if (!current.isValid()) return;

    Q_UNUSED(previous);
    ui->pushButtonShow->setEnabled(false);
    ui->pushButtonHide->setEnabled(true);
    ui->listViewHide->setCurrentIndex(QModelIndex());                                    // Clears selection and currentIndex emits currentChanged

    int         count = ui->listViewShow->model()->rowCount();
    int         i     = current.row();

    bool b1    = (count > 1 && i > 0);
    bool b2    = (count > 1 && i < count - 1);

    ui->pushButtonTop->setEnabled(b1);
    ui->pushButtonUp->setEnabled(b1);
    ui->pushButtonDown->setEnabled(b2);
    ui->pushButtonBottom->setEnabled(b2);

    ui->pushButtonColor->setEnabled(true);
}

void ModelFeaturesDialog::onShowFeature()
{
    QModelIndex idx = ui->listViewHide->currentIndex();
    if (!idx.isValid()) return;

    const QSignalBlocker listViewHideBlocker(ui->listViewHide->selectionModel());
    const QSignalBlocker listViewShowBlocker(ui->listViewShow->selectionModel());

    int k = idx.row();
    assert(k != -1);

    QMap<int, QVariant> roles;
    roles[Qt::DisplayRole] = ui->listViewHide->model()->data(idx, Qt::DisplayRole);
    ui->listViewHide->model()->removeRow(idx.row());

    bool ok = ui->listViewShow->model()->insertRow(0);
    assert(ok);
    if (ok)
    {
        QModelIndex id = ui->listViewShow->model()->index(0, 0);
        ui->listViewShow->model()->setItemData(id, roles);                               // InsertString
        ui->listViewShow->setCurrentIndex(id);                                           // SetCurSel
        ui->listViewHide->setCurrentIndex(QModelIndex());                                // Clears selection and currentIndex emits currentChanged

        // update display order
        int n = 0;
        for (int i = 0; i < mDisplayOrder.size(); ++i)
        {
            if (mDisplayOrder[i] >= 0)
            {
                ++mDisplayOrder[i];
            }
            else if (mDisplayOrder[i] == -1)
            {
                if (n == k)
                {
                    mDisplayOrder[i] = 0;
                }
                ++n;
            }
        }
        ui->pushButtonTop->setEnabled(false);
        ui->pushButtonUp->setEnabled(false);

        int count = ui->listViewShow->model()->rowCount();
        bool b2    = (count > 1);
        ui->pushButtonDown->setEnabled(b2);
        ui->pushButtonBottom->setEnabled(b2);
        ui->pushButtonShow->setEnabled(false);
        ui->pushButtonHide->setEnabled(true);
        ui->pushButtonColor->setEnabled(true);
        doc->setModelFeatureDisplayOrder(mDisplayOrder);
    }
#if !defined(NDEBUG)
    validateDisplayOrder();
#endif
}

void ModelFeaturesDialog::onHideFeature()
{
    QModelIndex idx = ui->listViewShow->currentIndex();
    if (!idx.isValid()) return;

    const QSignalBlocker listViewHideBlocker(ui->listViewHide->selectionModel());
    const QSignalBlocker listViewShowBlocker(ui->listViewShow->selectionModel());

    int k = idx.row();
    assert(k != -1);

    QMap<int, QVariant> roles;
    roles[Qt::DisplayRole] = ui->listViewShow->model()->data(idx, Qt::DisplayRole);
    ui->listViewShow->model()->removeRow(idx.row());

    int n = 0;
    for (int i = 0; i < mDisplayOrder.size(); ++i)
    {
        if (mDisplayOrder[i] == -1)
        {
            ++n;
        }
        else if (mDisplayOrder[i] == k)
        {
            mDisplayOrder[i] = -1;
            bool ok          = ui->listViewHide->model()->insertRow(n);
            assert(ok);
            if (ok)
            {
                QModelIndex id = ui->listViewHide->model()->index(n, 0);
                ui->listViewHide->model()->setItemData(id, roles);                       // InsertString
                ui->listViewHide->setCurrentIndex(id);                                   // SetCurSel
                ui->listViewShow->setCurrentIndex(QModelIndex());                        // Clears selection and currentIndex emits currentChanged
            }
        }
        else if (mDisplayOrder[i] > k)
        {
            --mDisplayOrder[i];
        }
    }
    ui->pushButtonTop->setEnabled(false);
    ui->pushButtonUp->setEnabled(false);
    ui->pushButtonDown->setEnabled(false);
    ui->pushButtonBottom->setEnabled(false);
    ui->pushButtonShow->setEnabled(true);
    ui->pushButtonHide->setEnabled(false);
    ui->pushButtonColor->setEnabled(true);
    doc->setModelFeatureDisplayOrder(mDisplayOrder);

#if !defined(NDEBUG)
    validateDisplayOrder();
#endif
}

void ModelFeaturesDialog::onMoveToTop()
{
    QModelIndex idx = ui->listViewShow->currentIndex();
    assert(idx.isValid());

    int k = idx.row();
    assert(k != -1);

    QMap<int, QVariant> roles;
    roles[Qt::DisplayRole] = ui->listViewShow->model()->data(idx, Qt::DisplayRole);
    ui->listViewShow->model()->removeRow(idx.row());                                     // DeleteString

    bool ok = ui->listViewShow->model()->insertRow(0);
    assert(ok);
    if (ok)
    {
        QModelIndex id = ui->listViewShow->model()->index(0, 0);
        ui->listViewShow->model()->setItemData(id, roles);                               // InsertString
        ui->listViewShow->setCurrentIndex(id);                                           // SetCurSel

        int n;
        for (int i = 0; i < mDisplayOrder.size(); i++)
        {
            if (mDisplayOrder[i] == k)
            {
                n = i;
            }
            else if (mDisplayOrder[i] >= 0 && mDisplayOrder[i] < k)
            {
                ++mDisplayOrder[i];
            }
        }
        mDisplayOrder[n] = 0;

        ui->pushButtonTop->setEnabled(false);
        ui->pushButtonUp->setEnabled(false);
        ui->pushButtonDown->setEnabled(true);
        ui->pushButtonBottom->setEnabled(true);
        doc->setModelFeatureDisplayOrder(mDisplayOrder);
    }
#if !defined(NDEBUG)
    validateDisplayOrder();
#endif
}

void ModelFeaturesDialog::onMoveToBottom()
{
    int                 lastPos = ui->listViewShow->model()->rowCount() - 1;
    QModelIndex         idx     = ui->listViewShow->currentIndex();
    int                 k       = idx.row();

    QMap<int, QVariant> roles;
    roles[Qt::DisplayRole] = ui->listViewShow->model()->data(idx, Qt::DisplayRole);

    ui->listViewShow->model()->removeRow(idx.row());                                     // DeleteString
    bool ok = ui->listViewShow->model()->insertRow(lastPos);
    assert(ok);
    if (ok)
    {
        QModelIndex id = ui->listViewShow->model()->index(lastPos, 0);
        ui->listViewShow->model()->setItemData(id, roles);                               // InsertString
        ui->listViewShow->setCurrentIndex(id);                                           // SetCurSel

        int n;
        for (int i = 0; i < mDisplayOrder.size(); i++)
        {
            if (mDisplayOrder[i] == k)
            {
                n = i;
            }
            else if (mDisplayOrder[i] > k)
            {
                --mDisplayOrder[i];
            }
        }
        mDisplayOrder[n] = lastPos;

        ui->pushButtonTop->setEnabled(true);
        ui->pushButtonUp->setEnabled(true);
        ui->pushButtonDown->setEnabled(false);
        ui->pushButtonBottom->setEnabled(false);
        doc->setModelFeatureDisplayOrder(mDisplayOrder);
    }
#if !defined(NDEBUG)
    validateDisplayOrder();
#endif
}

void ModelFeaturesDialog::onMoveUp()
{
    QModelIndex idx = ui->listViewShow->currentIndex();
    assert(idx.isValid());

    int k = idx.row();
    assert(k != -1);

    QMap<int, QVariant> roles;
    roles[Qt::DisplayRole] = ui->listViewShow->model()->data(idx, Qt::DisplayRole);

    ui->listViewShow->model()->removeRow(idx.row());                                     // DeleteString

    int  j  = k - 1;
    bool ok = ui->listViewShow->model()->insertRow(j);
    assert(ok);
    if (ok)
    {
        QModelIndex id = ui->listViewShow->model()->index(j, 0);
        ui->listViewShow->model()->setItemData(id, roles);                               // InsertString
        ui->listViewShow->setCurrentIndex(id);                                           // SetCurSel
        int m, n;
        for (int i = 0; i < mDisplayOrder.size(); ++i)
        {
            if (mDisplayOrder[i] == k)
            {
                n = i;
            }
            else if (mDisplayOrder[i] == k - 1)
            {
                m = i;
            }
        }
        mDisplayOrder[n] = k - 1;
        mDisplayOrder[m] = k;

        ui->pushButtonTop->setEnabled(j != 0);
        ui->pushButtonUp->setEnabled(j != 0);
        ui->pushButtonDown->setEnabled(true);
        ui->pushButtonBottom->setEnabled(true);
        doc->setModelFeatureDisplayOrder(mDisplayOrder);
    }
#if !defined(NDEBUG)
    validateDisplayOrder();
#endif
}

void ModelFeaturesDialog::onMoveDown()
{
    int         lastPos = ui->listViewShow->model()->rowCount() - 1;

    QModelIndex idx     = ui->listViewShow->currentIndex();
    assert(idx.isValid());

    int k = idx.row();
    assert(k != -1);

    QMap<int, QVariant> roles;
    roles[Qt::DisplayRole] = ui->listViewShow->model()->data(idx, Qt::DisplayRole);
    ui->listViewShow->model()->removeRow(idx.row());                                     // DeleteString

    int j = k + 1;
    bool ok = ui->listViewShow->model()->insertRow(j);
    assert(ok);
    if (ok)
    {
        QModelIndex id = ui->listViewShow->model()->index(j, 0);
        ui->listViewShow->model()->setItemData(id, roles);                               // InsertString
        ui->listViewShow->setCurrentIndex(id);                                           // SetCurSel

        int m, n;
        for (int i = 0; i < mDisplayOrder.size(); ++i)
        {
            if (mDisplayOrder[i] == k)
            {
                n = i;
            }
            else if (mDisplayOrder[i] == k + 1)
            {
                m = i;
            }
        }
        mDisplayOrder[n] = k + 1;
        mDisplayOrder[m] = k;

        ui->pushButtonTop->setEnabled(true);
        ui->pushButtonUp->setEnabled(true);
        ui->pushButtonDown->setEnabled(j != lastPos);
        ui->pushButtonBottom->setEnabled(j != lastPos);
        doc->setModelFeatureDisplayOrder(mDisplayOrder);
    }

#if !defined(NDEBUG)
    validateDisplayOrder();
#endif
}

void ModelFeaturesDialog::onColor()
{
    QModelIndex index;
    QListView * listView;

    QModelIndex show = ui->listViewShow->currentIndex();
    QModelIndex hide = ui->listViewHide->currentIndex();

    if (show.isValid())
    {
        assert(show.row() != -1);
        index    = show;
        listView = ui->listViewShow;
    }
    else if (hide.isValid())
    {
        assert(hide.row() != -1);
        index    = hide;
        listView = ui->listViewHide;
    }
    else
    {
        // nothing to do
        assert(false);
        return;
    }

    double      rgba[4];
    QString     s  = listView->model()->data(index).toString();
    std::string ss = s.toStdString().append(40ul - static_cast<size_t>(s.size()), ' ');  // Fortran character[40]
    doc->modelFeatureColor(ss.c_str(), rgba);

    QColor current(rgba[0] * 255, rgba[1] * 255, rgba[2] * 255);
    QColor color = QColorDialog::getColor(current, this, tr("Color"));
    if (color.isValid())
    {
        rgba[0] = color.redF();
        rgba[1] = color.greenF();
        rgba[2] = color.blueF();
        rgba[3] = color.alphaF();
        assert(rgba[3] == 1);
        doc->setModelFeatureColor(ss.c_str(), rgba);
    }
}

#if !defined(NDEBUG)
bool ModelFeaturesDialog::validateDisplayOrder()
{
    int show = std::count_if(mDisplayOrder.begin(), mDisplayOrder.end(), [](int i) { return i >= 0; });
    int hide = std::count_if(mDisplayOrder.begin(), mDisplayOrder.end(), [](int i) { return i == -1; });
    assert(show == ui->listViewShow->model()->rowCount());
    assert(hide == ui->listViewHide->model()->rowCount());

    std::vector<std::string> labels = doc->modelFeatureLabels();
    std::vector<std::string> hidden;
    std::vector<std::string> showing(show, "missing");

    int                      lidx = 0;
    for (auto index : mDisplayOrder)
    {
        std::string s = labels[lidx];
        if (index == -1)
        {
            hidden.push_back(s);
        }
        else
        {
            showing[index] = s;
        }
        ++lidx;
    }

    int row  = 0;
    int rows = std::min(show, hide);
    for (; row < rows; ++row)
    {
        std::string hide = hidden[row];
        std::string show = showing[row];
        qDebug() << hide.c_str() << "    " << show.c_str();
    }
    rows = std::max(show, hide);
    for (; row < rows; ++row)
    {
        std::string hide;
        std::string show;
        if (hidden.size() > row)
        {
            hide = hidden[row];
        }
        else
        {
            hide = std::string(40, ' ');
        }
        if (showing.size() > row)
        {
            show = showing[row];
        }
        qDebug() << hide.c_str() << "    " << show.c_str();
    }
    return true;
}
#endif

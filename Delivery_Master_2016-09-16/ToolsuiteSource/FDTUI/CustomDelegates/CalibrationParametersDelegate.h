#ifndef CALIBRATIONPARAMETERSDELEGATE_H
#define CALIBRATIONPARAMETERSDELEGATE_H

class Project;
class QStyledItemDelegate;

class CalibrationParametersDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit CalibrationParametersDelegate(Project* project, QObject* parent = 0);
    ~CalibrationParametersDelegate();

private:
    Project* m_project;
    QStringList m_calibrationParameters;

private slots:
    void reloadCalibrationParameters();

    // QAbstractItemDelegate interface
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void setEditorData(QWidget* editor, const QModelIndex& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

    // QStyledItemDelegate interface
public:
    QString displayText(const QVariant& value, const QLocale& locale) const;
};

#endif // CALIBRATIONPARAMETERSDELEGATE_H

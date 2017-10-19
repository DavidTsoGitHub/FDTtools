#include "CalibrationParametersDelegate.h"

#include "Project.h"
#include <QComboBox>
#include <QStyledItemDelegate>

CalibrationParametersDelegate::CalibrationParametersDelegate(Project* project, QObject* parent) : QStyledItemDelegate(parent),
    m_project(project)
{
    reloadCalibrationParameters();

    connect(m_project, &Project::PropertyUpdated, [=]()
    {
        if (m_project->GetFunctionBlocks().IsDirty())
        {
            reloadCalibrationParameters();
        }
    });
}

CalibrationParametersDelegate::~CalibrationParametersDelegate()
{
}

QWidget* CalibrationParametersDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& /*option*/, const QModelIndex& /*index*/) const
{
    QComboBox* editor = new QComboBox(parent);

    editor->addItem(QString("<unset>"));
    foreach (QString calParameter, m_calibrationParameters)
    {
        editor->addItem(calParameter);
    }

    return editor;
}

void CalibrationParametersDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    if (m_calibrationParameters.contains(index.data().toString()))
    {
        combobox->setCurrentText(index.data().toString());
    }
    else
    {
        combobox->setCurrentText("<unset>");
    }

    combobox->showPopup();
}

void CalibrationParametersDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* combobox = static_cast<QComboBox*>(editor);
    QString newValue = combobox->currentText();
    if (newValue.compare("<unset>") == 0)
    {
        model->setData(index, QVariant(""));
    }
    else
    {
        model->setData(index, QVariant(combobox->currentText()));
    }
}

QString CalibrationParametersDelegate::displayText(const QVariant& value, const QLocale& /*locale*/) const
{
    if (!m_calibrationParameters.contains(value.toString()))
    {
        return QString("<unset>");
    }
    return value.toString();
}

void CalibrationParametersDelegate::reloadCalibrationParameters()
{
    m_calibrationParameters = m_project->GetAllCALParameters();
}

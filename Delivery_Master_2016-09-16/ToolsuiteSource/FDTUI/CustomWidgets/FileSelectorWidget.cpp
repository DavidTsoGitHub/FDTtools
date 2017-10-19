#include "FileSelectorWidget.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

FileSelectorWidget::FileSelectorWidget(QString path, QWidget* parent) : QWidget(parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(layout);
    this->setContentsMargins(0, 0, 0, 0);
    m_path = new QLineEdit(path);
    m_path->setToolTip(path);
    m_path->setReadOnly(true);

    m_browseButton = new QPushButton("...");
    m_browseButton->setFixedWidth(30);

    this->layout()->addWidget(m_path);
    this->layout()->addWidget(m_browseButton);

    connect(m_browseButton, &QAbstractButton::pressed, this, &FileSelectorWidget::slot_browseButton_pressed);
}

QString FileSelectorWidget::GetPath()
{
     return m_path->text();
}

void FileSelectorWidget::slot_browseButton_pressed()
{
    QString path = QFileDialog::getOpenFileName(this, "Select file");
    if (!path.isEmpty())
    {
        bool pathChanged = m_path->text().compare(path) != 0;

        m_path->setText(path);
        m_path->setToolTip(path);

        if (pathChanged) emit signal_FilePathChanged();
    }
}

#ifndef JAILEDFILESELECTOR_H
#define JAILEDFILESELECTOR_H

class QDialog;
class QFileSystemModel;

namespace Ui {
class JailedDirectorySelector;
}

class JailedDirectorySelector : public QDialog
{
    Q_OBJECT

public:
    explicit JailedDirectorySelector(const QString& rootPath, QWidget *parent = 0);
    ~JailedDirectorySelector();

    void SetSelectedDirectory(const QString& selectedPath);
    QString GetSelectedDirectoryAsRelative();

private:
    Ui::JailedDirectorySelector *m_ui;

    QFileSystemModel* m_fileSystemModel;
    const QString m_rootPath;

    // QDialog interface
public slots:
    void accept();
    void reject();
};

#endif // JAILEDFILESELECTOR_H

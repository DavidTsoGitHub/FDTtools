#ifndef DIRECTORYSELECTORWIDGET_H
#define DIRECTORYSELECTORWIDGET_H

class QLineEdit;
class QPushButton;
class QString;
class QWidget;

class DirectorySelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DirectorySelectorWidget(QString path = QString(), QWidget* parent = 0);
    QString GetPath();

signals:
    void signal_DirectoryPathChanged(void);

private slots:
    void slot_browseButton_pressed();

private:
    QLineEdit* m_path;
    QPushButton* m_browseButton;
};

#endif // DIRECTORYSELECTORWIDGET_H

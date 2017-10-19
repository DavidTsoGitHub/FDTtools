#ifndef FILESELECTORWIDGET_H
#define FILESELECTORWIDGET_H

class QLineEdit;
class QPushButton;
class QString;
class QWidget;

class FileSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FileSelectorWidget(QString path = QString(), QWidget* parent = 0);
    QString GetPath();

signals:
    void signal_FilePathChanged(void);

private slots:
    void slot_browseButton_pressed();

private:
    QLineEdit* m_path;
    QPushButton* m_browseButton;
};

#endif // FILESELECTORWIDGET_H

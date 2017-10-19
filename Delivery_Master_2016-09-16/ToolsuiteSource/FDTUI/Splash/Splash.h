#ifndef SPLASH_H
#define SPLASH_H

class QDialog;
class QWidget;
class QString;

namespace Ui
{
class Splash;
}

class Splash : public QDialog
{
    Q_OBJECT

public:
    explicit Splash(QWidget* parent = 0);
    ~Splash();
    QString GetFilePath(void);

private:
    Ui::Splash* m_ui;
    QString m_fileName;

    void exit(void);

private slots:
    void slot_btnOpenClicked(void);
    void slot_btnCreateClicked(void);
};

#endif // SPLASH_H

#ifndef ACTIVEBUZZER_H
#define ACTIVEBUZZER_H

#include <QMap>
#include <QWidget>

namespace Ui {
class ActiveBuzzer;
}

class ActiveBuzzer : public QWidget
{
    Q_OBJECT

public:
    explicit ActiveBuzzer(QWidget *parent = 0);
    ~ActiveBuzzer();

    void StopSound();
    void SetCurrentSound(quint8 soundId);

private:
    Ui::ActiveBuzzer *m_ui;

    QMap<int, QString> m_sounds;
};

#endif // ACTIVEBUZZER_H

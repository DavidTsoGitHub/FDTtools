#ifndef PASSIVEBUZZER_H
#define PASSIVEBUZZER_H

#include <QWidget>
#include <QMap>

namespace Ui {
class PassiveBuzzer;
}

class PassiveBuzzer : public QWidget
{
    Q_OBJECT

public:
    explicit PassiveBuzzer(QWidget *parent = 0);
    ~PassiveBuzzer();

    void StopSound();
    void SetCurrentSound(quint8 soundId, quint8 volume);    

private:
    Ui::PassiveBuzzer* m_ui;

    QMap<int, QString> m_sounds;
};

#endif // PASSIVEBUZZER_H

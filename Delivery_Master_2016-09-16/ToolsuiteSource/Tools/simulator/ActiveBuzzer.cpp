#include "ActiveBuzzer.h"
#include "ui_ActiveBuzzer.h"

#include <QDateTime>

typedef enum
{
    Warning_Infinite = 0x00,
    Warning_1Hz = 0x01
} warning_e;

ActiveBuzzer::ActiveBuzzer(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ActiveBuzzer)
{
    m_ui->setupUi(this);

    m_sounds.insert(Warning_Infinite, "Warning Infinite");
    m_sounds.insert(Warning_1Hz, "1Hz Beep");

    m_ui->currentSoundLabel->setText("None");
}

ActiveBuzzer::~ActiveBuzzer()
{
    delete m_ui;
}

void ActiveBuzzer::StopSound()
{
    m_ui->currentSoundLabel->setText("None");
}

void ActiveBuzzer::SetCurrentSound(quint8 soundId)
{
    if (m_sounds.contains(soundId))
    {
        m_ui->currentSoundLabel->setText(m_sounds.value(soundId));
    }
    else
    {
        m_ui->currentSoundLabel->setText("Unknown");
    }

    m_ui->lastRequestLabel->setText("Last Request: " + QDateTime::currentDateTime().toString("HH:mm:ss.zzz"));
}

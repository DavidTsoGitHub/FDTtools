#include "PassiveBuzzer.h"
#include "ui_PassiveBuzzer.h"

#include <QDateTime>

typedef enum
{
    Sound_Tick = 0x00,
    Sound_Tack = 0x01,
    Sound_Warn3 = 0x02,
    Sound_Warn4 = 0x03,
    Sound_Warn5 = 0x04
} sounds_e;


PassiveBuzzer::PassiveBuzzer(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::PassiveBuzzer)
{
    m_ui->setupUi(this);

    m_sounds.insert(Sound_Tick, "Tick");
    m_sounds.insert(Sound_Tack, "Tack");
    m_sounds.insert(Sound_Warn3, "Warn3");
    m_sounds.insert(Sound_Warn4, "Warn4");
    m_sounds.insert(Sound_Warn5, "Warn5");

    m_ui->currentSoundLabel->setText("None");
}

PassiveBuzzer::~PassiveBuzzer()
{
    delete m_ui;
}

void PassiveBuzzer::StopSound()
{
    m_ui->currentSoundLabel->setText("None");
}

void PassiveBuzzer::SetCurrentSound(quint8 soundId, quint8 volume)
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
    m_ui->volumeLabel->setText(QString::number(volume));
}

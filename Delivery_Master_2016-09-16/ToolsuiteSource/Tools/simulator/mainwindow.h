#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "comthread.h"
#include "function.h"
#include "SpeedGauge.h"
#include "FuelGauge.h"
#include "UreaLevel.h"
#include "ActiveBuzzer.h"
#include "FuelGauge.h"
#include "UreaLevel.h"
#include "PassiveBuzzer.h"
#include "MemoryTableWidget.h"
#include "TellTale.h"
#include "BSPSpecification.h"

#include "Configuration.h"
#include "DCU.h"
#include "DisplayWindow.h"

#include <QMainWindow>
#include <QJsonObject>
#include <QKeyEvent>
#include <QSignalMapper>
#include <QBitmap>

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QString bspSpecificationPath, QWidget* parent = 0);
    ~MainWindow();
    void Initialize(void);

signals:
    void sendToPCIC(const QString& json);

private:

    bool m_verbose;
    Ui::MainWindow* m_ui;
    ComThread* m_comThread;
    ActiveBuzzer* m_activeBuzzer;
    PassiveBuzzer* m_passiveBuzzer;
    SpeedGauge* m_speedGauge;
    FuelGauge* m_fuelGauge;
    UreaLevel* m_ureaLevel;
    DisplayWindow* m_display;

    MemoryTableWidget* m_memoryTable;
    QMap<int, TellTale*> m_tellTales;

    Configuration m_configuration;

    QString m_host;
    QString m_port;

    BSPSpecification m_bspSpecification;

    QImage m_displayImage;
    QClipboard* m_clipboard;

    DCU* m_dcu;

    void handleIncomingMessage(const QByteArray& jsonMessage);
    void sendFunction(const Function& function);
    void setupUISignals(void);
    void setControlPanelState(bool state);
    void addGauges(void);
    void initializeTellTales(void);
    void drawUreaLevel(void);
    void setupDisplayWindow(void);
    void setupFacePlate(void);

private slots:
    void slot_actionQuit_triggered(void);
    void slot_memoryTable_ByteChanged(quint16 address, quint8 value);
    void slot_pcicStateCheckbox_clicked(bool checked);
    void slot_pwrModeButton_pressed(qint32 id);
    void slot_rawVehicleSpeedSlider_valueChanged(int);
    void slot_menuButton_pressed(int discreteInputIndex);
    void slot_menuButton_released(int discreteInputIndex);
    void slot_createNewComThread(void);
    void slot_socketErrorOrDisconnected(void);

    void slot_discreteInputButtonGroup_buttonToggled(int id, bool checked);
    void slot_vehicleSpeedFrequency_valueChanged(int value);
    void slot_WUSourceCANBtn_Clicked(void);
    void slot_fuelLevel_valueChanged(int value);
    void slot_BrakeCircuitPressure_valueChanged(int value);
    void slot_BrakeReservoirPressure_valueChanged(int value);
    void slot_IgnitionCheckbox_Clicked(bool checked);

protected:
    void keyPressEvent(QKeyEvent* keyEvent);
    void keyReleaseEvent(QKeyEvent *);

};

#endif // MAINWINDOW_H

#include "function.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FlowLayout.h"
#include "BSPSegmentDisplay.h"
#include "BSPSpecificationDeserializer.h"
#include "Exceptions/CannotOpenFileException.h"
#include "Exceptions/DeserializationExeception.h"
#include "ConfigFileReader.h"
#include "DisplayWindow.h"

#include <QAbstractSocket>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QBitmap>
#include <QDateTime>
#include <QPainter>
#include <QMessageBox>
#include <QClipboard>
#include <QFileInfo>

#include "Exceptions/SimulatorGeneralException.h"

#define NUMBER_OF_DCU_LAYERS 16

#define SIMLAYER(A) (NUMBER_OF_DCU_LAYERS-A)
typedef enum
{
    Low_Power = 0x00,
    Ignition_Off = 0x01,
    Ignition_On = 0x02
} powerModes_e;

typedef enum
{
    DI_ButtonOK = 0x00,
    DI_ButtonCancel = 0x01,
    DI_ButtonUp = 0x02,
    DI_ButtonDown = 0x03,
    DI_ChargingIndication = 0x12

} discreteInputIndex_e;

typedef enum
{
    Gauge_Speedometer = 0x00,
    Gauge_EngineSpeed = 0x01,
    Gauge_FuelLevel   = 0x02
} gaugeIndex_e;


MainWindow::MainWindow(QString bspSpecificationPath, QWidget* parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_comThread(NULL),
    m_configuration(),
    m_bspSpecification(NULL)
{
    m_ui->setupUi(this);

    connect(m_ui->actionQuit, &QAction::triggered, this, &MainWindow::slot_actionQuit_triggered);

    connect(m_ui->actionAbout, &QAction::triggered, [=] ()
    {
        QMessageBox::about(this, tr("About"),
                           tr("BSP Simulator\n\n") + \
                              tr("Application Version") + ": " + QCoreApplication::applicationVersion() + \
                              "\n" + tr("Build Date") + ": " + QString(APP_BUILD_DATE) + \
                              "\n" + tr("Build") + ": " + QString(APP_BUILD) + \
                              "\n\nCopyright © 2015-2016 Swedspot AB");
    });

    m_memoryTable = new MemoryTableWidget();
    connect(m_memoryTable, &MemoryTableWidget::signal_ByteChanged, this, &MainWindow::slot_memoryTable_ByteChanged);
    m_ui->persistentMemoryWidgetContents->setLayout(new QGridLayout());
    m_ui->persistentMemoryWidgetContents->layout()->addWidget(m_memoryTable);
    m_ui->menuWindows->addAction(m_ui->persistentMemoryDock->toggleViewAction());
    m_ui->persistentMemoryDock->hide();

    m_display = new DisplayWindow(this);

    m_activeBuzzer = new ActiveBuzzer();
    m_ui->activeBuzzerGroupBox->layout()->addWidget(m_activeBuzzer);

    m_passiveBuzzer = new PassiveBuzzer();
    m_ui->passiveBuzzerGroupBox->layout()->addWidget(m_passiveBuzzer);

    QString bspSpecPath = bspSpecificationPath;
    BSPSpecificationDeserializer deserializer(m_bspSpecification);
    try
    {
        ConfigFileReader configFileReader;

        DeserializerBase::DeserializerRespons response = configFileReader.Deserialize(&deserializer, bspSpecPath);


        if (response != DeserializerBase::DeserializationOK)
        {
            switch (response)
            {
                case DeserializerBase::ErrorNoFile:
                {
                    throw CannotOpenFileException();
                }
                default:
                {
                    QFileInfo fileInfo(bspSpecPath);
                    QString errorMessage = configFileReader.Error();
                    throw DeserializationExeception(fileInfo, errorMessage);
                    //throw SimulatorGeneralException("Desrialization Error. The simulator will close.", true);
                }
            }
        }
    }
    catch(ExceptionBase& e)
    {
        e.InnerExceptionStrings().insert(0, e.Message());
        throw DeserializationExeception(e.InnerExceptionStrings(), bspSpecPath);
    }

    if (m_bspSpecification.SegmentDisplay() == NULL)
    {
        m_ui->lcdSegmentDisplay->setVisible(false);
    }
}

void MainWindow::Initialize()
{
    m_ui->menuButtonGroup->setId(m_ui->menuUpButton, DI_ButtonUp);
    m_ui->menuButtonGroup->setId(m_ui->menuDownButton, DI_ButtonDown);
    m_ui->menuButtonGroup->setId(m_ui->menuOkButton, DI_ButtonOK);
    m_ui->menuButtonGroup->setId(m_ui->menuCancelButton, DI_ButtonCancel);

    m_ui->discreteInputButtonGroup->setId(m_ui->discreteInputChargingSwitch, DI_ChargingIndication);

    m_ui->gaugeInRestPosLabel->setVisible(true);

    m_dcu = new DCU(m_bspSpecification.GetDisplay());

    setupDisplayWindow();

    setupUISignals();

    setupFacePlate();
    addGauges();

    initializeTellTales();
    drawUreaLevel();

    setControlPanelState(false);

    slot_createNewComThread();
}

MainWindow::~MainWindow()
{
    delete m_ui;
    delete m_display;
    delete m_comThread;
    delete m_dcu;
}

void MainWindow::setupUISignals()
{
    connect(m_ui->chkPCICOn, &QCheckBox::toggled, this, &MainWindow::slot_pcicStateCheckbox_clicked);

    /* Frequency input */
    connect(m_ui->vehicleSpeedFrequencySpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &MainWindow::slot_vehicleSpeedFrequency_valueChanged);
    connect(m_ui->vehicleSpeedFrequencySpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), m_ui->vehicleSpeedFrequencySlider, &QAbstractSlider::setValue);
    connect(m_ui->vehicleSpeedFrequencySlider, &QAbstractSlider::valueChanged, m_ui->vehicleSpeedFrequencySpinBox, &QSpinBox::setValue);

    /* Fuel */
    connect(m_ui->fuelSlider, &QAbstractSlider::valueChanged, this, &MainWindow::slot_fuelLevel_valueChanged);

    /* Brake Reservoir Pressure */
    connect(m_ui->brakeReservoirSlider, &QAbstractSlider::valueChanged, this, &MainWindow::slot_BrakeReservoirPressure_valueChanged);
    connect(m_ui->brakeReservoirPressureSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), m_ui->brakeReservoirSlider, &QAbstractSlider::setValue);
    connect(m_ui->brakeReservoirSlider, &QAbstractSlider::valueChanged, m_ui->brakeReservoirPressureSpinBox, &QSpinBox::setValue);

    /* Brake Circuit Pressure */
    connect(m_ui->brakeCircuitSlider, &QAbstractSlider::valueChanged, this, &MainWindow::slot_BrakeCircuitPressure_valueChanged);
    connect(m_ui->brakeCircuitPressureSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), m_ui->brakeCircuitSlider, &QAbstractSlider::setValue);
    connect(m_ui->brakeCircuitSlider, &QAbstractSlider::valueChanged, m_ui->brakeCircuitPressureSpinBox, &QSpinBox::setValue);

    /* Menu buttons */
    connect(m_ui->menuButtonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed), this, &MainWindow::slot_menuButton_pressed);
    connect(m_ui->menuButtonGroup, static_cast<void(QButtonGroup::*)(int)>(&QButtonGroup::buttonReleased), this, &MainWindow::slot_menuButton_released);

    /* Discrete inputs */
    connect(m_ui->discreteInputButtonGroup, static_cast<void(QButtonGroup::*)(int, bool)>(&QButtonGroup::buttonToggled), this, &MainWindow::slot_discreteInputButtonGroup_buttonToggled);

    //WU Sources
    connect(m_ui->btnCAN, &QAbstractButton::clicked, this, &MainWindow::slot_WUSourceCANBtn_Clicked);
    connect(m_ui->IgnitionOnCheckbox, &QAbstractButton::clicked, this, &MainWindow::slot_IgnitionCheckbox_Clicked);


}

void MainWindow::addGauges()
{
    m_speedGauge = new SpeedGauge(0, 149, m_ui->vehicleSpeedGroupBox);
    m_speedGauge->move(10, 10);

    m_ui->gaugeInRestPosLabel->raise();

    m_fuelGauge = new FuelGauge(m_ui->fuelLevelGroupBox);
    m_fuelGauge->move(10, 10);
}

void MainWindow::initializeTellTales()
{
    FlowLayout* layout = new FlowLayout();
    layout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout* turnLayout = new QHBoxLayout();
    turnLayout->setContentsMargins(0, 0, 0, 0);

    foreach (TellTaleConfiguration configuredTelltale, m_configuration.GetTellTales())
    {
        TellTale* telltale = new TellTale(configuredTelltale.BitmapPath, configuredTelltale.ActiveColor);
        m_tellTales.insert(configuredTelltale.Index, telltale);
        if(configuredTelltale.Index == 0x00)
        {
            turnLayout->addWidget(telltale);
            turnLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
        }
        else if(configuredTelltale.Index == 0x02)
        {
            turnLayout->addWidget(telltale);
        }
        else
        {
            layout->addWidget(telltale);
        }
    }

    m_ui->telltalesGroupBox->setLayout(layout);
    m_ui->turnIndicatorGroupBox->setLayout(turnLayout);
}

void MainWindow::drawUreaLevel()
{
    m_ui->ureaLevelGroupBox->setLayout(new QGridLayout());
    m_ui->ureaLevelGroupBox->layout()->setContentsMargins(0, 0, 0, 0);
    m_ureaLevel = new UreaLevel();
    m_ui->ureaLevelGroupBox->layout()->addWidget(m_ureaLevel);
}

void MainWindow::setupDisplayWindow()
{
    quint16 displayHeight = m_bspSpecification.GetDisplay()->GetHeight();
    quint16 displayWidth = m_bspSpecification.GetDisplay()->GetWidth();

    m_display->setWindowTitle("BSP Display " + QString::number(displayWidth) + "x" + QString::number(displayHeight));
    m_display->SetScene(m_dcu->GetGraphicsScene());

    if (m_bspSpecification.GetDisplay()->GetRenderingOrientation() == BSPDisplay::CounterClockWise)
    {
        m_display->SetSceneSize(m_bspSpecification.GetDisplay()->GetWidth(), m_bspSpecification.GetDisplay()->GetHeight(), DisplayWindow::CCW);
    }
    else
    {
        m_display->SetSceneSize(m_bspSpecification.GetDisplay()->GetWidth(), m_bspSpecification.GetDisplay()->GetHeight(), DisplayWindow::NORMAL);
    }

    m_display->SetBackgroundBrush(QBrush(Qt::black));

    m_display->show();
    m_display->resize(m_display->sizeHint());
}

void MainWindow::setupFacePlate()
{

    QPixmap pic("Telltales/faceplate.png");
    int width = m_ui->facePlateView->geometry().width();
    int height = m_ui->facePlateView->geometry().height();
    QGraphicsScene* faceScene = new QGraphicsScene(QRectF(0, 0, width, height), 0);
   faceScene->addPixmap(pic.scaled(QSize((int)faceScene->width(), (int)faceScene->height()), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    m_ui->facePlateView->fitInView(QRectF(0, 0, width, height), Qt::KeepAspectRatio);
    m_ui->facePlateView->setScene(faceScene);

    m_ui->facePlateView->show();
    m_ui->facePlateView->setStyleSheet("background: transparent; border: 0");

}


void MainWindow::setControlPanelState(bool state)
{
    m_ui->menuUpButton->setEnabled(state);
    m_ui->menuDownButton->setEnabled(state);
    m_ui->menuOkButton->setEnabled(state);
    m_ui->menuCancelButton->setEnabled(state);

    m_ui->IgnitionOnCheckbox->setChecked(false);
    m_ui->IgnitionOnCheckbox->setEnabled(state);

    m_ui->vehicleSpeedFrequencySlider->setValue(0);
    m_ui->vehicleSpeedFrequencySlider->setEnabled(state);
    m_ui->vehicleSpeedFrequencySpinBox->setEnabled(state);

    m_ui->discreteInputChargingSwitch->setChecked(false);
    m_ui->discreteInputChargingSwitch->setEnabled(state);

    m_ui->btnCAN->setEnabled(state);
    m_ui->labelPowerMode->setText("");

    m_ui->IgnitionOnCheckbox->setEnabled(state);
    m_ui->fuelSlider->setEnabled(state);

}

void MainWindow::slot_pcicStateCheckbox_clicked(bool checked)
{
    static bool oldStatus = false;

    QString functionName = checked ? "Start" : "Stop";
    Function function(functionName, Function::NO_TYPE);


    //turn of ingition of power off
    if (!checked && oldStatus)
    {
        m_ui->IgnitionOnCheckbox->setChecked(false);
        Function Voltagefunction("BSP_Get_BatteryVoltage", Function::SET, 0);
        sendFunction(Voltagefunction);
    }
    oldStatus = checked;
    sendFunction(function);
    setControlPanelState(checked);
}

void MainWindow::slot_pwrModeButton_pressed(qint32 id)
{
    Function function("BSP_Get_SysPwrMode", Function::SET, QString::number(id));
    sendFunction(function);
}

void MainWindow::slot_rawVehicleSpeedSlider_valueChanged(int value)
{
    Function function("BSP_Get_VehSpd", Function::SET, QString::number(value << 8));
    sendFunction(function);
}

void MainWindow::slot_menuButton_pressed(int discreteInputIndex)
{
    Function function("BSP_Get_DiscreteInput", Function::SET, "1", discreteInputIndex);
    sendFunction(function);
}

void MainWindow::slot_menuButton_released(int discreteInputIndex)
{
    Function function("BSP_Get_DiscreteInput", Function::SET, "0", discreteInputIndex);
    sendFunction(function);
}

void MainWindow::slot_createNewComThread()
{
    m_comThread = new ComThread();
    connect(m_comThread, &ComThread::finished, m_comThread, &QObject::deleteLater);
    connect(m_comThread, &ComThread::messageReceivedFromSimulator, this, &MainWindow::handleIncomingMessage, Qt::QueuedConnection);
    connect(this, &MainWindow::sendToPCIC, m_comThread, &ComThread::sendMessageToPCIC);
    connect(m_comThread, &ComThread::signal_socketDisconnected, this, &MainWindow::slot_socketErrorOrDisconnected);
    connect(m_comThread, &ComThread::signal_socketError, this, &MainWindow::slot_socketErrorOrDisconnected);
}

void MainWindow::slot_socketErrorOrDisconnected()
{
    m_comThread->quit();
    m_comThread->wait();

    slot_createNewComThread();
}

void MainWindow::slot_discreteInputButtonGroup_buttonToggled(int id, bool checked)
{
    QString value = (checked ? "1" : "0");
    Function function("BSP_Get_DiscreteInput", Function::SET, value , id);
    sendFunction(function);
}

void MainWindow::slot_vehicleSpeedFrequency_valueChanged(int value)
{
    qint32 fixedPointFloatValue = (value << 8); // 24.8
    Function function("BSP_Get_Frequency", Function::SET, QString::number(fixedPointFloatValue));
    sendFunction(function);
}

void MainWindow::slot_fuelLevel_valueChanged(int value)
{
    Function function("BSP_Get_FuelLevel", Function::SET, QString::number(value));
    sendFunction(function);
}

void MainWindow::slot_BrakeReservoirPressure_valueChanged(int value)
{
    Function function("BSP_Get_Front_Brake_Pressure", Function::SET, QString::number(value));
    sendFunction(function);
}

void MainWindow::slot_BrakeCircuitPressure_valueChanged(int value)
{
    Function function("BSP_Get_Rear_Brake_Pressure", Function::SET, QString::number(value));
    sendFunction(function);
}

void MainWindow::slot_actionQuit_triggered(void)
{
    this->close();
}

void MainWindow::slot_memoryTable_ByteChanged(quint16 address, quint8 value)
{
    qDebug("0x%04x = 0x%02x", address, value);
    Function function("BSP_Read_Memory", Function::SET, QString::number(value), address);
    sendFunction(function);
}

void MainWindow::slot_WUSourceCANBtn_Clicked(void)
{
    Function function("APPL_WakeupSourceNotify", Function::NO_TYPE, QString::number(1));
    sendFunction(function);
}

void MainWindow::slot_IgnitionCheckbox_Clicked(bool Checked)
{
    if (Checked)
    {
        //Send WU event on positive flank
        Function ignitionStatusFunction("BSP_Get_IgnitionStatus", Function::SET, QString::number(1));
        sendFunction(ignitionStatusFunction);
        Function function("APPL_WakeupSourceNotify", Function::NO_TYPE, QString::number(2));
        sendFunction(function);
    }
    else
    {
        Function ignitionStatusFunction("BSP_Get_IgnitionStatus", Function::SET, QString::number(0));
        sendFunction(ignitionStatusFunction);
    }
}

void MainWindow::sendFunction(const Function& function)
{
    QJsonObject json;
    function.Serialize(json);
    QJsonDocument doc(json);

    QString jsonString(doc.toJson(QJsonDocument::Indented));

    emit sendToPCIC(jsonString);
}

void MainWindow::handleIncomingMessage(const QByteArray& msg)
{
    QJsonObject json = QJsonDocument::fromJson(msg).object();
    Function* f = Function::Deserialize(json);

    if (f->GetName() == "BSP_Set_NeedlePosition")
    {
        switch (f->GetIndex())
        {
            case Gauge_Speedometer:
                if (f->GetValue() == "0")
                {
                    m_ui->gaugeInRestPosLabel->setVisible(true);
                    m_speedGauge->slot_setNeedleValue(0);
                }
                else
                {
                    m_ui->gaugeInRestPosLabel->setVisible(false);
                    m_speedGauge->slot_setNeedleValue(f->GetValue().toInt());
                }
                break;
            case Gauge_EngineSpeed:
                break;
            case Gauge_FuelLevel:
                m_fuelGauge->slot_setNeedleValue(f->GetValue().toInt());
                break;
        }
    }
    else if (f->GetName() == "BSP_Set_Telltale")
    {
        bool telltaleState = f->GetValue() != "0";

        switch (f->GetIndex())
        {
        case 0x27:
            m_ureaLevel->SetSegment0State(telltaleState);
            break;
        case 0x28:
            m_ureaLevel->SetSegment1State(telltaleState);
            break;
        case 0x29:
            m_ureaLevel->SetSegment2State(telltaleState);
            break;
        case 0x2A:
            m_ureaLevel->SetSegment3State(telltaleState);
            break;
        default:
            if (m_tellTales.contains(f->GetIndex()))
            {
                m_tellTales.value(f->GetIndex())->SetTellTaleActive(telltaleState);
            }

            break;
        }
    }
    else if (f->GetName() == "BSP_Service_FramebufferUpdated")
    {
        Function requestFramebufferContents("SendFramebufferContents", Function::NO_TYPE, 0);
        sendFunction(requestFramebufferContents);
    }
    else if (f->GetName() == "PCIC_FramebufferContents")
    {
        // Only used for SOFTWARE rendered graphics
        /*
        QByteArray decodedFrameBuffer(QByteArray::fromBase64(f->GetValue().toUtf8()));

        QBitmap displayBitmap = QBitmap::fromData(QSize(m_bspSpecification.GetDisplay()->GetWidth(), m_bspSpecification.GetDisplay()->GetHeight()), (uchar *)decodedFrameBuffer.constData());
        if (m_bspSpecification.GetDisplay()->GetRenderingOrientation() == BSPDisplay::CounterClockWise)
        {
            m_displayImage = displayBitmap.toImage().transformed(QMatrix().rotate(90.0));
        }
        else
        {
            m_displayImage = displayBitmap.toImage();
        }

        // TODO: Fix for SW rendering
        //m_ui->display->setPixmap(QPixmap::fromImage(m_displayImage));
        */

        QByteArray decodedFrameBuffer(QByteArray::fromBase64(f->GetValue().toUtf8()));
        m_dcu->SetLayerConfiguration(0, 0, 0, 240, 320, 255, 1, 0, true);
        m_dcu->SetLayerContent(0, decodedFrameBuffer);
        m_dcu->LayersUpdatedComplete();

        m_display->SetRedrawText("Last redraw: " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"));
    }
    else if (f->GetName() == "PCIC_LayersUpdatedComplete")
    {
        m_dcu->LayersUpdatedComplete();
        m_display->SetScene(m_dcu->GetGraphicsScene());
    }
    else if (f->GetName() == "PCIC_LayerContents")
    {
        // Only used to emulate HARDWARE rendered graphics
        quint8 layerID = SIMLAYER(f->GetIndex());
        QByteArray decodedFrameBuffer(QByteArray::fromBase64(f->GetValue().toUtf8()));
        m_dcu->SetLayerContent(layerID, decodedFrameBuffer);
        m_display->SetRedrawText("Last redraw: " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"));

    }
    else if (f->GetName() == "PCIC_LayerSetup")
    {
        QJsonObject json = QJsonDocument::fromJson(f->GetValue().toUtf8()).object();

        qint8 layerID = SIMLAYER( static_cast<quint8>(json["layerID"].toInt()) );
        qint16 x = static_cast<quint16>(json["x"].toInt());
        qint16 y = static_cast<quint16>(json["y"].toInt());
        qint16 width = static_cast<quint16>(json["width"].toInt());
        qint16 height = static_cast<quint16>(json["height"].toInt());
        qint8 bpp = static_cast<quint8>(json["bpp"].toInt());
        qint8 trans = static_cast<quint8>(json["trans"].toInt());
        qint8 clutOffset = static_cast<quint8>(json["clutOffset"].toInt());
        bool enabled = json["enabled"].toBool();


        m_dcu->SetLayerConfiguration(layerID, x, y, width, height, trans, bpp, clutOffset, enabled);

    }
    else if (f->GetName() == "PCIC_ColorData")
    {
        bool ok = false;
        quint32 argb = f->GetValue().toUInt(&ok, 16);
        if (!ok)
        {
            qDebug() << "PCIC_ColorData, failed to parse value: " << f->GetValue();
        }
        else
        {
            m_dcu->SetCLUT(f->GetIndex(), argb);
        }
    }
    else if (f->GetName() == "PCIC_BackgroundColor")
    {
        quint8 colorID = f->GetValue().toUInt();
        m_display->SetBackgroundBrush(QBrush(QColor(QRgb(m_dcu->GetBackgroundColor(colorID)))));
    }
    else if (f->GetName() == "PCIC_ChromaKey")
    {
        bool ok = false;
        //Chroma-key strings are encoded "0x00[startColor];0x00[endColor]", e.g. "0x00fa00fa;0x00ff00ff"
        QStringList chromaKeyRange = f->GetValue().split(";", QString::SkipEmptyParts);
        quint32 chromaKeyStart = 0;
        quint32 chromaKeyEnd = 0;

        if (chromaKeyRange.count() == 2)
        {
            chromaKeyStart = chromaKeyRange.at(0).toUInt(&ok, 16);
            chromaKeyEnd = chromaKeyRange.at(1).toUInt(&ok, 16);
        }

        if (!ok)
        {
            qDebug() << "PCIC_ChromaKey, failed to parse value: " << f->GetValue();
        }
        else
        {
            m_dcu->SetChromaKey(f->GetIndex(), chromaKeyStart, chromaKeyEnd);
        }
    }
    else if (f->GetName() == "BSP_Write_Memory")
    {
        QByteArray decodedCalBuffer(QByteArray::fromBase64(f->GetValue().toUtf8()));

        quint16 memAdress = ((quint8)decodedCalBuffer.at(0) << 8) | ((quint8)decodedCalBuffer.at(1));
        quint16 memLength = ((quint8)decodedCalBuffer.at(2) << 8) | ((quint8)decodedCalBuffer.at(3));

        for (int i = 0; i < memLength; ++i)
        {
            m_memoryTable->SetByte(memAdress + i, decodedCalBuffer.at(4+i));
        }
    }
    else if ( f->GetName() == "BSP_RequestModeChange")
    {
        m_ui->labelPowerMode->setText(f->GetValue());
    }
    else if (f->GetName() == "BSP_Sound_Buzzer")
    {
        quint8 soundId = f->GetValue().toUInt();
        m_activeBuzzer->SetCurrentSound(soundId);
    }
    else if (f->GetName() == "BSP_Service_SoundGen")
    {
        quint16 soundId = f->GetIndex();
        quint8 volume = f->GetValue().toUInt();
        m_passiveBuzzer->SetCurrentSound(soundId, volume);
    }
    else if (f->GetName() == "BSP_StopActiveSoundGeneration")
    {
        m_activeBuzzer->StopSound();
    }
    else if (f->GetName() == "BSP_Service_StopSoundGeneration")
    {
        m_passiveBuzzer->StopSound();
    }

    else if (f->GetName() == "BSP_Set_Illumination")
    {
        quint8 channel = f->GetIndex();
        QString level = f->GetValue();
        if(channel == 0)
            m_ui->gaugeBackLightLevel->setText(level + "%");
        else if(channel == 1)
            m_ui->displayBackLightLevel->setText(level + "%");

    }
    else if (f->GetName() == "BSP_Set_DisplaySegments")
    {
        QString value = f->GetValue();
        QString displayString;
        quint8 id = f->GetIndex();

        if (id & 0x01)
        {
            displayString = "C ";
        }

        if(id & 0x02)
        {
            m_ui->lcdSegmentDisplay->setDecMode();
            m_ui->lcdSegmentDisplay->setSmallDecimalPoint(true);

            displayString += value;
            QString pop = value.mid(value.length() - 1, 1);
            displayString.chop(1);
            displayString += "." + pop;
        }

        if(id == 0x00)
        {
            displayString = value;
        }
        m_ui->lcdSegmentDisplay->display(displayString);

    }

    delete f;
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent)
{
    switch(QChar::toLower(keyEvent->key()))
    {
        case 'z':
            keyEvent->accept();
            slot_menuButton_pressed(DI_ButtonOK);
            break;
        case 'x':
            keyEvent->accept();
            slot_menuButton_pressed(DI_ButtonCancel);
            break;
        case 'w':
            keyEvent->accept();
            slot_menuButton_pressed(DI_ButtonUp);
            break;
        case 's':
            keyEvent->accept();
            slot_menuButton_pressed(DI_ButtonDown);
            break;
        case 'p':
            {
                keyEvent->accept();
                m_clipboard = QApplication::clipboard();
                QPixmap pixMap = QPixmap::grabWidget(m_display->GetGraphicsView());
                m_clipboard->setImage(pixMap.toImage());
                QMessageBox::information(this, tr("Simulator"),
                    tr("Display bitmap was saved to clipboard.") );
            }
            break;
        default:
            QWidget::keyPressEvent(keyEvent);
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* keyEvent)
{
    switch(QChar::toLower(keyEvent->key()))
    { 
        case 'z':
            keyEvent->accept();
            slot_menuButton_released(DI_ButtonOK);
            break;
        case 'x':
            keyEvent->accept();
            slot_menuButton_released(DI_ButtonCancel);
            break;
        case 'w':
            keyEvent->accept();
            slot_menuButton_released(DI_ButtonUp);
            break;
        case 's':
            keyEvent->accept();
            slot_menuButton_released(DI_ButtonDown);
            break;
        default:
            QWidget::keyPressEvent(keyEvent);
    }
}

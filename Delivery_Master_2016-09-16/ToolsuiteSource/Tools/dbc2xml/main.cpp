#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QTextStream>

#include <iostream>

#include "DBCParser.h"

using namespace std;


std::ostream& operator<<(std::ostream& str, const QString& string)
{
    return str << string.toStdString();
}

int run(QString, QString, bool);

int main(int argc, char* argv[])
{
    QCoreApplication application(argc, argv);
    application.setOrganizationName("SwedSpot AB");
    application.setApplicationName("DBC2XML");
    application.setApplicationVersion(APP_VERSION);

    QCommandLineParser clParser;
    clParser.setApplicationDescription("Converts the specified DBC file to XML.\nVersion: " + QString(APP_VERSION) + "\nBuild: " + QString(APP_BUILD) + "\nBuild Date: " + QString(APP_BUILD_DATE) + "\n");
    clParser.addHelpOption();
    clParser.addPositionalArgument("source", QCoreApplication::translate("main", "Source DBC file."));
    clParser.addPositionalArgument("destination", QCoreApplication::translate("main", "Output XML file or directory (optional)."));

    // -v, --verbose
    QCommandLineOption verbosityOption(QStringList() << "v" << "verbose", QCoreApplication::translate("main", "Verbose output"));
    clParser.addOption(verbosityOption);

    // -f, --force-overwrite
    QCommandLineOption forceOption(QStringList() << "f" << "force-overwrite", QCoreApplication::translate("main", "Force overwriting of existing file"));
    clParser.addOption(forceOption);

    // parse the command line
    clParser.process(application);

    bool verbose = clParser.isSet(verbosityOption);
    bool force = clParser.isSet(forceOption);

    const QStringList args = clParser.positionalArguments();
    if (args.length() == 0 || args.length() > 2)
    {
        clParser.showHelp(0);
    }
    // the source file name is mandatory
    QString src = args.at(0);

    // determine the destination file name
    QString dest;
    if (args.length() == 2)
    {
        dest = args.at(1);
        QFileInfo fileInfo = QFileInfo(dest);
        if (fileInfo.exists() && fileInfo.isDir())
        {
            dest.append(QDir::separator());
            QString destName = src;
            int dotPos = destName.lastIndexOf(QString("."));
            if (dotPos > 0)
            {
                destName.truncate(dotPos);
            }
            dest.append(destName);
            dest.append(QString(".xml"));
        }
    }
    else
    {
        dest = src;
        int dotPos = dest.lastIndexOf(QString("."));
        if (dotPos > 0)
        {
            dest.truncate(dotPos);
        }
        dest.append(QString(".xml"));
    }

    // if the destination file exists and --force-overwrite was omitted, ask the user what to do:
    if (QFileInfo(dest).exists() && !force)
    {
        cout << "Destination file already exists. Overwrite, y/n? ";
        char overwrite;
        cin >> overwrite;
        if (!(overwrite == 'y' || overwrite == 'Y'))
        {
            exit(0);
        }
    }

    // run the application:
    return run(src, dest, verbose);
}

int run(QString src, QString dest, bool verbose)
{
    QFile dbcFile(src);
    if (!dbcFile.exists())
    {
        cout << "Source file does not exist." << endl;
        return -1;
    }

    if (!dbcFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout << "Could not open source file." << endl;
        return -1;
    }

    QString data(dbcFile.readAll());

    dbcFile.close();

    if (verbose)
    {
        cout << "Parsing source file " << QFileInfo(dbcFile).absoluteFilePath() << endl;
    }

    MessageAttributeMapping mapping;
    mapping.CycleTimeAttributeName = "GenMsgCycleTime";
    mapping.DelayTimeAttributeName = "GenMsgDelayTime";
    mapping.SendTypeAttributeName = "GenMsgSendType";
    mapping.SendTypeMapping.insert("0", CANMessage::Cyclic);
    mapping.SendTypeMapping.insert("1", CANMessage::Event);
    mapping.SendTypeMapping.insert("2", CANMessage::CyclicAndEvent);

    DBCParser parser(data, &mapping);

    const QStringList* attributes = parser.GetAttributeNames();
    const QStringList* modules = parser.GetModuleNames();
    const QList<CANMessage*>* messages = parser.GetMessages();

    QString xmlString;
    QXmlStreamWriter xml(&xmlString);
    xml.setAutoFormatting(true);
    xml.writeStartDocument();

    xml.writeStartElement("CAN");

    xml.writeStartElement("Attributes");
    for (int i = 0; i < attributes->length(); i++)
    {
        xml.writeTextElement("Attribute", attributes->at(i));
    }
    xml.writeEndElement(); // attributes

    xml.writeStartElement("Modules");
    for (int i = 0; i < modules->length(); i++)
    {
        xml.writeTextElement("Module", modules->at(i));
    }
    xml.writeEndElement(); // modules

    xml.writeStartElement("Messages");
    foreach (CANMessage* message, *messages)
    {
        if(message->CANMessageSendType == CANMessage::Undefined || message->CANMessageSendType == CANMessage::Unsupported)
        {
            cout << "Message '" << message->Name << "' defaulted to cyclic send type" << endl;
            message->CANMessageSendType = CANMessage::Cyclic;
        }
        xml.writeStartElement("Message");
        xml.writeAttribute("ID", QString::number(message->CANID));

        xml.writeTextElement("Name", message->Name);
        xml.writeTextElement("Producer", message->Producer);
        xml.writeTextElement("DLC", QString::number(message->DLC));

        xml.writeStartElement("Timing");
        xml.writeAttribute("Type", CANMessage::SendTypeString(message->CANMessageSendType));
        xml.writeTextElement("Cycle", QString::number(message->CycleTime));
        xml.writeTextElement("Delay", QString::number(message->Delay));
        xml.writeTextElement("InitialDelay", QString::number(message->InitialDelay));
        xml.writeTextElement("Timeout", QString::number(message->Timeout));
        xml.writeEndElement(); // timing

        xml.writeStartElement("Signals");
        foreach (const CANSignal* signalInMessage, message->Signals())
        {
            xml.writeStartElement("Signal");
            xml.writeAttribute("Name", signalInMessage->Name);
            xml.writeAttribute("TimeoutNotification", (signalInMessage->TimeoutNotification ? "Yes" : "No"));
            xml.writeAttribute("IsEventTrigger",  (signalInMessage->IsEventTrigger ? "True" : "False"));

            xml.writeStartElement("Position");
            xml.writeTextElement("StartBit", QString::number(signalInMessage->StartBit));
            xml.writeTextElement("Length", QString::number(signalInMessage->BitLength));
            xml.writeEndElement(); // position

            xml.writeStartElement("Value");
            xml.writeTextElement("Offset", QString::number(signalInMessage->Offset));
            xml.writeTextElement("Min", QString::number(signalInMessage->Min));
            xml.writeTextElement("Max", QString::number(signalInMessage->Max));
            xml.writeTextElement("Resolution", QString::number(signalInMessage->Scaling));
            xml.writeTextElement("UOM", signalInMessage->UOM);
            xml.writeEndElement(); // value

            xml.writeStartElement("Consumers");
            foreach (QString consumer, signalInMessage->Consumers)
            {
                xml.writeTextElement("Consumer", consumer);
            }
            xml.writeEndElement(); // consumers

            xml.writeEndElement(); // signal
        }
        xml.writeEndElement(); // signals

        xml.writeEndElement(); // message
    }
    xml.writeEndElement(); // messages

    xml.writeEndElement(); // dbc
    xml.writeEndDocument();

    // now write the xml string to file:
    QFile xmlFile(dest);
    if (!xmlFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        cout << "Could not open destination file." << endl;
        return -1;
    }
    if (verbose)
    {
        cout << "Writing XML data to " << QFileInfo(xmlFile).absoluteFilePath() << endl;
    }

    QTextStream stream(&xmlFile);
    stream << xmlString;

    xmlFile.close();

    return 0;
}

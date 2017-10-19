#ifndef DISPLAYERRORMESSAGESHANDLER_H
#define DISPLAYERRORMESSAGESHANDLER_H

#include <QObject>

class DisplayErrorMessagesHandler : public QObject
{
    Q_OBJECT
public:
    explicit DisplayErrorMessagesHandler(QObject *parent = 0);
    ~DisplayErrorMessagesHandler();

    void DisplayErrorMessage(const QString& information, QStringList* errorMessages);

public slots:
    void slot_DisplayErrorMessage(const QString& information, QStringList* errorMessages);
};

#endif // DISPLAYERRORMESSAGESHANDLER_H

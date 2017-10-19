#include "DisplayErrorMessagesHandler.h"

#include <QMessageBox>
#include <QObject>

DisplayErrorMessagesHandler::DisplayErrorMessagesHandler(QObject *parent) : QObject(parent)
{

}

DisplayErrorMessagesHandler::~DisplayErrorMessagesHandler()
{

}

void DisplayErrorMessagesHandler::DisplayErrorMessage(const QString& information, QStringList* errorMessages)
{
    slot_DisplayErrorMessage(information, errorMessages);
}

void DisplayErrorMessagesHandler::slot_DisplayErrorMessage(const QString& information, QStringList* errorMessages)
{
    QMessageBox::warning(NULL, information, errorMessages->join('\n'), QMessageBox::Ok);
}

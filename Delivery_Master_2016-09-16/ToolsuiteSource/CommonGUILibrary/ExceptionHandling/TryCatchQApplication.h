#ifndef TRYCATCHQAPPLICATION_H
#define TRYCATCHQAPPLICATION_H

#include <QApplication>

class QObject;

class TryCatchQApplication : public QApplication
{
    Q_OBJECT

public:
    TryCatchQApplication(int& argc, char* argv[]);
    ~TryCatchQApplication(void);

    // QCoreApplication interface
public:
    bool notify(QObject* receiver, QEvent* event);

signals:
    void signal_OnRecoverbleException(void);
    void signal_OnNewErrorMessages(const QString& information, QStringList* errorMessages);
};

#endif // TRYCATCHQAPPLICATION_H

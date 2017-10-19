#ifndef EXCEPTIONBASE_H
#define EXCEPTIONBASE_H

#include <stdexcept>
#include <QString>
#include <QStringList>

using std::runtime_error;

class ExceptionBase : public runtime_error
{
public:
    QStringList &InnerExceptionStrings(void);
    const QString& Information(void) const;
    const QString& Message(void) const;
    const QString& Name(void) const;
    const bool& Fatal(void) const;
    virtual ~ExceptionBase(void) throw();

protected:
    ExceptionBase(QString name, QString information, QString message, bool isFatal = false);
    ExceptionBase(QString name, QString information, QString message, QStringList innerExceptionStrings, bool isFatal = false);


private:
    bool m_fatal;
    QStringList m_innerExceptionStrings;
    QString m_information;
    QString m_message;
    QString m_name;

};

#endif // EXCEPTIONBASE_H

#ifndef REGEXPATTERNS
#define REGEXPATTERNS

#include <QRegExp>

class RegExPatterns
{
public:
    static const QRegExp NotAcceptedCodeCharacters(void);
    static const QRegExp NotAcceptedIntCharecters(void);
};

#endif // REGEXPATTERNS



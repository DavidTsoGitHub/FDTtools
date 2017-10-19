#include "RegExPatterns.h"

const QRegExp RegExPatterns::NotAcceptedCodeCharacters(void)
{
    QRegExp regExp("[^a-zA-Z0-9_]");
    return regExp;
}

const QRegExp RegExPatterns::NotAcceptedIntCharecters(void)
{
    QRegExp regExp("[^0-9]");
    return regExp;
}

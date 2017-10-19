#ifndef ILABELFONT
#define ILABELFONT

#include "ProjectObject.h"

class ILabelHasFont
{
public:
    virtual void SetFontID(quint32 id) = 0;
    virtual quint32 GetFontID() const = 0;
};

#endif // ILABELFONT


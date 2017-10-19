#include "VersionNumber.h"

VersionNumber::VersionNumber(int major, int minor, int stepping):
    m_major(major),
    m_minor(minor),
    m_stepping(stepping)
{
}

int VersionNumber::GetMajor() const
{
    return m_major;
}

int VersionNumber::GetMinor() const
{
    return m_minor;
}

int VersionNumber::GetStepping() const
{
    return m_stepping;
}

QString VersionNumber::GetVersion() const
{
    return QString("%1.%2.%3").arg(QString::number(m_major), QString::number(m_minor), QString::number(m_stepping));
}

bool operator<(const VersionNumber &leftHandSide, const VersionNumber &rightHandSide)
{
    if(leftHandSide.GetMajor() < rightHandSide.GetMajor() )
    {
        return true;
    }
    else if((leftHandSide.GetMajor() == rightHandSide.GetMajor()) &&
            (leftHandSide.GetMinor() < rightHandSide.GetMinor()))
    {
        return true;
    }
    //else if((leftHandSide.GetMajor() == rightHandSide.GetMajor()) &&
    //        (leftHandSide.GetMinor() == rightHandSide.GetMinor()) &&
    //        (leftHandSide.GetStepping() < rightHandSide.GetStepping()))
    //{
    //    return true;
    //}
    else
    {
        return false;
    }
}

bool operator==(const VersionNumber &leftHandSide, const VersionNumber &rightHandSide)
{
    if((leftHandSide.GetMajor() == rightHandSide.GetMajor()) &&
       (leftHandSide.GetMinor() == rightHandSide.GetMinor())/* &&
       (leftHandSide.GetStepping() == rightHandSide.GetStepping())*/)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool operator>(const VersionNumber &leftHandSide, const VersionNumber &rightHandSide)
{
    return rightHandSide < leftHandSide;
}

bool operator<=(const VersionNumber &leftHandSide, const VersionNumber &rightHandSide)
{
     return !(leftHandSide > rightHandSide);
}

bool operator>=(const VersionNumber &leftHandSide, const VersionNumber &rightHandSide)
{
     return !(leftHandSide < rightHandSide);
}

bool operator!=(const VersionNumber &leftHandSide, const VersionNumber &rightHandSide)
{
     return !(leftHandSide == rightHandSide);
}

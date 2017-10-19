#ifndef VERSIONNUMBER_H
#define VERSIONNUMBER_H

#include <QObject>

class VersionNumber
{

public:
    explicit VersionNumber(int major, int minor, int stepping);
    int GetMajor() const;
    int GetMinor() const;
    int GetStepping() const;
    QString GetVersion() const;

private:
    int m_major;
    int m_minor;
    int m_stepping;
};

bool operator<(const VersionNumber& leftHandSide, const VersionNumber& rightHandSide);
bool operator>(const VersionNumber& leftHandSide, const VersionNumber& rightHandSide);
bool operator<=(const VersionNumber& leftHandSide, const VersionNumber& rightHandSide);
bool operator>=(const VersionNumber& leftHandSide, const VersionNumber& rightHandSide);

bool operator==(const VersionNumber& leftHandSide, const VersionNumber& rightHandSide);
bool operator!=(const VersionNumber& leftHandSide, const VersionNumber& rightHandSide);
#endif // VERSIONNUMBER_H

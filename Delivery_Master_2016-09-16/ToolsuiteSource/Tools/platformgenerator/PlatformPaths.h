#ifndef PLATFORMPATHS
#define PLATFORMPATHS

#include <QString>


class PlatformPaths
{
public:

    typedef enum
    {
        PC
    } PlatformTarget;

    static const QString GetMCALTargetPath(PlatformTarget target);
    static const QString GetMCALCommonPath(void);
    static const QString CreatePlatformComponentPath(QString componentName);
    static const QString CreateFunctionBlockPath(QString functionBlockName);
    static const QString CreateServicePath(QString serviceName);
    static const QString CreateUIWidgetPath(QString widgetName);

private:
    static QString m_FunctionBlockRoot;
    static QString m_ServicesRoot;
    static QString m_UIWidgetRoot;
    static QString m_PlatformRoot;
    static QString m_MCALRoot;

};

#endif // PLATFORMPATHS

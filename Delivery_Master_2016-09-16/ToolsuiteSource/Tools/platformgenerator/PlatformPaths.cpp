#include "PlatformPaths.h"


QString PlatformPaths::m_FunctionBlockRoot("Application/FunctionBlocks");
QString PlatformPaths::m_UIWidgetRoot("Application/UIWidgets");
QString PlatformPaths::m_ServicesRoot("Application/Services");
QString PlatformPaths::m_PlatformRoot("Platform");
QString PlatformPaths::m_MCALRoot("MCAL");


const QString PlatformPaths::GetMCALTargetPath(PlatformTarget target)
{
    switch (target)
    {
        case PC:
            return QString(m_MCALRoot).append("/PC");
    }

    return QString(m_MCALRoot).append("/Undefined");
}

const QString PlatformPaths::GetMCALCommonPath()
{
        return QString(m_MCALRoot).append("/Common");
}

const QString PlatformPaths::CreatePlatformComponentPath(QString componentName)
{
    return QString(m_PlatformRoot).append("/").append(componentName);
}

const QString PlatformPaths::CreateFunctionBlockPath(QString functionBlockName)
{
    return QString(m_FunctionBlockRoot).append("/").append(functionBlockName);
}

const QString PlatformPaths::CreateServicePath(QString serviceName)
{
    return QString(m_ServicesRoot).append("/").append(serviceName);
}

const QString PlatformPaths::CreateUIWidgetPath(QString widgetName)
{
    return QString(m_UIWidgetRoot).append("/").append(widgetName);
}

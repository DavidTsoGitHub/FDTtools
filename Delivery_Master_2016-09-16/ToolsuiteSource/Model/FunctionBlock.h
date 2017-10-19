#ifndef FUNCTIONBLOCK
#define FUNCTIONBLOCK

#include "FunctionDefinition.h"
#include "ProjectObject.h"
#include "IGCLSignalSource.h"
#include "IGCLSignalSink.h"
#include "ISelfValidation.h"

#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QMap>

class FunctionBlock : public ProjectObject, public IGCLSignalSource, public IGCLSignalSink, public ISelfValidation
{
public:
    explicit FunctionBlock(ProjectObject* parent = 0);
    void SetPathInFunctionLibrary(QString pathInFunctionLibrary);

    // FunctionBlock interface
public:
    void SetID(qint32 id);
    qint32 GetID() const;
    const QString GetPathInFunctionLibrary() const;
    bool Initialize(QDir functionLibraryRoot);
    const FunctionDefinition& GetFunctionDefinition() const;

    bool HasConfigurationParameter(const QString& parameterName) const;
    QString GetConfigurationParameterValue(const QString& parameterName) const;
    void SetConfigurationParameter(const QString& parameterName, const QString& parameterValue);

    bool HasCalibrationParameterDefaultValue(const QString& parameterName) const;
    QString GetCalibrationParameterDefaultValue(const QString& parameterName) const;
    void SetCalibrationParameterDefaultValue(const QString& parameterName, const QString& parameterValue);
    const QMap<QString, QString> GetAllCalibrationParameters(void) const;

    void AddLabelMappingsParameter(LabelMappings* labelMappings);
    LabelMappings* GetLabelMappingsParameter(const QString& labelMappingsName) const;
    bool HasLabelMappings(void) const;

    bool HasDTCParameter(const QString& faultName) const;
    void SetDTCParameterID(const QString& faultName, quint32 dtcID);
    quint32 GetDTCParameterID(const QString& faultName) const;

    void SetIconResourceID(const QString& resourceName, quint32 iconID);
    quint32 GetIconResourceID(const QString& resourceName) const;

    void SetImageResourceID(const QString& resourceName, quint32 imageID);
    quint32 GetImageResourceID(const QString& resourceName) const;

private:
    qint32 m_id;
    QString m_pathInFunctionLibrary;

    QMap<QString, QString> m_configurationParameters;
    QMap<QString, QString> m_calibrationParameterDefaultValues;
    QMap<QString, qint32> m_dtcParameterIDs;
    QMap<QString, qint32> m_iconResourceIDs;
    QMap<QString, qint32> m_imageResourceIDs;

    FunctionDefinition m_functionDefinition;
    bool m_isInitialized;

    // IGCLSignalSink interface
public:
    const QList<GCLSignalSink*>& GetSignalSinks() const;
    GCLSignalSink* GetSignalSink(QString signalName) const;

    // IGCLSignalSource interface
public:
    const QString GetComponentName() const;
    const QList<GCLSignalSource*>& GetSignalSources() const;
    GCLSignalSource* GetSignalSource(QString signalName) const;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor& visitor);

    // ISelfValidation interface
public:
    bool objectSelfValidation(Project* project, QStringList* errorList);
private:
    bool validateImageResources(Project* project, QStringList* errorList);
    bool validateConfigurationParameters(QStringList* errorList);
    bool validateCalibrationParameters(QStringList* errorList);
};

#endif // FUNCTIONBLOCK

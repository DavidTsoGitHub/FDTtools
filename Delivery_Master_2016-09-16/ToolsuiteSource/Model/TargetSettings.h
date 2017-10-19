#ifndef BUILDTARGETCONTAINER_H
#define BUILDTARGETCONTAINER_H

class ProjectObject;

class QString;

class TargetSettings
{
public:
    TargetSettings();
    explicit TargetSettings(const TargetSettings& other);

    typedef enum
    {
        Simulator,
        TargetHW
    } Target;

    typedef enum
    {
        LittleEndian,
        BigEndian
    } TargetEndianess;

    void SetActiveTarget(Target target);
    Target GetActiveTarget();

    void SetSimulatorCompilerName(const QString& compiler);
    const QString& GetSimulatorCompilerName() const;

    void SetTargetCompilerName(const QString& compiler);
    const QString& GetTargetCompilerName() const;

    void SetTargetEndianess(TargetEndianess endianess);
    TargetEndianess GetTargetEndianess() const;

    void SaveSettings(ProjectObject* project);

private:
    Target m_activeTarget;
    QString m_simulatorCompilerName;
    QString m_targetCompilerName;
    TargetEndianess m_targetEndianess;
};

#endif // BUILDTARGETCONTAINER_H

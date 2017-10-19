#include "AnimationManagerGenerator.h"
#include "Logger.h"

const quint32 AnimationManagerGenerator::m_animationManagerCycleTimeMs = 50u;

AnimationManagerGenerator::AnimationManagerGenerator(Project *project) :
    m_project(project),
    m_numberOfAnimations(0)
{
    m_animationsCfgHeaderPath = QString(PlatformPaths::CreateServicePath("AnimationManager")).append("/Animations_cfg.h");
    m_animationsCfgSourcePath = QString(PlatformPaths::CreateServicePath("AnimationManager")).append("/Animations_cfg.c");
}

AnimationManagerGenerator::~AnimationManagerGenerator()
{

}

void AnimationManagerGenerator::OnGenerateFiles()
{
    LOG_DEBUG("Entering");
    generateAnimationsCfgSource();
    generateAnimationsCfgHeader();
}

void AnimationManagerGenerator::generateAnimationsCfgHeader()
{
    QStringList fileLines;

    fileLines.append("#ifndef ANIMATIONS_CFG_H");
    fileLines.append("#define ANIMATIONS_CFG_H");
    fileLines.append("");
    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"StandardTypes.h\"");
    fileLines.append("");
    fileLines.append("typedef enum");
    fileLines.append("{");
    fileLines.append("    FRAMEBYFRAME_ANIMATION,");
    fileLines.append("    OPACITY_ANIMATION,");
    fileLines.append("    TRANSLATION_ANIMATION");
    fileLines.append("} animation_type_e;");
    fileLines.append("");
    fileLines.append("typedef enum");
    fileLines.append("{");
    fileLines.append("    LOOP_ANIMATION,");
    fileLines.append("    ONESHOT_ANIMATION,");
    fileLines.append("    BOUNCE_ANIMATION,");
    fileLines.append("    BOUNCE_LOOP_ANIMATION");
    fileLines.append("} animation_repeatmode_e;");
    fileLines.append("");
    fileLines.append("#define NUMBER_OF_ANIMATIONS " + QString::number(m_numberOfAnimations) + "u");
    fileLines.append("");
    fileLines.append("typedef struct");
    fileLines.append("{");
    fileLines.append("    sint16 x;");
    fileLines.append("    sint16 y;");
    fileLines.append("} animation_pos_t;");
    fileLines.append("");
    fileLines.append("typedef union ");
    fileLines.append("{");
    fileLines.append("    animation_pos_t position;");
    fileLines.append("    uint32 imageID;");
    fileLines.append("    uint8 opacity;");
    fileLines.append("} animation_value_t;");
    fileLines.append("");
    fileLines.append("typedef struct");
    fileLines.append("{");
    fileLines.append("    sint16 step;");
    fileLines.append("    animation_value_t value;");
    fileLines.append("} animation_steps_t;");
    fileLines.append("");
    fileLines.append("typedef struct");
    fileLines.append("{");
    fileLines.append("    uint32 id;");
    fileLines.append("    animation_type_e type;");
    fileLines.append("    animation_repeatmode_e repeat;");
    fileLines.append("    sint16 stepCount;");
    fileLines.append("    const animation_steps_t* steps;");
    fileLines.append("    sint16 defaultStep;");
    fileLines.append("    void (*AnimationSourceFunction)(uint32 value);");
    fileLines.append("} animation_t;");
    fileLines.append("");
    fileLines.append("extern const animation_t animations[NUMBER_OF_ANIMATIONS];");
    quint32 count = 1;
    foreach (AnimationFrameByFrame* animation, m_project->GetAnimationDefinition().FrameByFrameAnimations())
    {
        fileLines.append("extern const animation_steps_t frameByFrameAnimation_" + QString::number(count) + "[" + QString::number(animation->ImageIds().count()) + "];");
        ++count;
    }
    count = 1;
    foreach (AnimationOpacity* animation, m_project->GetAnimationDefinition().OpacityAnimations())
    {
        quint32 steps = (animation->GetLengthMs() / m_animationManagerCycleTimeMs) + 1;
        fileLines.append("extern const animation_steps_t opacityAnimation_" + QString::number(count) + "[" + QString::number(steps) + "];");
        ++count;
    }
    count = 1;
    foreach (AnimationTranslation* animation, m_project->GetAnimationDefinition().TranslationAnimations())
    {
        quint32 steps = (animation->GetLengthMs() / m_animationManagerCycleTimeMs) + 1;
        fileLines.append("extern const animation_steps_t translationAnimation_" + QString::number(count) + "[" + QString::number(steps) + "];");
        ++count;
    }
    fileLines.append("");
    fileLines.append("#endif //ANIMATIONS_CFG_H");
    fileLines.append("");

    AddGeneratedFile(m_animationsCfgHeaderPath, fileLines.join("\n"));
}

void AnimationManagerGenerator::generateAnimationsCfgSource()
{
    QStringList fileLines;
    QStringList animationList;

    fileLines.append("/********************************* Includes **********************************/");
    fileLines.append("#include \"Animations_cfg.h\"");
    fileLines.append("#include \"GCL_AnimationManager.h\"");
    fileLines.append("");

    if (m_project->GetAnimationDefinition().FrameByFrameAnimations().count() > 0)
    {
        fileLines.append("// --- FrameByFrame Animations ------------------------------------------------");
        fileLines.append("");
        quint32 count = 1;
        foreach (AnimationFrameByFrame* animation, m_project->GetAnimationDefinition().FrameByFrameAnimations())
        {
            fileLines.append("const animation_steps_t frameByFrameAnimation_" + QString::number(count) + "[" + QString::number(animation->ImageIds().count()) + "] = \n{");
            quint32 stepTime = 0;
            bool firstStep = true;
            foreach (quint32 imageID, animation->ImageIds())
            {
                if (firstStep)
                {
                    fileLines.append("    { .step = 0, .value = { .imageID = " + QString::number(imageID) + "u } },");
                    firstStep = false;
                }
                else
                {
                    stepTime += animation->GetFrameDurationMs();
                    fileLines.append("    { .step = " + QString::number(qRound(stepTime / 50.0)) + ", .value = { .imageID = " + QString::number(imageID) + "u } },");
                }
            }

            QString repeatMode;
            switch (animation->GetRepeatMode())
            {
                case Animation::RepeatMode::LOOP:
                    repeatMode = "LOOP_ANIMATION";
                    break;
                case Animation::RepeatMode::ONESHOT:
                    repeatMode = "ONESHOT_ANIMATION";
                    break;
                case Animation::RepeatMode::BOUNCE:
                    repeatMode = "BOUNCE_ANIMATION";
                    break;
                case Animation::RepeatMode::BOUNCE_LOOP:
                    repeatMode = "BOUNCE_LOOP_ANIMATION";
                    break;
                default:
                    repeatMode = "UNKNOWN_ANIMATION";
                    LOG_ERROR("Unkown repeat mode for animation!");
                    break;
            }

            animationList.append("    { .id = " + QString::number(animation->GetID()) + "u, .type = FRAMEBYFRAME_ANIMATION, .repeat = " + repeatMode + ", .stepCount = " + QString::number(animation->ImageIds().count()) + ", .steps = frameByFrameAnimation_" + QString::number(count) + ", .defaultStep = " + QString::number(animation->GetDefaultStep()) + ", .AnimationSourceFunction = GCL_Write_" + animation->GetComponentName() + "_CurrentImageID }");

            fileLines.append("};");
            fileLines.append("");
            ++count;
        }
    }

    if (m_project->GetAnimationDefinition().OpacityAnimations().count() > 0)
    {
        fileLines.append("// --- Transparencey Animations -----------------------------------------------");
        fileLines.append("");
        quint32 count = 1;
        foreach (AnimationOpacity* animation, m_project->GetAnimationDefinition().OpacityAnimations())
        {
            quint32 steps = (animation->GetLengthMs() / m_animationManagerCycleTimeMs) + 1;
            fileLines.append("const animation_steps_t opacityAnimation_" + QString::number(count) + "[" + QString::number(steps) + "] = \n{");
            for (quint32 step = 0; step < steps; ++step)
            {
                fileLines.append("    { .step = " + QString::number(step) + ", .value = { .opacity = " + QString::number(static_cast<quint8>(animation->GetOpacityAtTime(step * m_animationManagerCycleTimeMs) * (255.0/100))) + "u } },");
            }

            QString repeatMode;
            switch (animation->GetRepeatMode())
            {
                case Animation::RepeatMode::LOOP:
                    repeatMode = "LOOP_ANIMATION";
                    break;
                case Animation::RepeatMode::ONESHOT:
                    repeatMode = "ONESHOT_ANIMATION";
                    break;
                case Animation::RepeatMode::BOUNCE:
                    repeatMode = "BOUNCE_ANIMATION";
                    break;
                case Animation::RepeatMode::BOUNCE_LOOP:
                    repeatMode = "BOUNCE_LOOP_ANIMATION";
                    break;
                default:
                    repeatMode = "UNKNOWN_ANIMATION";
                    LOG_ERROR("Unkown repeat mode for animation!");
                    break;
            }

            QString defaultStep;
            switch (animation->GetDefaultStep())
            {
                case -1:
                    defaultStep = "-1";
                    break;
                case 0:
                    defaultStep = "0";
                    break;
                case 1:
                    defaultStep = QString::number(steps - 1);
                    break;
                default:
                    defaultStep = "-1";
                    break;
            }

            animationList.append("    { .id = " + QString::number(animation->GetID()) + "u, .type = OPACITY_ANIMATION, .repeat = " + repeatMode + ", .stepCount = " + QString::number(steps) + ", .steps = opacityAnimation_" + QString::number(count) + ", .defaultStep = " + defaultStep + ", .AnimationSourceFunction = NULL }");

            fileLines.append("};");
            fileLines.append("");
            ++count;
        }
    }


    if (m_project->GetAnimationDefinition().TranslationAnimations().count() > 0)
    {
        fileLines.append("// --- Translation Animations -------------------------------------------------");
        fileLines.append("");
        quint32 count = 1;
        foreach (AnimationTranslation* animation, m_project->GetAnimationDefinition().TranslationAnimations())
        {
            quint32 steps = (animation->GetLengthMs() / m_animationManagerCycleTimeMs) + 1;
            fileLines.append("const animation_steps_t translationAnimation_" + QString::number(count) + "[" + QString::number(steps) + "] = \n{");
            for (quint32 step = 0; step < steps; ++step)
            {
                QPointF point = animation->GetRelativePosAtTime(step*m_animationManagerCycleTimeMs);
                fileLines.append("    { .step = " + QString::number(step) + ", .value = { .position = { .x = " + QString::number(qRound(point.x())) + ", .y = " + QString::number(qRound(point.y())) + " } } },");
            }

            QString repeatMode;
            switch (animation->GetRepeatMode())
            {
                case Animation::RepeatMode::LOOP:
                    repeatMode = "LOOP_ANIMATION";
                    break;
                case Animation::RepeatMode::ONESHOT:
                    repeatMode = "ONESHOT_ANIMATION";
                    break;
                case Animation::RepeatMode::BOUNCE:
                    repeatMode = "BOUNCE_ANIMATION";
                    break;
                case Animation::RepeatMode::BOUNCE_LOOP:
                    repeatMode = "BOUNCE_LOOP_ANIMATION";
                    break;
                default:
                    repeatMode = "UNKNOWN_ANIMATION";
                    LOG_ERROR("Unkown repeat mode for animation!");
                    break;
            }

            QString defaultStep;
            switch (animation->GetDefaultStep())
            {
                case -1:
                    defaultStep = "-1";
                    break;
                case 0:
                    defaultStep = "0";
                    break;
                case 1:
                    defaultStep = QString::number(steps - 1);
                    break;
                default:
                    defaultStep = "-1";
                    break;
            }

            animationList.append("    { .id = " + QString::number(animation->GetID()) + "u, .type = TRANSLATION_ANIMATION, .repeat = " + repeatMode + ", .stepCount = " + QString::number(steps) + ", .steps = translationAnimation_" + QString::number(count) + ", .defaultStep = " + defaultStep + ", .AnimationSourceFunction = NULL }");

            fileLines.append("};");
            fileLines.append("");
            ++count;
        }
    }

    fileLines.append("const animation_t animations[NUMBER_OF_ANIMATIONS] =");
    fileLines.append("{");
    fileLines.append(animationList.join(",\n"));
    fileLines.append("};");
    fileLines.append("");

    m_numberOfAnimations = animationList.count();

    AddGeneratedFile(m_animationsCfgSourcePath, fileLines.join("\n"));
}

#ifndef LABELMANAGERGENERATOR_H
#define LABELMANAGERGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"

#include <QVector>
#include <QSet>

class LabelManagerGenerator : public FileGeneratorBase
{
public:
    explicit LabelManagerGenerator(Project* project);

    // FileGeneratorBase interface
public:
    void OnGenerateFiles(void);

private:
    Project* m_project;
    LabelDefinition& m_labelDefinition;
    QVector<QString> m_stringVector;

    QString m_labelManagerHeaderPath;
    QString m_labelManagerSourcePath;
    QString m_stringDBHeaderPath;
    QString m_fontRoot;

    void generateLabelManagerHeader(void);
    void generateLabelManagerSource(void);
    int generateStringDBEntry(QString string);
    void generateFonts(void);
    void generateStringDBHeader(void);

    QSet<quint16> getUTF16CharactersUsedByFont(const Font* font);
};

#endif // LABELMANAGERGENERATOR_H

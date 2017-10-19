#ifndef BUILDER_H
#define BUILDER_H

#include <QString>
#include <QDir>

class Builder
{
public:
    struct clOptions
    {
        bool build;
        bool generate;
        bool quickBuild;
        QString compilerString;
        QDir outputRoot;
    };

    explicit Builder(clOptions options);
    ~Builder();

    bool Run(void);

private:
        clOptions m_options;
};

#endif // BUILDER_H

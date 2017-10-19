#include "CompilerFactory.h"
#include "TestCompiler.h"
#include "GCCCompiler.h"
#include "CWCompiler.h"
#include <QList>


QList<CompilerBase*> CompilerFactory::GetAvailableCompilers()
{
    QList<CompilerBase*> availableCompilers;

    availableCompilers.append(new GCCCompiler());
    availableCompilers.append(new CWCompiler());
    availableCompilers.append(new TestCompiler());

    return availableCompilers;
}

CompilerBase* CompilerFactory::GetCompiler(QString compilerXml)
{
    QString compilerType = PeekForType(compilerXml);
    CompilerBase* compiler;

    if (compilerType.compare("TestCompiler") == 0)
    {
        compiler = new TestCompiler();
    }
    else if (compilerType.compare("GCC Compiler") == 0)
    {
        compiler = new GCCCompiler();
    }
    else if (compilerType.compare("CodeWarrior_MPC5606S") == 0)
    {
        compiler = new CWCompiler();
    }
    else
    {
        return NULL;
    }

    compiler->Deserialize(compilerXml);
    return compiler;
}

CompilerBase* CompilerFactory::GetCompilerSkeleton(QString compilerType)
{
    if (compilerType.compare("TestCompiler") == 0)
    {
        return new TestCompiler();
    }
    else if (compilerType.compare("GCC Compiler") == 0)
    {
        return new GCCCompiler();
    }
    else if (compilerType.compare("CodeWarrior_MPC5606S") == 0)
    {
        return new CWCompiler();
    }
    else
    {
        return NULL;
    }
}

QString CompilerFactory::PeekForType(QString compilerXml)
{
    QXmlStreamReader reader(compilerXml);

    if (reader.readNextStartElement())
    {
        if (reader.name() == "CompilerData")
        {
            while (!(reader.name() == "CompilerData" && reader.isEndElement()) && !reader.atEnd())
            {
                reader.readNext();
                if (!reader.isStartElement()) continue;

                if (reader.name() == "Type")
                {
                    return reader.readElementText();
                }
            }
        }
        else
        {
            return "";
        }
    }

    return "";
}

QString CompilerFactory::PeekForName(QString compilerXml)
{
    QXmlStreamReader reader(compilerXml);

    if (reader.readNextStartElement())
    {
        if (reader.name() == "CompilerData")
        {
            while (!(reader.name() == "CompilerData" && reader.isEndElement()) && !reader.atEnd())
            {
                reader.readNext();
                if (!reader.isStartElement()) continue;

                if (reader.name() == "Name")
                {
                    return reader.readElementText();
                }
            }
        }
        else
        {
            return "";
        }
    }

    return "";
}

#include "Builder.h"

#include "Logger.h"
#include "CompilerFactory.h"

Builder::Builder(clOptions options) :
    m_options(options)
{

}

Builder::~Builder()
{

}

bool Builder::Run()
{
    CompilerBase* compiler = CompilerFactory::GetCompiler(m_options.compilerString);

    if (compiler == NULL)
    {
        LOG_ERROR("Couldn't create instance of compiler using XML: " + m_options.compilerString);
        return false;
    }

    if (!compiler->HasMandatoryCompilerSpecificData())
    {
        LOG_ERROR("Mandatory compiler settings missing, goto \"Tools->Settings->Compilers\" and check the complier settings for the current build target!");
        return false;
    }

    if(m_options.generate)
    {
        LOG_INFO("Generating buildfiles to " + m_options.outputRoot.absolutePath() + "...");

        compiler->quickBuild = m_options.quickBuild;

        if (!compiler->GenerateBuildEnvironment(m_options.outputRoot.absolutePath()))
        {
            LOG_ERROR("Generating buildfiles failed!");
            return false;
        }
        LOG_INFO("Generating buildfiles...done!");
    }
    if(m_options.build)
    {
        LOG_INFO("Building " + m_options.outputRoot.absolutePath() + "...");
        if (!compiler->Build(m_options.outputRoot.absolutePath()))
        {
            LOG_ERROR("Build failed!");
            return false;
        }



        LOG_INFO("Building ... done!");
    }

    return true;
}

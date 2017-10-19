/*
 * Autotest.h
 * Framework for registering and running multiple qt test suites.
 *
 * Usage:
 *  Create your qttest suite class.
 *  Include the test suite in the list of suites to run by using
 *  the DECLARE_TEST macro specified in this file.
 *
 *  Define a main function somewhere in your project using the
 *  TEST_MAIN macro specified in this file.
 *
 * Details:
 *  Autotest registers known test suites in a global list of known tests.
 *  The same test suite name may not be used twice.
 *  Using the main() macro function defined below, the run() function is
 *  called to loop through the list of registered tests and execute them.
 *
 * Options:
 *  Test result is by default output to console.
 *  To generate xml file output add "toxml" to the command line arguments.
 *  This will generate an xml file with test results for each testsuite.
 *
 */

#ifndef AUTOTEST_H
#define AUTOTEST_H

#include <QTest>
#include <QList>
#include <QString>
#include <QSharedPointer>

namespace AutoTest
{

/**
 * @brief TestList Test list type
 */
typedef QList<QObject*> TestList;

/**
 * @brief testList Get the list of known tests.
 * @return Reference to the singelton list instance of known tests.
 */
inline TestList& testList()
{
    static TestList list;
    return list;
}

/**
 * @brief findObject Check if a test object is already present in the list of tests suites.
 * @param object Object to check for in the list of test suites.
 * @return true if the object exists in the list of known tests.
 *         false otherwise.
 */
inline bool findObject(QObject* object)
{
    TestList& list = testList();

    if (list.contains(object))
    {
        return true;
    }

    foreach (QObject* test, list)
    {
        if (test->objectName() == object->objectName())
        {
            return true;
        }
    }

    return false;
}

/**
 * @brief addTest Adds a test suite to the list of known test suites.
 *                Existsing test suites will not be added.
 * @param object Test object
 */
inline void addTest(QObject* object)
{
    TestList& list = testList();
    if (!findObject(object))
    {
        list.append(object);
    }
}

/**
 * @brief run Run all test suites. For xml file output, specify "toxml" as command argument.
 * @param argc Executable command argument count
 * @param argv Executable command arguments
 * @return
 */
inline int run(int argc, char* argv[])
{
    int ret = 0;

    QCoreApplication app(argc, argv);
    bool writeToFile = app.arguments().contains("toxml");

    foreach (QObject* test, testList())
    {
        QStringList testCmd;
        QString className = test->metaObject()->className();
        QString xmlFile = className + QObject::tr(".xml");

        while(xmlFile.contains(":"))
        {
            xmlFile.replace(":","_");
        }

        if(writeToFile)
        {
           // testCmd << " " << "-xunitxml" << "-o" << xmlFile;
		   testCmd << " " << "-xml" << "-o" << xmlFile;
        }

        ret += QTest::qExec(test, testCmd);
    }
    return ret;
}

} // end namespace AutoTest

/**
 *  Template Test container class
 */
template <class T>
class Test
{
public:

    /**
     * @brief child Pointer to Test suite class
     */
    QSharedPointer<T> child;

    /**
     * @brief Test Create a test suite representation using the container template
     * @param name Name of test suite
     */
    explicit Test(const QString& name) : child(new T)
    {
        child->setObjectName(name);
        AutoTest::addTest(child.data());
    }
};

/**
  * DECLARE_TEST
  * Macro definition for adding a test suite to the list of known test suites
  */
#define DECLARE_TEST(className) static Test<className> t(#className);


/**
  * TEST_MAIN
  * Macro definition for the test main method for ease of use
  * Will launch all tests registered using the DECLARE_TEST macro
  */
#define TEST_MAIN \
        int main(int argc, char* argv[]) \
        { return AutoTest::run(argc, argv); }

 /**
   * TEST_TO_BE_IMPLEMENTED
   * Macro for ease of definitng a point of deliberate failure for unimplmented tests.
   */
#define TEST_TO_BE_IMPLEMENTED { QVERIFY(false); }

#endif // AUTOTEST_H



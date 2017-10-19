#ifndef MENUGENERATOR_H
#define MENUGENERATOR_H

#include "FileGeneratorBase.h"
#include "Project.h"
#include "QVector"
#include "QString"
#include "IconWidget.h"

class MenuGenerator : public FileGeneratorBase
{
public:
    explicit MenuGenerator(Project* project);
    ~MenuGenerator(void);

    // FileGeneratorBase interface
public:
    void OnGenerateFiles(void);

private:
    QString m_MenuConfigFilePath;
    LayoutDefinition& m_layout;
    BSPSpecification& m_bspSpecification;
    MenuDefinition* m_menuDefinition;

    QString m_nextSignalName;
    QString m_previousSignalName;
    QString m_enterSignalName;
    QString m_leaveSignalName;
    QMap<const MenuItem*, QString> m_uniqueMenuItemNames;

    int generateStringDBEntry(QString string);
    void generateMenuConfigFile(void);
    void generateFontHeaders(void);
    void addStaticMenuFiles(void);

    void appendMenuItemDeclarations(QStringList& sourceLines, const QList<MenuItem*>& menuItems);
    void appendMenuItemWidgetsDefinitions(QStringList& sourceLines, const QList<MenuItem*>& menuItems);
    void appendMenuItemDefinitions(QStringList& sourceLines, const QList<MenuItem*>& menuItems, bool wrapMenu);
    void appendMenuItemsNavigation(QStringList& sourceLines, const QList<MenuItem*>& menuItems, bool wrapMenu);
    static MenuItem* getPreviousMenuItem(MenuItem* currentItem, const QList<MenuItem*>& container);
    static MenuItem* getNextMenuItem(MenuItem* currentItem, const QList<MenuItem*>& container);

    QString getUniqueMenuItemName(const MenuItem* menuItem);

    void addWidgetIncludes(const QList<MenuItem*>& menuItems, QStringList& includeLines);
};


#endif // MENUGENERATOR_H

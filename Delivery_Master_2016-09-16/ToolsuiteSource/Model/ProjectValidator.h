#ifndef PROJECTVALIDATOR_H
#define PROJECTVALIDATOR_H

class DisplayArea;
class GCLSignalSink;
class Layout;
class MenuItem;
class Project;
class RootMenuItemContainer;
class QRegExp;

class ProjectValidator
{
public:
    struct layerAndGramUsage_t {
        quint32 layerCount;
        quint32 gramUsage;
    };

    explicit ProjectValidator(Project& project);

    bool ProjectIsValidForSimulator();
    bool ProjectIsValidForTarget();
    QString Errors() const;
    QString Warnings() const;
    static ProjectValidator::layerAndGramUsage_t CalculateLayerAndGramUsage(Project* project, Layout* layout);
    static void CheckNumberOfLayersNeededByMenu(const QList<MenuItem*>& menuItems, quint32& layersNeeded);
    static QRegExp ValidCIdentifierRegExp();

private:
    Project& m_project;
    QStringList m_errors;
    QStringList m_warnings;
    bool m_validateWidth;
    bool m_mandatorySinkUnmapped;
    quint8 m_allowedWidthMultiple;

    bool pcicFunctionsValid();
    bool functionBlocksContainLibraryOrSourceFilesForTarget(bool validateForSimulation);
    bool targetSourceFolderExist();
    bool layoutDefinitionIsValid();

    bool setupWidthLimitations();
    bool commonValidatorsAreValid();
    bool menuHasSubMenuItem();
    bool allMandatorySinksAreMapped();
    bool chromaKeyRangeIsValid();
    bool schedulerCycleTimesAreValid();
    bool diagnosticsValid();
    bool labelFontsAreValid();
    bool layoutsHaveUniqueNames();
    bool layoutsLayersAndMemoryUsagesAreOk();
    bool menuItemsAreUnique();
    bool selfValidation();

    bool widgetSizesAreValid();
    bool noBordersOnHWRenderedAreas();
    void checkMenuItemWidgetsValidSizeAndPos(const QList<MenuItem*>& menuItems, DisplayArea* area, bool& allWidgetSizesOk);

    void findMenuItemsRecursively(QList<MenuItem *> *itemList, MenuItem *beginningMenuItem);
    void findMenuItemsRecursively(QList<MenuItem *> *itemList, RootMenuItemContainer &rootMenuItem);

    int getFiles(const QDir &currentDir);

    bool findUnmappedSinkInMenu(GCLSignalSink *sink);
    bool findUnmappedSinkInMenuItem(MenuItem *item, GCLSignalSink *sink, QString menuItemLocation);
    bool findUnmappedSinkInCheckMessage(GCLSignalSink *sink);
};

#endif // PROJECTVALIDATOR_H

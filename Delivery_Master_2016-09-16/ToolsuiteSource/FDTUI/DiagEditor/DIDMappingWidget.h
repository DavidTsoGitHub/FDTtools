#ifndef DIDMAPPINGWIDGET_H
#define DIDMAPPINGWIDGET_H

#include "ProjectModel.h"
#include "DTOs/CellInformation.h"

#include <QTableWidget>
#include <QString>
#include <QStringList>
#include <QList>


class DIDMappingWidget: public QTableWidget
{
    Q_OBJECT

public:
    explicit DIDMappingWidget(QWidget *parent = 0);
    ~DIDMappingWidget();

    void PopulateTable(ProjectModel* projectModel);

private slots:
    void slot_afterCellChanged(int row, int column);
    void slot_didMappingWidgetCellDeleteButton_clicked(CellInformation &cellInformation);
    void slot_ComboBox_currentTextChanged(const QString &parameterName, CellInformation &cellInformation);
    void slot_functionBlockAdded(void);
    void slot_functionBlockRemoved();
signals:
    void signal_PropertiesChanged(const QModelIndex& projectIndex);

private:
    void setupDIDMappingTable(void);
    void setupSignals(void);
    bool rowIsEmpty(int row);
    void addDeleteCellButton(quint32 rowPosition);
    void addHeader(int column, QString header);
    void insertEmptyRow(void);
    void addMapping(quint16 key, QString value, quint16 row);

    ProjectModel* m_projectModel;
    QStringList m_parameterNameList;
    void cleanDIDMappingTable();
    void redrawDIDMappingTable();
    void removeDID(int row);
    QString getRowMasterText(quint32 row);
    DIDCALMapping* m_didCALMapping;
    quint16 getDIDForRow(int row);
    QList<quint16> m_localDIDList;
    QString convertUint16ToHexString(quint16 value);    
    void addDIDCALMapping(int row, quint16 did, QString calParam);
    void updateDIDCALMapping(quint16 did, QString calParam);
};

#endif // DIDMAPPINGWIDGET_H

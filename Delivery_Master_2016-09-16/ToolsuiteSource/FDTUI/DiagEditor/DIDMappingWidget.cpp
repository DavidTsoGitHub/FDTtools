#include "DIDMappingWidget.h"
#include "LabelCellComboBox.h"

#include <QMap>
#include <CellPushButton.h>

#define DELETEBUTTONCOLUMN 0
#define KEYCOLUMN 1
#define MAPPINGCOLUMN 2

DIDMappingWidget::DIDMappingWidget(QWidget* parent): QTableWidget(parent)
{
    setupDIDMappingTable();
    setupSignals();
}

DIDMappingWidget::~DIDMappingWidget()
{
}

void DIDMappingWidget::PopulateTable(ProjectModel *projectModel)
{
    m_localDIDList.clear();
    this->blockSignals(true);
    this->setRowCount(0);

    m_projectModel = projectModel;
    connect(m_projectModel, SIGNAL(signal_FunctionBlockAdded(void)), this, SLOT(slot_functionBlockAdded(void)));
    connect(m_projectModel, SIGNAL(signal_FunctionBlockRemoved(void)), this, SLOT(slot_functionBlockRemoved(void)));

    m_didCALMapping = m_projectModel->Project().GetDiagDefinition()->GetDidCalMapping();
    m_parameterNameList = m_projectModel->Project().GetAllCALParameters();
    m_parameterNameList.insert(0, "");

    QMapIterator<quint16, QString> it(m_didCALMapping->GetDIDMappings());
    quint16 row = 0;
    while (it.hasNext())
    {
        it.next();
        addMapping(it.key(), it.value(), row);
        m_localDIDList.append(it.key());
        ++row;
    }

    insertEmptyRow();
    this->blockSignals(false);
}

void DIDMappingWidget::setupDIDMappingTable()
{
    this->setColumnCount(3);
    this->setColumnWidth(DELETEBUTTONCOLUMN, 20);
    this->setColumnWidth(KEYCOLUMN, 100);
    this->setColumnWidth(MAPPINGCOLUMN, 300);
    this->verticalHeader()->setVisible(false);

    addHeader(0, "");
    addHeader(KEYCOLUMN, "DID");
    addHeader(MAPPINGCOLUMN, "CAL ID");

    insertEmptyRow();
}

void DIDMappingWidget::addHeader(int column, QString header)
{
    QTableWidgetItem* columnHeader = new QTableWidgetItem(header);
    columnHeader->setFlags(0);
    this->setHorizontalHeaderItem(column, columnHeader);
}

void DIDMappingWidget::setupSignals()
{
    connect(this, SIGNAL(cellChanged(int,int)), this, SLOT(slot_afterCellChanged(int, int)));
}

void DIDMappingWidget::addDeleteCellButton(quint32 rowPosition)
{
    CellPushButton* deleteButton = new CellPushButton(rowPosition, 0);
    deleteButton->setText("X");

    deleteButton->setStyleSheet("QPushButton {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #b8b7b7, stop: 1 #ff2b2b);}");
    connect(deleteButton, SIGNAL(signal_clicked(CellInformation&)), this, SLOT(slot_didMappingWidgetCellDeleteButton_clicked(CellInformation&)));

    this->setCellWidget(rowPosition, 0, deleteButton);
}

void DIDMappingWidget::insertEmptyRow()
{
    int row = this->rowCount();
    this->insertRow(row);

    LabelCellComboBox* combobox = new LabelCellComboBox(row, MAPPINGCOLUMN);
    combobox->addItems(m_parameterNameList);
    connect(combobox, SIGNAL(signal_currentIndexChanged(const QString&, CellInformation&)), this, SLOT(slot_ComboBox_currentTextChanged(const QString&, CellInformation&)));
    this->setItem(row, KEYCOLUMN, new QTableWidgetItem(""));
    this->setCellWidget(row, MAPPINGCOLUMN, combobox);
    this->setCurrentCell(row, 1);
}

void DIDMappingWidget::addMapping(quint16 key, QString value, quint16 row)
{
    this->insertRow(row);
    this->setItem(row, KEYCOLUMN, new QTableWidgetItem(QString("0x" + QString("%1").arg(key, 4, 16, QChar('0')).toUpper())));

    LabelCellComboBox* combobox = new LabelCellComboBox(row, MAPPINGCOLUMN);
    combobox->addItems(m_parameterNameList);
    this->setCellWidget(row,MAPPINGCOLUMN,combobox);

    combobox->setCurrentIndex(combobox->findText(value));
    connect(combobox, SIGNAL(signal_currentIndexChanged(const QString&, CellInformation&)), this, SLOT(slot_ComboBox_currentTextChanged(const QString&, CellInformation&)));
    addDeleteCellButton(row);
}

quint16 DIDMappingWidget::getDIDForRow(int row)
{
    bool status = false;
    if ((this->item(row, KEYCOLUMN))->text().isEmpty())
    {
        return 0;
    }

    quint16 did = this->item(row, KEYCOLUMN)->text().toUInt(&status, 16);

    if (status)
    {
        return did;
    }
    else
    {
        QMessageBox::warning(this, "Incorrect DID format", "The did must be in a hex format: '" + getRowMasterText(row) + "'\n\n", QMessageBox::Ok);
        return 0;
    }
}

QString DIDMappingWidget::convertUint16ToHexString(quint16 value)
{
    QString hexString = QString("0x" + QString("%1").arg(value, 4, 16, QChar('0')).toUpper());
    return hexString;
}

void DIDMappingWidget::slot_afterCellChanged(int row, int column)
{
    bool isDirty = false;
    if (row + 1 == this->rowCount())
    {
        quint16 did = getDIDForRow(row);

        if (did == 0)
        {
            this->blockSignals(true);
            this->item(row, KEYCOLUMN)->setText("");
            this->blockSignals(false);

            return;
        }

        if (m_localDIDList.contains(did))
        {
            QMessageBox::warning(this, "Add DID", "The DID already exists: '" + getRowMasterText(row) + "'\n\n", QMessageBox::Ok);
            this->blockSignals(true);
            this->item(row, KEYCOLUMN)->setText("");
            this->blockSignals(false);
            return;
        }

        insertEmptyRow();
        QString calParam = ((LabelCellComboBox*)(this->cellWidget(row, MAPPINGCOLUMN)))->currentText();
        addDeleteCellButton(row);

        if (column == KEYCOLUMN)
        {
            m_localDIDList.append(did);
            addDIDCALMapping(row, did, calParam);
            this->blockSignals(true);
            this->item(row, KEYCOLUMN)->setText(convertUint16ToHexString(did));
            this->blockSignals(false);
            isDirty = true;
        }
        else if (column == MAPPINGCOLUMN)
        {
            if (did)
            {
                updateDIDCALMapping(did, calParam);
                isDirty = true;
            }
        }
    }
    else
    {
        quint16 did = getDIDForRow(row);
        if (did == 0)
        {
            if (row < m_localDIDList.count())
            {
                this->blockSignals(true);
                this->item(row, KEYCOLUMN)->setText(convertUint16ToHexString(m_localDIDList[row]));
                this->blockSignals(false);
            }
            return;
        }

        if (m_localDIDList.contains(did))
        {
            QMessageBox::warning(this, "Add DID", "The DID already exists: '" + getRowMasterText(row) + "'\n\n", QMessageBox::Ok);
            this->blockSignals(true);
            this->item(row, KEYCOLUMN)->setText(convertUint16ToHexString(m_localDIDList[row]));
            this->blockSignals(false);
            return;
        }

        QString calParam = ((LabelCellComboBox*)(this->cellWidget(row, MAPPINGCOLUMN)))->currentText();

        if (column == KEYCOLUMN)
        {
            if (did != m_localDIDList[row])
            {
                addDIDCALMapping(row, did, calParam);
                this->item(row, KEYCOLUMN)->setText(convertUint16ToHexString(did));
                isDirty = true;
            }
        }
        else if (column == MAPPINGCOLUMN)
        {
            updateDIDCALMapping(did, calParam);
            isDirty = true;
        }
    }

    if (isDirty)
    {
        m_projectModel->SetDirty(true);
    }
}

bool DIDMappingWidget::rowIsEmpty(int row)
{
    QTableWidgetItem* keyItem = this->item(row, KEYCOLUMN);
    QTableWidgetItem* valueItem = this->item(row, MAPPINGCOLUMN);

    return (keyItem == NULL || keyItem->text().isEmpty()) && (valueItem == NULL || valueItem->text().isEmpty());
}

void DIDMappingWidget::cleanDIDMappingTable()
{
    this->clear();
    this->setRowCount(0);
    m_localDIDList.clear();
    setupDIDMappingTable();
}

void DIDMappingWidget::redrawDIDMappingTable()
{
    cleanDIDMappingTable();
    PopulateTable(m_projectModel);
}

void DIDMappingWidget::addDIDCALMapping(int row, quint16 did, QString calParam)
{
    m_didCALMapping->RemoveDID(m_localDIDList[row]); // Makes sure the DID isn't allready connected
    m_localDIDList[row] = did;
    m_didCALMapping->AddDID(did); // If the did exists it will be overwritten.
    m_didCALMapping->UpdateCALMapping(did, calParam);
    this->setCurrentCell(row, MAPPINGCOLUMN);
}

void DIDMappingWidget::updateDIDCALMapping(quint16 did, QString calParam)
{
    m_didCALMapping->UpdateCALMapping(did, calParam);
}

void DIDMappingWidget::removeDID(int row)
{
    bool status = false;
    m_didCALMapping->RemoveDID(this->item(row, KEYCOLUMN)->text().toUInt(&status, 16));
}

QString DIDMappingWidget::getRowMasterText(quint32 row)
{
    return this->item(row, KEYCOLUMN)->text();
}

void DIDMappingWidget::slot_didMappingWidgetCellDeleteButton_clicked(CellInformation& cellInformation)
{

    QMessageBox::StandardButton reply = QMessageBox::warning(this, "Delete DID", "You are about to delete the following DID: '" + getRowMasterText(cellInformation.GetRow()) + "'\n\nDo you want to proceed?", QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No)
    {
        return;
    }

    this->blockSignals(true);
    removeDID(cellInformation.GetRow());
    this->removeRow(cellInformation.GetRow());
    redrawDIDMappingTable();
    m_projectModel->SetDirty(true);
    this->blockSignals(false);
}

void DIDMappingWidget::slot_ComboBox_currentTextChanged(const QString &parameterName, CellInformation &cellInformation)
{
    int row = cellInformation.GetRow();
    quint16 did = getDIDForRow(row);

    if (did != 0)
    {
        m_didCALMapping->UpdateCALMapping(did, parameterName);
        m_projectModel->SetDirty(true);
    }
}

void DIDMappingWidget::slot_functionBlockAdded()
{
    redrawDIDMappingTable();
}

void DIDMappingWidget::slot_functionBlockRemoved()
{
    m_projectModel->SetDirty(true);
    redrawDIDMappingTable();
}

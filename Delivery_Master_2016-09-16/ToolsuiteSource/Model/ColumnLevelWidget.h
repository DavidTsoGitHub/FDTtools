#ifndef COLUMNLEVELWIDGET_H
#define COLUMNLEVELWIDGET_H

#include "UIWidgetBase.h"
#include "LayoutDefinition.h"

class ColumnLevelWidget : public UIWidgetBase
{
public:
    static const QString TypeName;

    explicit ColumnLevelWidget(LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);
    ColumnLevelWidget(quint32 uiWidgetID, LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);

    quint16 GetSpacing() const;
    void SetSpacing(const quint16& spacing);

    quint8 GetNumberOfBlocks() const;
    void SetNumberOfBlocks(const quint8& numberOfBlocks);

    quint8 GetMin() const;
    void SetMin(const quint8& min);

    quint8 GetMax() const;
    void SetMax(const quint8& max);

    quint8 GetMinBlockHeight() const;
    void SetMinBlockHeight(const quint8& minBlockHeight);

    quint8 GetMaxBlockHeight() const;
    void SetMaxBlockHeight(const quint8& maxBlockHeight);

private:
    typedef enum
    {
        SpacingColumn,
        NumberOfBlocksColumn,
        MinColumn,
        MaxColumn,
        MinBlockHeight,
        MaxBlockHeight,
        SinkColumn,
        SourceColumn,
        ColumnLevelWidgetColumnCount
    } columnLevelWidgetColumnIndex_t;

    LayoutDefinition& m_layoutDefinition;
    quint16 m_spacing;
    quint8 m_numberOfBlocks;
    quint8 m_min;
    quint8 m_max;
    quint8 m_minBlockHeight;
    quint8 m_maxBlockHeight;
    QString m_sinkDataType;
    QString m_sourceDataType;

    // UIWidgetBase interface
public:
    QString GetUIWidgetTypeName() const;
    GraphicsItemBase* GetGraphicsItem(bool drawChromaKeyColor);
    QString GetSecondaryColorPairForegroundHeader();
    QString GetSecondaryColorPairBackgroundHeader();

protected:
    UIWidgetBase* CloneThis(quint32 widgetID);
    QVariant GetWidgetPropertyData(int column) const;
    QString GetWidgetPropertyHeader(int column) const;
    void SetWidgetProperty(int column, QVariant value);
    DataType_t GetWidgetPropertyDataType(int column) const;
    int GetWidgetPropertyCount() const;
    QString WidgetPropertyToolTip(int column) const;
    QString LongDescription();


};

#endif // COLUMNLEVELWIDGET_H

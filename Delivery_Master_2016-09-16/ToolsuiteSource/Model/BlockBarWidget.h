#ifndef BLOCKBARWIDGET_H
#define BLOCKBARWIDGET_H

#include "UIWidgetBase.h"
#include "LayoutDefinition.h"

class BlockBarWidget : public UIWidgetBase
{
public:
    static const QString TypeName;

    explicit BlockBarWidget(LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);
    BlockBarWidget(quint32 uiWidgetID, LayoutDefinition &layoutDefinition, ProjectObject* parent = 0);

    quint16 GetSpacing() const;
    void SetSpacing(const quint16& spacing);

    quint8 GetNumberOfBlocks() const;
    void SetNumberOfBlocks(const quint8& numberOfBlocks);

    quint32 GetMin() const;
    void SetMin(const quint32& min);

    quint32 GetMax() const;
    void SetMax(const quint32& max);

private:
    typedef enum
    {
        SpacingColumn,
        NumberOfBlocksColumn,
        MinColumn,
        MaxColumn,
        SinkColumn,
        BlockBarWidgetColumnCount
    } blockBarColumnIndex_t;

    LayoutDefinition& m_layoutDefinition;
    quint16 m_spacing;
    quint8 m_numberOfBlocks;
    quint32 m_min;
    quint32 m_max;

    // UIWidgetBase interface
public:
    QString GetUIWidgetTypeName() const;
    GraphicsItemBase* GetGraphicsItem(bool drawChromaKeyColor = true);
    QString LongDescription();

protected:
    UIWidgetBase* CloneThis(quint32 widgetID);
    QVariant GetWidgetPropertyData(int column) const;
    QString GetWidgetPropertyHeader(int column) const;
    void SetWidgetProperty(int column, QVariant value);
    DataType_t GetWidgetPropertyDataType(int column) const;
    int GetWidgetPropertyCount() const;
    QString WidgetPropertyToolTip(int column) const;
};


#endif // BLOCKBARWIDGET_H

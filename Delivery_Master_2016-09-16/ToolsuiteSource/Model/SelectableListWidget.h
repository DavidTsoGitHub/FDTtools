#ifndef SELECTABLELISTWIDGET_H
#define SELECTABLELISTWIDGET_H

#include "UIWidgetBase.h"

class LayoutDefinition;
enum class ColorComponent;

class SelectableListWidget : public UIWidgetBase
{
public:
    static const QString TypeName;

    explicit SelectableListWidget(LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);
    explicit SelectableListWidget(quint32 widgetID, LayoutDefinition& layoutDefinition, ProjectObject* parent = 0);
    ~SelectableListWidget();

    bool GetSelectable(void) const;
    void SetSelectable(bool selectable);

    bool GetShowNumbering(void) const;
    void SetShowNumbering(bool showNumbering);

    quint8 GetDisplayItemCount(void) const;
    void SetDisplayItemCount(quint8 count);

    void SetSelectedItemFGColorComponent(ColorComponent colorComponent, quint8 value);
    QColor GetSelectedItemFGColor(void) const;

    void SetSelectedItemBGIsTransparent(bool transparent);
    bool GetSelectedItemBGIsTransparent(void) const;

    void SetSelectedItemBGColorComponent(ColorComponent colorComponent, quint8 value);
    QColor GetSelectedItemBGColor(void) const;

    TextAlignment_t GetTextAlignment() const;
    QString GetTextAlignmentEnumString(void) const;
    void SetTextAlignment(const TextAlignment_t& textAlignment);

private:
    typedef enum
    {
        SelectableColumn,
        ShowNumberingColumn,
        DisplayItemCountColumn,
        TextAlignmentColumn,
        CurrentValueSinkColumn,
        LabelMappingsSinkColumn,
        SelectedValueSourceColumn,
        selectableListWidgetColumnCount
    } selectableListWidgetColumns_t;

    LayoutDefinition& m_layoutDefinition;
    bool m_selectable;
    bool m_showNumbering;
    quint8 m_displayItemCount;
    TextAlignment_t m_textAlignment;

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
    int GetWidgetPropertyCount() const;
    DataType_t GetWidgetPropertyDataType(int column) const;
    QString WidgetPropertyToolTip(int column) const;
    QString LongDescription();
};

#endif // SELECTABLELISTWIDGET_H

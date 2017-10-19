#ifndef LABELMAPPINGSPUSHBUTTON_H
#define LABELMAPPINGSPUSHBUTTON_H

class LabelMappings;
class QPushButton;
class QString;
class QWidget;

class LabelMappingsPushButton : public QPushButton
{
    Q_OBJECT
public:
    LabelMappingsPushButton(LabelMappings* labelMappings, QString text, QWidget* parent = 0);
    ~LabelMappingsPushButton();

signals:
    void signal_Clicked(LabelMappings* labelMappings);

private:
    LabelMappings* m_labelMappings;
};

#endif // LABELMAPPINGSPUSHBUTTON_H

#ifndef SIGNALNAMEWIDGET_H
#define SIGNALNAMEWIDGET_H

class GCLSignalSink;
class GCLSignalSource;
class QWidget;

class SignalNameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SignalNameWidget(const GCLSignalSink* signal, QWidget* parent = 0);
    explicit SignalNameWidget(const GCLSignalSource* source, QWidget* parent = 0);

private:
    QFont m_signalNameFont;
    QFont m_signalTypeFont;

    void setupWidget();
    QHBoxLayout* thisLayout();
};

#endif // SIGNALNAMEWIDGET_H

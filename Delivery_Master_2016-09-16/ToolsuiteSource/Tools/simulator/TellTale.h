#ifndef TELLTALE_H
#define TELLTALE_H

#include <QLabel>

class TellTale : public QLabel
{

public:
    explicit TellTale(QString resourcePath, QColor activeColor = QColor("blue"), QWidget *parent = 0);

    void SetTellTaleActive(bool active);

private:

    QPixmap m_inactivePixmap;
    QPixmap m_activePixmap;
};

#endif // TELLTALE_H

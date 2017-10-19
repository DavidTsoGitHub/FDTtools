#ifndef IMAGEPREVIEWPROPERTIESWIDGET_H
#define IMAGEPREVIEWPROPERTIESWIDGET_H

#include "ProjectObjectPropertiesWidget.h"

class Icon;
class Image;
class Project;

class QLabel;
class QLineEdit;
class QPixmap;
class QString;
class QWidget;

class ImagePreviewPropertiesWidget : public ProjectObjectPropertiesWidget
{
    Q_OBJECT

public:
    explicit ImagePreviewPropertiesWidget(Project* project, QWidget* parent = 0);

    void SetIcon(Icon* icon);
    void SetImage(Image* image);
signals:

private:
    Project* m_project;

    Icon* m_icon;
    Image* m_image;
    QLineEdit* m_imageName;
    QLabel* m_imageLabel;
    QPixmap* m_pixmap;
    QString m_fileName;
    QString m_filePath;

    void loadProperties(Image* image);
    void loadProperties(Icon* icon);

protected:
    virtual void resizeEvent(QResizeEvent* event);
    virtual void showEvent(QShowEvent* event);

    //ProjectObjectPropertiesWidget interface
    void beforePopulateSignals();
    void afterPopulateUIHook();
};

#endif // IMAGEPREVIEWPROPERTIESWIDGET_H

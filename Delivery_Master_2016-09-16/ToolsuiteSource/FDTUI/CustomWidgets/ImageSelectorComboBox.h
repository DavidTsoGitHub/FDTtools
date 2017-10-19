#ifndef IMAGESELECTORCOMBOBOX_H
#define IMAGESELECTORCOMBOBOX_H

class FunctionBlock;
class ImageDefinition;
class Project;
class QComboBox;
class QString;

class ImageSelectorComboBox : public QComboBox
{
    Q_OBJECT

public:
    ImageSelectorComboBox(FunctionBlock* functionBlock, QString imageResourceName, Project* project, QWidget* parent = 0);

private slots:
    void populate();
    void slot_currentIndexChanged(QString);

private:
    const ImageDefinition& m_imageDefinition;
    QString m_imageResourceName;
    FunctionBlock* m_functionBlock;

    bool m_signalsEnabled;
    void projectUpdated();
};

#endif // IMAGESELECTORCOMBOBOX_H

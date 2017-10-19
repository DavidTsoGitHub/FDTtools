#ifndef IMAGE
#define IMAGE

#include "ProjectObject.h"

class ProjectObjectVisitor;

const quint32 UndefinedImageID = std::numeric_limits<quint32>::max();

class Image : public ProjectObject
{
public:
    static Image UndefinedImage;

    explicit Image();

    quint32 GetID(void) const;
    void SetID(const quint32 id);

    const QString GetName(void) const;
    void SetName(const QString& name);

    const QString GetPath(void) const;
    void SetPath(const QString& path);

    const QString GetDescription(void) const;
    void SetDescription(const QString& path);

    quint32 GetWidth(void) const;
    void SetWidth(quint32 width);

    quint32 GetHeight(void) const;
    void SetHeight(quint32 height);

    quint32 GetSize(void) const;

    const QString GetFileName() const;

    bool GetStoreOnExternalFlash(void) const;
    void SetStoreOnExternalFlash(bool state);

    bool GetUseForNonAnimations(void) const;
    void SetUseForNonAnimations(bool state);

    bool GetUseForAnimations(void) const;
    void SetUseForAnimations(bool state);

private:
    quint32 m_id;
    QString m_name;
    QString m_filePath;
    QString m_description;
    quint32 m_width;
    quint32 m_height;
    bool m_storeOnExternalFlash;
    bool m_useForNonAnimations;
    bool m_useForAnimations;

    // ProjectObject interface
protected:
    void AcceptVisitor(ProjectObjectVisitor &visitor) override;
};

#endif // IMAGE


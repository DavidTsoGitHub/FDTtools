#ifndef SERIALIZERBASE_H
#define SERIALIZERBASE_H

class QString;

class SerializerBase
{
public:
    virtual QString Serialize(void) = 0;
    const char* GetEncoding(void) const;

protected:
    explicit SerializerBase(const char* encoding);
    virtual ~SerializerBase(void);

private:
    const char* m_encoding;

};

#endif // SERIALIZERBASE_H

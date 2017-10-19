#ifndef BSPMEMORY
#define BSPMEMORY

class BSPMemory
{
public:
    BSPMemory();

    void SetInternalMemorySize(quint64 size);
    quint32 GetInternalMemorySize() const;

    void SetExternalMemorySize(quint32 size);
    quint32 GetExternalMemorySize() const;

    void SetExternalMemoryAddress(quint32 address);
    quint32 GetExternalMemoryAddress() const;

private:
    quint32 m_internalMemorySize;
    quint32 m_externalMemorySize;
    quint32 m_externalMemoryAddress;

};

#endif // BSPMEMORY


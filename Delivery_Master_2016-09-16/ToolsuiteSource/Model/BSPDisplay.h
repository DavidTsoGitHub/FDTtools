#ifndef BSPDISPLAY_H
#define BSPDISPLAY_H

class QString;
template <class Key, class T> class QMap;
class IBSP_DCU_BPPModes;

class BSPDisplay
{
public:
    typedef enum
    {
        Normal,
        CounterClockWise
    } BSPDisplayOrientation;

    typedef enum
    {
        SW,
        HW
    } BSPRenderingType;

    BSPDisplay();

    quint16 GetWidth() const;
    void SetWidth(const quint16& value);

    quint16 GetHeight() const;
    void SetHeight(const quint16& value);

    quint32 GetAvailableMemory() const;
    void SetAvailableMemory(const quint32& value);

    bool GetUseGRAM() const;
    void SetUseGRAM(bool value);

    quint16 GetNumberOfLayers() const;
    void SetNumberOfLayers(const quint16& value);

    BSPDisplayOrientation GetRenderingOrientation() const;
    void SetRenderingOrientation(const BSPDisplayOrientation& value);

    BSPRenderingType GetRenderingType() const;
    void SetRenderingType(const BSPRenderingType& value);

    quint8 GetBpp() const;
    void SetBpp(const quint8& value);

    IBSP_DCU_BPPModes* GetBSPDCUBPPModes() const;
    void SetBSPDCUBPPModes(IBSP_DCU_BPPModes* bspDCUBPPModes);

private:
    quint16 width;
    quint16 height;
    quint32 availableMemory;
    bool useGRAM;
    quint16 numberOfLayers;
    BSPDisplayOrientation renderingOrientation;
    BSPRenderingType renderingType;
    quint8 bpp;
    IBSP_DCU_BPPModes* m_bspDCUBPPModes;
};

#endif // BSPDISPLAY_H


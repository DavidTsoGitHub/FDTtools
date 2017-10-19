#include "BSPDisplay.h"

BSPDisplay::BSPDisplay() :
    m_bspDCUBPPModes(NULL)
{
}

BSPDisplay::BSPDisplayOrientation BSPDisplay::GetRenderingOrientation() const
{
    return renderingOrientation;
}

void BSPDisplay::SetRenderingOrientation(const BSPDisplayOrientation& value)
{
    renderingOrientation = value;
}

BSPDisplay::BSPRenderingType BSPDisplay::GetRenderingType() const
{
    return renderingType;
}

void BSPDisplay::SetRenderingType(const BSPRenderingType& value)
{
    renderingType = value;
}

quint8 BSPDisplay::GetBpp() const
{
    return bpp;
}

void BSPDisplay::SetBpp(const quint8& value)
{
    bpp = value;
}

IBSP_DCU_BPPModes* BSPDisplay::GetBSPDCUBPPModes() const
{
    return m_bspDCUBPPModes;
}

void BSPDisplay::SetBSPDCUBPPModes(IBSP_DCU_BPPModes* bspDCUBPPModes)
{
    m_bspDCUBPPModes = bspDCUBPPModes;
}

quint16 BSPDisplay::GetNumberOfLayers() const
{
    return numberOfLayers;
}

void BSPDisplay::SetNumberOfLayers(const quint16& value)
{
    numberOfLayers = value;
}

bool BSPDisplay::GetUseGRAM() const
{
    return useGRAM;
}

void BSPDisplay::SetUseGRAM(bool value)
{
    useGRAM = value;
}

quint32 BSPDisplay::GetAvailableMemory() const
{
    return availableMemory;
}

void BSPDisplay::SetAvailableMemory(const quint32& value)
{
    availableMemory = value;
}

quint16 BSPDisplay::GetHeight() const
{
    return height;
}

void BSPDisplay::SetHeight(const quint16& value)
{
    height = value;
}

quint16 BSPDisplay::GetWidth() const
{
    return width;
}

void BSPDisplay::SetWidth(const quint16& value)
{
    width = value;
}

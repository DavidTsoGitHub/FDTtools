#include "BSPMapping.h"

const quint8 BSPMapping::UnknownIndex = 0xFF;

BSPMapping::BSPMapping()
{

}

quint8 BSPMapping::GetIndexFromSignalName(QString signalName) const
{
    foreach (IndexToSignalMapping* indexToSignalMapping, IndexToSignalMappings)
    {
        if (indexToSignalMapping->GCLSignal == signalName)
        {
            return indexToSignalMapping->ID;
        }
    }

    return BSPMapping::UnknownIndex;
}

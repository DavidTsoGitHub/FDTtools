#include "BSPMappingTests.h"
#include "BSPMapping.h"



void BSPMappingTests::GetIndexFromSignalName_KnownSignal_ReturnsIndex()
{
    // Arrange
    const QString signalName("Signal1");
    const quint8 index = 3;
    BSPMapping target;
    IndexToSignalMapping mapping;
    mapping.ID = index;
    mapping.GCLSignal = signalName;
    target.IndexToSignalMappings.append(&mapping);

    // Act
    quint8 result = target.GetIndexFromSignalName(signalName);

    // Assert
    QCOMPARE(result, index);
}

void BSPMappingTests::GetIndexFromSignalName_UnknownSignal_ReturnsUnknownIndex()
{
    // Arrange
    const QString signalName("Signal1");
    const QString unknownSignalName("Signal2");
    const quint8 index = 3;
    BSPMapping target;
    IndexToSignalMapping mapping;
    mapping.ID = index;
    mapping.GCLSignal = signalName;
    target.IndexToSignalMappings.append(&mapping);

    // Act
    quint8 result = target.GetIndexFromSignalName(unknownSignalName);

    // Assert
    QCOMPARE(result, BSPMapping::UnknownIndex);
}

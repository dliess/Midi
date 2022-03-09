#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "RtMidiAdaptTypes.h"

using namespace rtmidiadapt;

TEST(RtMidiAdaptTypesTest, start)
{
    DeviceOnUsbPort devOnUsbPort("Ableton Push 2:Ableton Push 2 Live Port 32a3:11d4");
    GTEST_ASSERT_EQ(devOnUsbPort.getDeviceName(), "Ableton Push 2");
    GTEST_ASSERT_EQ(devOnUsbPort.getMidiPort(), "Ableton Push 2 Live Port");
    GTEST_ASSERT_EQ(devOnUsbPort.getFullMidiPort(), "Ableton Push 2:Ableton Push 2 Live Port");
    GTEST_ASSERT_EQ(devOnUsbPort.getUsbPortName(), "32a3:11d4");
}
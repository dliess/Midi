#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "TransportHandling.h"

using namespace midi;

class TransportHandlingTester : public TransportHandling<TransportHandlingTester>
{
public:
    TransportHandlingTester()
    {
        registerStartedChangedCb(this, [this](bool started){
            startedChangedCb(started);
        });
        registerTransportMaskChangedCb(this, [this](bool masked){
            transportMaskChangedCb(masked);
        });
    }
    MOCK_METHOD(bool, sendStart, ());
    MOCK_METHOD(bool, sendStop, ());

    MOCK_METHOD(void, startedChangedCb, (bool started));
    MOCK_METHOD(void, transportMaskChangedCb, (bool masked));
};

TEST(TransportHandlingTest, start)
{
    TransportHandlingTester tester;
    EXPECT_CALL(tester, sendStart());
    EXPECT_CALL(tester, startedChangedCb(true));
    tester.start();
    tester.unregisterStartedChangedCb(&tester);
}

TEST(TransportHandlingTest, startMasked)
{
    TransportHandlingTester tester;

    EXPECT_CALL(tester, sendStart()).Times(0);
    EXPECT_CALL(tester, sendStop()).Times(0);
    EXPECT_CALL(tester, startedChangedCb(true)).Times(0);
    EXPECT_CALL(tester, transportMaskChangedCb(true));

    tester.setTransportMasked(true);
    tester.start();
}

TEST(TransportHandlingTest, unregisterTest)
{
    TransportHandlingTester tester;
    EXPECT_CALL(tester, sendStart());
    EXPECT_CALL(tester, startedChangedCb(true)).Times(0);
    tester.unregisterStartedChangedCb(&tester);
    tester.start();
}

TEST(TransportHandlingTest, stop)
{
    TransportHandlingTester tester;
    EXPECT_CALL(tester, sendStop()).Times(0);
    tester.stop();
}

TEST(TransportHandlingTest, toggleStartStop)
{
    TransportHandlingTester tester;
    EXPECT_CALL(tester, sendStart());
    EXPECT_CALL(tester, sendStop());
    EXPECT_CALL(tester, startedChangedCb(true));
    EXPECT_CALL(tester, startedChangedCb(false));
    tester.toggleStartStop();
    tester.toggleStartStop();
}


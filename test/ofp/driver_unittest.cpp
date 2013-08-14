#include "ofp/unittest.h"
#include "ofp/driver.h"

using namespace ofp;

class MockChannelListener : public ChannelListener {
public:
    void onChannelUp(Channel *channel) override
    {
    }
    void onChannelDown(Channel *channel) override
    {
    }
    void onMessage(const Message *message) override;

    void onException(const Exception *error) override
    {
    }
};


void MockChannelListener::onMessage(const Message *message)
{
}


TEST(driver, test)
{
    Driver driver;

    driver.listen(Driver::Controller, IPv6Address{}, Driver::DefaultPort,
                  ProtocolVersions{}, []{
        return new MockChannelListener;
    });

    //driver.run();
}

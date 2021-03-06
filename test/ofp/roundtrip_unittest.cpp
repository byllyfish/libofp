// Copyright (c) 2015-2018 William W. Fisher (at gmail dot com)
// This file is distributed under the MIT License.

#include "ofp/ofp.h"
#include "ofp/unittest.h"

using namespace ofp;

const UInt16 kTestingPort = 6666;

class TestController : public ChannelListener {
 public:
  TestController() {
    log_debug("TestController constructed");
    ++GLOBAL_controllerCount;
  }

  ~TestController() override {
    log_debug("TestController destroyed");
    --GLOBAL_controllerCount;
  }

  void onChannelUp(Channel *channel) override {
    ++GLOBAL_controllerCount;
    log_debug("TestController::onChannelUp",
              std::make_pair("connid", channel->connectionId()));
    channel_ = channel;

    DatapathID dpid{0x1234, MacAddress{"A1:B2:C3:D4:E5:F6"}};
    EXPECT_EQ(dpid, channel->datapathId());
    EXPECT_EQ(0, channel->auxiliaryId());

    // TODO(bfish): Check DefaultHandshake parameters.
    channel->driver()->stop(1000_ms);

    if (GLOBAL_shutdownCount > 0) {
      --GLOBAL_shutdownCount;
      channel->shutdown();
    }
  }

  void onChannelDown(Channel *channel) override {
    --GLOBAL_controllerCount;
    log_debug("TestController::onChannelDown",
              std::make_pair("connid", channel->connectionId()));
  }

  void onMessage(Message *message) override {
    log_debug("TestController::onMessage",
              std::make_pair("connid", message->source()->connectionId()));
    EXPECT_EQ(OFPT_FEATURES_REPLY, message->type());
  }

  static ChannelListener *factory() { return new TestController; }
  static int GLOBAL_controllerCount;
  static int GLOBAL_shutdownCount;

 private:
  Channel *channel_ = nullptr;
};

class TestAgent : public ChannelListener {
 public:
  TestAgent() {
    ++GLOBAL_agentCount;
    log_debug("TestAgent constructed");
  }

  ~TestAgent() override {
    --GLOBAL_agentCount;
    log_debug("TestAgent destroyed");
  }

  void onChannelUp(Channel *channel) override {
    ++GLOBAL_agentCount;
    log_debug("TestAgent::onChannelUp",
              std::make_pair("connid", channel->connectionId()));

    // When agent channel comes up, we expect the datapathId to be all zeros.
    DatapathID dpid;
    EXPECT_EQ(dpid, channel->datapathId());
    EXPECT_EQ(0, channel->auxiliaryId());
    EXPECT_LT(0, channel->version());
    EXPECT_GE(OFP_VERSION_LAST, channel->version());
  }

  void onChannelDown(Channel *channel) override {
    --GLOBAL_agentCount;
    log_debug("TestAgent::onChannelDown",
              std::make_pair("connid", channel->connectionId()));
  }

  void onMessage(Message *message) override {
    log_debug("TestAgent::onMessage",
              std::make_pair("connid", message->source()->connectionId()));

    if (message->type() == OFPT_FEATURES_REQUEST) {
      DatapathID dpid{0x1234, MacAddress{"A1:B2:C3:D4:E5:F6"}};
      FeaturesReplyBuilder reply{message->xid()};
      reply.setDatapathId(dpid);
      reply.send(message->source());

    } else if (message->type() == OFPT_MULTIPART_REQUEST &&
               message->subtype() == OFPMP_PORT_DESC) {
      PortList ports;  // empty for now

      MultipartReplyBuilder reply;
      reply.setReplyType(OFPMP_PORT_DESC);
      reply.setReplyBody(ports.data(), ports.size());
      reply.send(message->source());
    }
  }

  static ChannelListener *factory() { return new TestAgent; }
  static int GLOBAL_agentCount;
  static int GLOBAL_auxCount;
};

int TestController::GLOBAL_controllerCount = 0;
int TestController::GLOBAL_shutdownCount = 0;
int TestAgent::GLOBAL_agentCount = 0;
int TestAgent::GLOBAL_auxCount = 0;

TEST(roundtrip, basic_test) {
  // Before we start, there are no controller or agent listeners.
  TestController::GLOBAL_controllerCount = 0;
  TestAgent::GLOBAL_agentCount = 0;

  {
    Driver driver;
    IPv6Address localhost{"127.0.0.1"};

    std::error_code err1;
    (void)driver.listen(ChannelOptions::FEATURES_REQ, 0,
                        IPv6Endpoint{localhost, kTestingPort},
                        ProtocolVersions::All, TestController::factory, err1);
    // There should be no error on listen, unless the port is in use.
    EXPECT_FALSE(err1);

    (void)driver.connect(
        ChannelOptions::NONE, 0, IPv6Endpoint{localhost, kTestingPort},
        ProtocolVersions::All, TestAgent::factory,
        [](Channel *, std::error_code err) { EXPECT_FALSE(err); });

    // The driver will run until the Controller shuts it down.
    driver.run();

    EXPECT_EQ(2, TestController::GLOBAL_controllerCount);
    EXPECT_EQ(2, TestAgent::GLOBAL_agentCount);
  }

  // Destruction is complete when Driver goes out of scope.
  EXPECT_EQ(0, TestController::GLOBAL_controllerCount);
  EXPECT_EQ(0, TestAgent::GLOBAL_agentCount);
}

TEST(roundtrip, shutdown_test) {
  TestController::GLOBAL_controllerCount = 0;
  TestAgent::GLOBAL_agentCount = 0;

  {
    Driver driver;
    IPv6Address localhost{"127.0.0.1"};

    std::error_code err1;
    (void)driver.listen(ChannelOptions::FEATURES_REQ, 0,
                        IPv6Endpoint{localhost, kTestingPort},
                        ProtocolVersions::All, TestController::factory, err1);

    EXPECT_FALSE(err1);

    (void)driver.connect(
        ChannelOptions::NONE, 0, IPv6Endpoint{localhost, kTestingPort},
        ProtocolVersions::All, TestAgent::factory,
        [](Channel *, std::error_code err) { EXPECT_FALSE(err); });

    TestController::GLOBAL_shutdownCount = 1;
    driver.run();

    EXPECT_EQ(0, TestController::GLOBAL_shutdownCount);
    EXPECT_EQ(0, TestController::GLOBAL_controllerCount);
    EXPECT_EQ(0, TestAgent::GLOBAL_agentCount);
  }

  // Destruction is complete when Driver goes out of scope.
  EXPECT_EQ(0, TestController::GLOBAL_controllerCount);
  EXPECT_EQ(0, TestAgent::GLOBAL_agentCount);
}

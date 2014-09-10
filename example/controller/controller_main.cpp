#include "ofp/ofp.h"
#include "simplechannellistener.h"
#include <iostream>

using namespace ofp;

int main(int argc, char **argv) {
  log::setOutputStream(&std::cerr);

  Driver driver;

  std::error_code err = driver.listen(
      ChannelMode::Controller, IPv6Endpoint{OFP_DEFAULT_PORT}, ProtocolVersions::All,
      []() { return new controller::SimpleChannelListener; });

  driver.run();

  if (err) {
    log::error("Error running controller:", err);
    return 1;
  }

  return 0;
}

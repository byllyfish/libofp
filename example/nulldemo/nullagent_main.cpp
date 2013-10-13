#include "ofp.h"
#include "nullagent.h"
#include <iostream>
#include <vector>

using namespace ofp;

int main(int argc, const char **argv)
{
    std::vector<std::string> args{argv + 1, argv + argc};

    IPv6Address addr{};
    ProtocolVersions version{};
    if (!args.empty()) {
        addr = IPv6Address{args[0]};
        if (args.size() == 2) {
            int num = std::stoi(args[1]);
            version = ProtocolVersions{UInt8_narrow_cast(num)};
        }
    }

    ofp::log::set(&std::cerr);

    EnetAddress enetAddr{"01-02-03-04-05-00"};

    Features features;
    features.setDatapathId(DatapathID{0, enetAddr});
    features.setBufferCount(1);
    features.setTableCount(1);
    features.setCapabilities(0);

    Driver driver;
    
    if (addr.valid()) {
        auto result =
            driver.connect(Driver::Agent, &features, IPv6Endpoint{addr, OFP_DEFAULT_PORT},
                           version, NullAgent::Factory);
        result.done([](Exception ex) {
            // This will not be called, unless `addr` is invalid; agent will keep
            // retrying the connection.
            std::cout << "Result: " << ex << '\n';
        });

    } else {
        auto result =
            driver.listen(Driver::Agent, &features, IPv6Endpoint{OFP_DEFAULT_PORT},
                          version, NullAgent::Factory);

        result.done([](Exception ex) {
            // This may be called if port is already in use.
            std::cout << "Result: " << ex << '\n';
        });
    }

    driver.run();
}

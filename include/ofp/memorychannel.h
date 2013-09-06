#ifndef OFP_MEMORYCHANNEL_H
#define OFP_MEMORYCHANNEL_H

#include "ofp/writable.h"
#include "ofp/bytelist.h"
#include "ofp/padding.h"
#include "ofp/constants.h"

namespace ofp { // <namespace ofp>

class MemoryChannel : public Writable {
public:
    MemoryChannel(UInt8 version = OFP_VERSION_LAST) : version_{version}
    {
    }

    const UInt8 *data() const
    {
        assert(flushed_);
        return buf_.data();
    }

    size_t size() const
    {
        return buf_.size();
    }

    UInt8 version() const override
    {
        return version_;
    }

    void write(const void *data, size_t length) override
    {
        buf_.add(data, length);
    }

    void flush() override
    {
        flushed_ = true;
    }

    UInt32 nextXid() override
    {
        return nextXid_++;
    }

    template <class MesgBuilderType>
    static ByteList serialize(MesgBuilderType &msg, UInt8 version)
    {
        MemoryChannel writer{version};
        msg.send(&writer);
        return writer.buf_;
    }

private:
    ByteList buf_;
    UInt32 nextXid_ = 1;
    UInt8 version_;
    bool flushed_ = false;
    Padding<2> pad_;
};

} // </namespace ofp>

#endif // OFP_MEMORYCHANNEL_H

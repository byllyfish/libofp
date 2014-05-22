#include "ofp/mpexperimenter.h"
#include "ofp/multipartrequest.h"
#include "ofp/writable.h"

using namespace ofp;

const MPExperimenter *MPExperimenter::cast(const MultipartRequest *req) {
    return req->body_cast<MPExperimenter>();
}

ByteRange MPExperimenter::expData() const {
    // N.B. obtain length of multipart section from MultipartRequest header. 
    // (Request and Reply both have same format.)
    const MultipartRequest *req = MultipartRequest::msg_cast(this);
    assert(req);
    assert(req->requestBodySize() >= sizeof(MPExperimenter));

    return ByteRange{BytePtr(this) + sizeof(MPExperimenter), req->requestBodySize() - sizeof(MPExperimenter)};
}

void MPExperimenterBuilder::write(Writable *channel) {
    channel->write(&msg_, sizeof(msg_));
    channel->write(data_.data(), data_.size());
    channel->flush();
}
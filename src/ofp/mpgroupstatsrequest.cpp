#include "ofp/mpgroupstatsrequest.h"
#include "ofp/multipartrequest.h"
#include "ofp/writable.h"

using namespace ofp;

const MPGroupStatsRequest *MPGroupStatsRequest::cast(
    const MultipartRequest *req) {
  return req->body_cast<MPGroupStatsRequest>();
}

bool MPGroupStatsRequest::validateInput(Validation *context) const {
  size_t length = context->lengthRemaining();
  return (length == sizeof(MPGroupStatsRequest));
}

void MPGroupStatsRequestBuilder::write(Writable *channel) {
  channel->write(&msg_, sizeof(msg_));
  channel->flush();
}

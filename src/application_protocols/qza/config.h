#pragma once

#include "src/meta_protocol_proxy/codec/factory.h"
#include "src/application_protocols/qza/qza_codec.pb.h"
#include "src/application_protocols/qza/qza_codec.pb.validate.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MetaProtocolProxy {
namespace Qza {

class QzaCodecConfig
    : public MetaProtocolProxy::CodecFactoryBase<aeraki::meta_protocol::codec::QzaCodec> {
public:
  QzaCodecConfig() : CodecFactoryBase("aeraki.meta_protocol.codec.qza") {}
  MetaProtocolProxy::CodecPtr createCodec(const Protobuf::Message& config) override;
};

} // namespace Qza
} // namespace MetaProtocolProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy

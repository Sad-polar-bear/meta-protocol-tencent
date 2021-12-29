#include "envoy/registry/registry.h"

#include "src/meta_protocol_proxy/codec/factory.h"
#include "src/application_protocols/qza/config.h"
#include "src/application_protocols/qza/qza_codec.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MetaProtocolProxy {
namespace Qza {

MetaProtocolProxy::CodecPtr QzaCodecConfig::createCodec(const Protobuf::Message&) {
  return std::make_unique<Qza::QzaCodec>();
};

/**
 * Static registration for the trpc codec. @see RegisterFactory.
 */
REGISTER_FACTORY(QzaCodecConfig, MetaProtocolProxy::NamedCodecConfigFactory);

} // namespace Qza
} // namespace MetaProtocolProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy

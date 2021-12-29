#pragma once

#include "envoy/buffer/buffer.h"
#include "envoy/common/optref.h"
#include "envoy/common/pure.h"

#include "common/buffer/buffer_impl.h"
#include "common/common/logger.h"

#include "src/meta_protocol_proxy/codec/codec.h"
#include "src/application_protocols/qza/protocol.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MetaProtocolProxy {
namespace Qza {

enum class QzaDecodeStatus {
  DecodeHeader,
  DecodePayload,
  DecodeDone,
  WaitForData,
};

/**
 * Codec for Qza protocol.
 */
class QzaCodec : public MetaProtocolProxy::Codec,
                  public Logger::Loggable<Logger::Id::misc> {
public:
  QzaCodec() {};
  ~QzaCodec() override = default;

  MetaProtocolProxy::DecodeStatus decode(Buffer::Instance& buffer,
                                         MetaProtocolProxy::Metadata& metadata) override;
  void encode(const MetaProtocolProxy::Metadata& metadata,
              const MetaProtocolProxy::Mutation& mutation, Buffer::Instance& buffer) override;
  void onError(const MetaProtocolProxy::Metadata& metadata, const MetaProtocolProxy::Error& error,
               Buffer::Instance& buffer) override;

protected:
  // 处理一次buffer数据
  QzaDecodeStatus handleState(Buffer::Instance& buffer);
  QzaDecodeStatus decodeQzaHeader(Buffer::Instance& buffer);
  QzaDecodeStatus decodePayload(Buffer::Instance& buffer);
  void toMetadata(MetaProtocolProxy::Metadata& metadata);

private:
  QzaDecodeStatus decode_status{QzaDecodeStatus::DecodeHeader};
  QzaHeader qza_header_;
  MetaProtocolProxy::MessageType messageType_;
  std::unique_ptr<Buffer::OwnedImpl> origin_msg_;
};

} // namespace Qza
} // namespace MetaProtocolProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy

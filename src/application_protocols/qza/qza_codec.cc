#include <any>

#include "envoy/buffer/buffer.h"

#include "common/common/logger.h"

#include "src/meta_protocol_proxy/codec/codec.h"
#include "src/application_protocols/qza/qza_codec.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MetaProtocolProxy {
namespace Qza {

MetaProtocolProxy::DecodeStatus QzaCodec::decode(Buffer::Instance& buffer,
                                                  MetaProtocolProxy::Metadata& metadata) {
	ENVOY_LOG(debug, "Qza decoder: {} bytes available, msg type: {}", buffer.length(), metadata.getMessageType());
  messageType_ = metadata.getMessageType();
  ASSERT(messageType_ == MetaProtocolProxy::MessageType::Request ||
         messageType_ == MetaProtocolProxy::MessageType::Response);

  while (decode_status != QzaDecodeStatus::DecodeDone) {
    decode_status = handleState(buffer);
    if (decode_status == QzaDecodeStatus::WaitForData) {
      return DecodeStatus::WaitForData;
    }
  }

  toMetadata(metadata);
  // reset decode status
  decode_status = QzaDecodeStatus::DecodeHeader;
	return DecodeStatus::Done;
}

void QzaCodec::encode(const MetaProtocolProxy::Metadata& metadata,
                      const MetaProtocolProxy::Mutation& mutation, Buffer::Instance& buffer) {
  // TODO we don't need to implement encode for now
  (void)metadata;
  (void)mutation;
  (void)buffer;
}

void QzaCodec::onError(const MetaProtocolProxy::Metadata& metadata,
                        const MetaProtocolProxy::Error& error, Buffer::Instance& buffer) {
  (void)metadata;
  QzaHeader response;
  response.set_header_len(QzaHeader::QZA_HEADER_SIZE);
  response.set_pack_len(QzaHeader::QZA_HEADER_SIZE);
  switch (error.type) {
  case MetaProtocolProxy::ErrorType::RouteNotFound:
    response.set_rsp_code(static_cast<int16_t>(QzaCode::QzaNoRoute));
    break;
  default:
    response.set_rsp_code(static_cast<int16_t>(QzaCode::QzaServerSystem));
    break;
  }
  response.encode(buffer);
}

QzaDecodeStatus QzaCodec::handleState(Buffer::Instance& buffer) {
  switch (decode_status)
  {
  case QzaDecodeStatus::DecodeHeader:
    return decodeQzaHeader(buffer);
  case QzaDecodeStatus::DecodePayload:
    return decodePayload(buffer);
  default:
    NOT_REACHED_GCOVR_EXCL_LINE;
  }
  return QzaDecodeStatus::DecodeDone;
}

QzaDecodeStatus QzaCodec::decodeQzaHeader(Buffer::Instance& buffer) {
  ENVOY_LOG(debug, "decode qza header: {}", buffer.length());
  if (buffer.length() < QzaHeader::QZA_HEADER_SIZE) {
    ENVOY_LOG(debug, "continue {}", buffer.length());
    return QzaDecodeStatus::WaitForData;
  }

  if(!qza_header_.decode(buffer)) {
    throw EnvoyException(fmt::format("qza header invalid"));
  }
  
  return QzaDecodeStatus::DecodePayload;
}

QzaDecodeStatus QzaCodec::decodePayload(Buffer::Instance& buffer) {
  ENVOY_LOG(debug, "decode payload {} ? {}", qza_header_.get_pack_len(), buffer.length());
  if (buffer.length() < qza_header_.get_pack_len()) {
    ENVOY_LOG(debug, "continue {}", buffer.length());
    return QzaDecodeStatus::WaitForData;
  }

  origin_msg_ = std::make_unique<Buffer::OwnedImpl>();
  origin_msg_->move(buffer, qza_header_.get_pack_len());

  return QzaDecodeStatus::DecodeDone;
}

void QzaCodec::toMetadata(MetaProtocolProxy::Metadata& metadata) {
  metadata.setRequestId(qza_header_._pack_flow);

  ENVOY_LOG(debug, "qza_header_._req_cmd: {}", qza_header_.get_req_cmd());
  metadata.putString("cmd", std::to_string(qza_header_.get_req_cmd()));
  metadata.putString("sub_cmd", std::to_string(qza_header_.get_req_sub_cmd()));
  metadata.putString("uid", std::to_string(qza_header_.get_uid()));
  
  metadata.setOriginMessage(*origin_msg_);
}

} // namespace Qza
} // namespace MetaProtocolProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
#include "src/application_protocols/qza/protocol.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MetaProtocolProxy {
namespace Qza {

const uint32_t QzaDetailInfo::QZA_DETAIL_INFO_SIZE = 95;
const uint32_t QzaHeader::QZA_HEADER_SIZE = 106;

bool QzaDetailInfo::decode(Buffer::Instance& buffer, uint64_t offset) {
  if (buffer.length() < QZA_DETAIL_INFO_SIZE+offset) {
    ENVOY_LOG(error, "QzaDetailInfo decode buffer.length:{} < {}.", 
              buffer.length(), QZA_DETAIL_INFO_SIZE+offset);
    return false;
  }

  uint64_t pos = offset;
  _version = buffer.peekBEInt<int8_t>(pos);
  pos += sizeof(int8_t);
  _type_platform = buffer.peekBEInt<uint8_t>(pos);
  pos += sizeof(uint8_t);
  _type_app = buffer.peekBEInt<uint32_t>(pos);
  pos += sizeof(uint32_t);
  _type_source = buffer.peekBEInt<uint32_t>(pos);
  pos += sizeof(uint32_t);
  buffer.copyOut(pos, sizeof(_reserve_buf), _reserve_buf);
  pos += sizeof(_reserve_buf);
  _req_ip = buffer.peekBEInt<uint32_t>(pos);
  pos += sizeof(uint32_t);
  _ptlogin_id = buffer.peekBEInt<uint32_t>(pos);
  pos += sizeof(uint32_t);
  _auth_type = buffer.peekBEInt<int8_t>(pos);
  pos += sizeof(int8_t);
  _auth_type_hi = buffer.peekBEInt<int8_t>(pos);
  pos += sizeof(int8_t);
  _extend_len_hi = buffer.peekBEInt<int8_t>(pos);
  pos += sizeof(int8_t);
  _open_key_len_hi = buffer.peekBEInt<int8_t>(pos);
  pos += sizeof(int8_t);
  buffer.copyOut(pos, sizeof(_reserve_buf2), _reserve_buf2);
  pos += sizeof(_reserve_buf2);
  _alpha_flag = buffer.peekBEInt<int8_t>(pos);
  pos += sizeof(int8_t);
  _ptlogin_state = buffer.peekBEInt<uint16_t>(pos);
  pos += sizeof(uint16_t);
  _req_cmd_item = buffer.peekBEInt<int16_t>(pos);
  pos += sizeof(int16_t);
  _client_ip = buffer.peekBEInt<uint32_t>(pos);
  pos += sizeof(uint32_t);
  _host_uin = buffer.peekBEInt<uint32_t>(pos);
  pos += sizeof(uint32_t);
  _client_uin = buffer.peekBEInt<uint32_t>(pos);
  pos += sizeof(uint32_t);
  buffer.copyOut(pos, sizeof(_extend), _extend);
  pos += sizeof(_extend);
  _pack_len_high = buffer.peekBEInt<uint16_t>(pos);
  pos += sizeof(uint16_t);
  _extend_len = buffer.peekBEInt<uint8_t>(pos);
  pos += sizeof(uint8_t);
  _extend_str.resize(_extend_len);
  buffer.copyOut(pos, _extend_len, &(_extend_str[0]));
  pos += _extend_len;
  
  ASSERT(pos == (offset + QZA_DETAIL_INFO_SIZE + _extend_len));
  return true;
}

bool QzaDetailInfo::encode(Buffer::Instance& buffer) {
  buffer.writeBEInt(_version);
  buffer.writeBEInt(_type_platform);
  buffer.writeBEInt(_type_app);
  buffer.writeBEInt(_type_source);
  buffer.add(_reserve_buf, sizeof(_reserve_buf));
  buffer.writeBEInt(_req_ip);
  buffer.writeBEInt(_ptlogin_id);
  buffer.writeBEInt(_auth_type);
  buffer.writeBEInt(_auth_type_hi);
  buffer.writeBEInt(_extend_len_hi);
  buffer.writeBEInt(_open_key_len_hi);
  buffer.add(_reserve_buf2, sizeof(_reserve_buf2));
  buffer.writeBEInt(_alpha_flag);
  buffer.writeBEInt(_ptlogin_state);
  buffer.writeBEInt(_req_cmd_item);
  buffer.writeBEInt(_client_ip);
  buffer.writeBEInt(_host_uin);
  buffer.writeBEInt(_client_uin);
  buffer.add(_extend, sizeof(_extend));
  buffer.writeBEInt(_pack_len_high);
  buffer.writeBEInt(_extend_len);
  buffer.add(_extend_str);
  return true;
}

bool QzaHeader::decode(Buffer::Instance& buffer) {
  if (buffer.length() < QZA_HEADER_SIZE) {
    ENVOY_LOG(error, "QzaHeader decode buffer.length:{} < {}.", buffer.length(), QZA_HEADER_SIZE);
    return false;
  }

  uint64_t pos = 0;
  _version = buffer.peekBEInt<int8_t>(pos);
  pos += sizeof(int8_t);
  _pack_flow = buffer.peekBEInt<uint32_t>(pos);
  pos += sizeof(uint32_t);
  _pack_len = buffer.peekBEInt<uint16_t>(pos);
  pos += sizeof(uint16_t);
  _req_cmd = buffer.peekBEInt<int16_t>(pos);
  pos += sizeof(int16_t);
  _rsp_code = buffer.peekBEInt<int16_t>(pos);
  pos += sizeof(int16_t);

  ASSERT(pos == (QZA_HEADER_SIZE - QzaDetailInfo::QZA_DETAIL_INFO_SIZE));
  return _detail_info.decode(buffer, pos);
}

bool QzaHeader::encode(Buffer::Instance& buffer) {
  buffer.writeBEInt(_version);
  buffer.writeBEInt(_pack_flow);
  buffer.writeBEInt(_pack_len);
  buffer.writeBEInt(_req_cmd);
  buffer.writeBEInt(_rsp_code);
  return _detail_info.encode(buffer);
}

} // namespace Qza
} // namespace MetaProtocolProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy

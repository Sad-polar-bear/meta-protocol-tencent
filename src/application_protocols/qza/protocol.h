#pragma once

#include "common/buffer/buffer_impl.h"
#include "common/common/logger.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace MetaProtocolProxy {
namespace Qza {

enum class QzaCode {
  QzaNoRoute = 50,
  QzaServerSystem = 51,
};

struct QzaDetailInfo : public Logger::Loggable<Logger::Id::filter> {
  static const uint32_t QZA_DETAIL_INFO_SIZE;
  //版本号
  int8_t _version;
  //平台类型，平台会针对于此字段统计
  uint8_t	_type_platform;
  //应用类型，平台会针对于此字段统计
  uint32_t _type_app;
  //具体来源类型，例如操作触发地。此字段由业务自己控制，平台不单独针对于此字段统计
  uint32_t _type_source;
  //保留字段
  char _reserve_buf[23];
  //业务请求的IP
  uint32_t _req_ip;
  //业务验证登陆态的id	
  uint32_t _ptlogin_id;
  //业务登陆态key校验的类型
  int8_t _auth_type;
  //业务登陆态key校验的类型扩展
  int8_t _auth_type_hi;
  //变长附加信息长度扩展
  int8_t _extend_len_hi;
  //openkey长度扩展
  int8_t _open_key_len_hi;
  //保留字段2
  char _reserve_buf2[5];
  //标记是否alpha用户 1表示是alpha用户
  int8_t _alpha_flag;
  //登陆状态 0xffff-未验证, 0-成功 1-失败
  uint16_t _ptlogin_state;
  //业务请求的子命令字
  int16_t _req_cmd_item;		
  //访问者的客户端IP
  uint32_t _client_ip;
  //被访问者的UIN
  uint32_t _host_uin;
  //访问者的UIN
  uint32_t _client_uin;
  //扩展数据，通过skey校验登陆态的地方会使用这里的前11个字节存储skey相关信息
  char _extend[25];
  //包长高位 整个包长度==_pack_len + 65536 * _detail_info._pack_len_high;
  uint16_t _pack_len_high;
  //变长附加信息长度
  uint8_t	_extend_len;
  //扩展信息
  std::string _extend_str;

  //QzaDetailInfo解码
  bool decode(Buffer::Instance& buffer, uint64_t offset);
  //QzaDetailInfo编码
  bool encode(Buffer::Instance& buffer);
};

struct QzaHeader : public Logger::Loggable<Logger::Id::filter> {
  static const uint32_t QZA_HEADER_SIZE;
  //版本号
  int8_t _version;
  //流水号
  uint32_t _pack_flow;
  //整个包长度的低位 整个包长度==_pack_len + 65536 * _detail_info._pack_len_high;
  uint16_t _pack_len;
  //请求的命令字
  int16_t _req_cmd;
  //返回码
  int16_t _rsp_code;
  //请求详细
  QzaDetailInfo _detail_info;

  //qza协议头解码
  bool decode(Buffer::Instance& buffer);
  //qza协议头编码
  bool encode(Buffer::Instance& buffer);

  uint32_t get_pack_len() const;
  uint32_t get_extend_len() const;
  uint32_t get_header_len() const; 
  int16_t get_req_cmd() const;
  int16_t get_req_sub_cmd() const;
  uint32_t get_uid() const;
  void set_pack_len(uint32_t pack_len);
  void set_extend_len(uint32_t extend_len);
  void set_header_len(uint32_t header_len);
  void set_rsp_code(int16_t rsp_code);
};

// ------------------ getter for QzaHeader---------------------
inline uint32_t QzaHeader::get_pack_len() const {
  return (static_cast<uint32_t>(_detail_info._pack_len_high) << 16) 
        +(static_cast<uint32_t>(_pack_len));
}
inline uint32_t QzaHeader::get_extend_len() const {
  return (static_cast<uint32_t>(_detail_info._extend_len_hi) << 8)
        +(static_cast<uint32_t>(_detail_info._extend_len));
}
inline uint32_t QzaHeader::get_header_len() const {
  return QZA_HEADER_SIZE + get_extend_len();
}
inline int16_t QzaHeader::get_req_cmd() const {
  return _req_cmd;
}
inline int16_t QzaHeader::get_req_sub_cmd() const {
  return _detail_info._req_cmd_item;
}
inline uint32_t QzaHeader::get_uid() const {
  return _detail_info._client_uin;
}

// ------------------ setter for QzaHeader---------------------
inline void QzaHeader::set_pack_len(uint32_t pack_len) {
  _detail_info._pack_len_high = pack_len >> 16;
  _pack_len = pack_len & 0xffff;
}
inline void QzaHeader::set_extend_len(uint32_t extend_len) {
  _detail_info._extend_len_hi = extend_len >> 8;
  _detail_info._extend_len = extend_len & 0xff;
}
inline void QzaHeader::set_header_len(uint32_t header_len) {
  ASSERT(header_len >= QZA_HEADER_SIZE);
  set_extend_len(header_len-QZA_HEADER_SIZE);
}
inline void QzaHeader::set_rsp_code(int16_t rsp_code) {
  _rsp_code = rsp_code;
}

} // namespace Qza
} // namespace MetaProtocolProxy
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
/*
 * code_extern.h
 *
 *  Created on: 2015Äê02ÔÂ03ÈÕ
 *      Author: jimm
 */

//extern int32_t encode_bool(uint8_t* buf, const uint32_t size, uint32_t* offset, const bool data);
extern int32_t encode_int8(uint8_t* buf, const uint32_t size, uint32_t* offset, const int8_t data);
extern int32_t encode_int16(uint8_t* buf, const uint32_t size, uint32_t* offset, const int16_t data);
extern int32_t encode_int32(uint8_t* buf, const uint32_t size, uint32_t* offset, const int32_t data);
extern int32_t encode_int64(uint8_t* buf, const uint32_t size, uint32_t* offset, const int64_t data);
extern int32_t encode_uint8(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint8_t data);
extern int32_t encode_uint16(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint16_t data);
extern int32_t encode_uint32(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint32_t data);
extern int32_t encode_uint64(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint64_t data);
extern int32_t encode_uint8_mem(uint8_t* buf, const uint32_t size, uint32_t* offset, const uint8_t *data, const uint32_t length, const uint32_t maxLength);
extern int32_t encode_char_mem(uint8_t* buf, const uint32_t size, uint32_t* offset, const char *data, const uint16_t maxLength);
extern int32_t encode_double(uint8_t* buf, const uint32_t size, uint32_t* offset, const double data);

//extern int32_t decode_bool(const uint8_t *buf, const uint32_t size, uint32_t* offset, bool* data);
extern int32_t decode_int8(const uint8_t *buf, const uint32_t size, uint32_t* offset, int8_t* data);
extern int32_t decode_int16(const uint8_t *buf, const uint32_t size, uint32_t* offset, int16_t* data);
extern int32_t decode_int32(const uint8_t *buf, const uint32_t size, uint32_t* offset, int32_t* data);
extern int32_t decode_int64(const uint8_t *buf, const uint32_t size, uint32_t* offset, int64_t* data);
extern int32_t decode_uint8(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint8_t* data);
extern int32_t decode_uint16(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint16_t* data);
extern int32_t decode_uint32(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint32_t* data);
extern int32_t decode_uint64(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint64_t* data);
extern int32_t decode_uint8_mem(const uint8_t *buf, const uint32_t size, uint32_t* offset, uint8_t* data, const uint32_t length, const uint32_t maxLength);
extern int32_t decode_char_mem(const uint8_t *buf, const uint32_t size, uint32_t* offset, char* data, const uint16_t maxLength);
extern int32_t decode_double(const uint8_t *buf, const uint32_t size, uint32_t* offset, double* data);

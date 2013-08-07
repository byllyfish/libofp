#include "ofp/types.h"


constexpr char HexDigits[17] = "0123456789ABCDEF";


inline
unsigned FromHex(char hex) 
{
	assert(std::isxdigit(hex));
	return ofp::Unsigned_cast(hex <= '9'? hex - '0': (hex & ~0x20) - 'A' + 10);
}

inline
char ToHex(ofp::UInt8 value) 
{
	assert(value < 16);
	return HexDigits[value];
}


std::string ofp::RawDataToHex(const void *data, size_t len)
{
	std::string result;
	result.reserve(2*len);

	const UInt8 *pos = static_cast<const UInt8*>(data);
	const UInt8 *end = pos + len;
	
	char buf[2];
	while (pos < end) {
		buf[0] = ToHex(*pos >> 4);
		buf[1] = ToHex(*pos++ & 0x0F);
		result.append(buf, sizeof(buf));
	}
	
	return result;	
}


std::string ofp::RawDataToHex(const void *data, size_t len, char delimiter, int word)
{
	std::string result;
	result.reserve(2*len);

	const UInt8 *pos = static_cast<const UInt8*>(data);
	const UInt8 *end = pos + len;
	
	int cnt = 0;
	char buf[2];
	while (pos < end) {
		if (++cnt > word) {
			result.push_back(delimiter);
			cnt = 1;
		}
		buf[0] = ToHex(*pos >> 4);
		buf[1] = ToHex(*pos++ & 0x0F);
		result.append(buf, sizeof(buf));
	}
	
	return result;	
}


size_t ofp::HexToRawData(const std::string &hex, void *data, size_t maxlen)
{
	UInt8 *begin = static_cast<UInt8*>(data);
	UInt8 *end = begin + maxlen;
	
	unsigned ch[2];
	unsigned idx = 0;
	UInt8 *out = begin;
	for (auto inp = hex.c_str(); *inp; ++inp) {
		if (std::isxdigit(*inp)) {
			ch[idx++] = FromHex(*inp);
			if (idx >= 2) {
				assert(ch[0] < 16 && ch[1] < 16);
				*out++ = UInt8_narrow_cast((ch[0] << 4) | ch[1]);
				if (out >= end)
					break;
				idx = 0;
			}
		}
	}
	
	assert(end >= out);
	std::memset(out, 0, Unsigned_cast(end - out));
	
	assert(out >= begin);
	return Unsigned_cast(out - begin);
}


std::string ofp::HexToRawData(const std::string &hex)
{
	std::string result;
	result.reserve(hex.size() / 2);
	
	unsigned ch[2];
	unsigned idx = 0;
	for (auto inp = hex.c_str(); *inp; ++inp) {
		if (std::isxdigit(*inp)) {
			ch[idx++] = FromHex(*inp);
			if (idx >= 2) {
				assert(ch[0] < 16 && ch[1] < 16);
				result.push_back(static_cast<char>((ch[0] << 4) | ch[1]));
				idx = 0;
			}
		}
	}
	
	return result;
}


bool ofp::IsMemFilled(const void *data, size_t len, char ch)
{
	const UInt8 *p = static_cast<const UInt8 *>(data);
	while (len-- > 0) {
		if (*p++ != ch) return false;
	}
	return true;
}

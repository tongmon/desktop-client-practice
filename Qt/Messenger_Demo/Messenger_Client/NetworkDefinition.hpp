#ifndef HEADER__FILE__NETWORKDEFINITION
#define HEADER__FILE__NETWORKDEFINITION

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <type_traits>

class TCPHeader
{
    template <size_t SIZE>
    union Bytes {
        std::array<std::byte, SIZE> bytes;
        std::uint64_t number;
    };

    Bytes<8> m_connection_type;
    Bytes<8> m_data_size;

  public:
    TCPHeader(const std::string &data)
    {
        auto char_to_byte = [](char c) {
            return std::byte(c);
        };

        std::transform(data.begin(), data.begin() + 8, m_connection_type.bytes.begin(), char_to_byte);
        std::transform(data.begin() + 8, data.end(), m_data_size.bytes.begin(), char_to_byte);
    }

    std::uint64_t GetConnectionType()
    {
        return m_connection_type.number;
    }

    std::uint64_t GetDataSize()
    {
        return m_data_size.number;
    }
};

constexpr size_t TCP_HEADER_SIZE = sizeof(TCPHeader);

constexpr std::uint64_t LOGIN_CONNECTION_TYPE = 0;
constexpr std::uint64_t TEXTCHAT_CONNECTION_TYPE = 1;
constexpr std::uint64_t IMAGECHAT_CONNECTION_TYPE = 2;

#endif /* HEADER__FILE__NETWORKDEFINITION */

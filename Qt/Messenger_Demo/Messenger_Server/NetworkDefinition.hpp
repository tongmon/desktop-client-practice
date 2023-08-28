#ifndef HEADER__FILE__NETWORKDEFINITION
#define HEADER__FILE__NETWORKDEFINITION

#include <algorithm>
#include <array>
#include <cstdint>
#include <string>
#include <type_traits>

class TCPHeader
{
    enum
    {
        CONNECTION_TYPE = 0,
        DATA_SIZE,
        BUFFER_CNT
    };

    union Bytes {
        std::array<std::byte, 8> bytes;
        std::uint64_t number;
    };

    Bytes m_buffers[BUFFER_CNT];

  public:
    TCPHeader(const std::string &data)
    {
        for (int i = 0; i < 16; i += 8)
            for (int j = 0; j < 8; j++)
                m_buffers[i / 8].bytes[j] = static_cast<std::byte>(data[i + j]);
    }

    TCPHeader(std::uint64_t connection_type, std::uint64_t data_size)
    {
        m_buffers[CONNECTION_TYPE].number = connection_type;
        m_buffers[DATA_SIZE].number = data_size;
    }

    std::uint64_t GetConnectionType()
    {
        return m_buffers[CONNECTION_TYPE].number;
    }

    std::uint64_t GetDataSize()
    {
        return m_buffers[DATA_SIZE].number;
    }

    std::string GetHeaderBuffer()
    {
        std::string ret(16, 0);
        for (int i = 0; i < 16; i += 8)
            for (int j = 0; j < 8; j++)
                ret[i + j] = static_cast<char>(m_buffers[i / 8].bytes[j]);
        return ret;
    }
};

constexpr size_t TCP_HEADER_SIZE = sizeof(TCPHeader);

constexpr std::uint64_t LOGIN_CONNECTION_TYPE = 0;
constexpr std::uint64_t TEXTCHAT_CONNECTION_TYPE = 1;
constexpr std::uint64_t IMAGECHAT_CONNECTION_TYPE = 2;

#endif /* HEADER__FILE__NETWORKDEFINITION */

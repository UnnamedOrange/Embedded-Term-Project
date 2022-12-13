/**
 * @file source_to_stream_dummy.h
 * @author UnnamedOrange
 * @brief 测试用数据流裁剪类。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <array>

#include "source_to_stream_base.h"
#include <data_source/data_source_utility.h>

namespace user
{
    /**
     * @brief 测试用数据流裁剪类。
     * 裁剪后的数据流长四个字节，将输入原样输出。
     */
    class source_to_stream_dummy : public source_to_stream_base
    {
    public:
        /**
         * @brief 每次读取的数据流的大小。
         */
        static constexpr size_t size_per_read = 4;

    private:
        static constexpr int mux_address_size = 256;
        inline static int next_mux_address = 0;
        int mux_address;

    private:
        size_t current_idx{};
        std::array<byte_t, 4> buffer{};

    public:
        source_to_stream_dummy() : mux_address(next_mux_address)
        {
            next_mux_address = (next_mux_address + 1) % mux_address_size;
        }

    public:
        std::string name() const override { return "Dummy stream"; }

    public:
        void read_data_source(
            const std::shared_ptr<data_source_base>& data_source) override
        {
            async_push(data_source->read(size_per_read, mux_address));
        }
        std::optional<byte_array_t> next() override
        {
            // Note: next 函数一般要读取多个字节，
            std::optional<byte_t> byte;
            while ((byte = next_byte()))
            {
                buffer[current_idx++] = *byte;
                if (current_idx >= buffer.size())
                {
                    current_idx = 0;
                    return byte_array_t(buffer.begin(), buffer.end());
                }
            }
            return std::nullopt;
        }
    };
} // namespace user

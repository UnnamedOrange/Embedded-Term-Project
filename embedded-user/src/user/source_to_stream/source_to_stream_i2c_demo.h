/**
 * @file source_to_stream_i2c_demo.h
 * @author UnnamedOrange
 * @brief 用于 I2C 演示的数据流裁剪类。
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
    class source_to_stream_i2c_demo : public source_to_stream_base
    {
    private:
        static constexpr size_t data_size = 4;
        static constexpr int mux_address = 0x08;
        size_t current_idx{};
        std::array<byte_t, data_size> buffer{};

    public:
        std::string name() const override { return "I2C Demo"; }

    public:
        void read_data_source(
            const std::shared_ptr<data_source_base>& data_source) override
        {

            data_source->write({static_cast<byte_t>(data_size)}); // 长度。
            auto data = data_source->read(data_size, mux_address);
            if (data.size() != data_size)
                return;
            async_push(data);
        }
        std::optional<byte_array_t> next() override
        {
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

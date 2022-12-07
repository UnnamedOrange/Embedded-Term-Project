/**
 * @file source_to_stream_i2c_demo.h
 * @author UnnamedOrange
 * @brief 用于 I2C 演示的数据流裁剪类。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include "source_to_stream_base.h"
#include <data_source/data_source_utility.h>

namespace user
{
    class source_to_stream_i2c_demo : public source_to_stream_base
    {
    public:
        std::string name() const override { return "I2C Demo"; }

    public:
        void read_data_source(
            const std::shared_ptr<data_source_base>& data_source) override
        {
            // TODO: 实现 read_data_source 函数。
        }
        std::optional<byte_array_t> next() override
        {
            // TODO: 实现 next 函数。
            return std::nullopt;
        }
    };
} // namespace user

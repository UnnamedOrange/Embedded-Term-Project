/**
 * @file data_source_i2c.h
 * @author UnnamedOrange
 * @brief 真实的 I2C 数据源。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#if defined(BUILD_I2C)
#include <bcm2835.h>

#include "bcm2835_wrapper.h"
#endif

#include "data_source_base.h"
#include "data_source_type.h"
#include "data_source_utility.h"

namespace user
{
    /**
     * @brief 真实的 I2C 数据源。
     */
    class data_source_i2c : public data_source_base
    {
    public:
        data_source_type type() const override { return data_source_type::i2c; }
        std::string name() const override { return "I2C"; }

        byte_array_t read(size_t size, int mux_address) override
        {
#if defined(BUILD_I2C)
            // TODO: 实现 I2C 读取。
            return {};
#else
            return {};
#endif
        }

        void write(const byte_array_t&) override
        {
#if defined(BUILD_I2C)
            // TODO: 实现 I2C 写入。
#endif
        }
    };
} // namespace user

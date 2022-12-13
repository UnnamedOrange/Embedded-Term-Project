/**
 * @file data_source_i2c.h
 * @author UnnamedOrange
 * @brief 真实的 I2C 数据源。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

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
        data_source_i2c();

    public:
        data_source_type type() const override;
        std::string name() const override;

        byte_array_t read(size_t size, int mux_address) override;

        void write(const byte_array_t& byte_array) override;
    };
} // namespace user

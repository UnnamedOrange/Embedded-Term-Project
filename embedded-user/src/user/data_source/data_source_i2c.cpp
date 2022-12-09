/**
 * @file data_source_i2c.cpp
 * @author UnnamedOrange
 * @brief 真实的 I2C 数据源。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include <cstdint>

#if defined(BUILD_I2C)
#include <bcm2835.h>

#include "bcm2835_wrapper.h"
#endif

#include "data_source_base.h"
#include "data_source_i2c.h"
#include "data_source_type.h"
#include "data_source_utility.h"

using namespace user;
data_source_i2c::data_source_i2c()
{
#if defined(BUILD_I2C)
    bcm2835_i2c_setClockDivider(BCM2835_I2C_CLOCK_DIVIDER_2500);
#endif
}

data_source_type data_source_i2c::type() const { return data_source_type::i2c; }
std::string data_source_i2c::name() const { return "I2C"; }

byte_array_t data_source_i2c::read(size_t size, int mux_address)
{
#if defined(BUILD_I2C)
    bcm2835_i2c_setSlaveAddress(static_cast<uint8_t>(mux_address));
    byte_array_t ret(size);
    int result;
    result = bcm2835_i2c_read(reinterpret_cast<char*>(ret.data()), ret.size());
    if (result != BCM2835_I2C_REASON_OK)
        return {};
    return ret;
#else
    return {};
#endif
}

void data_source_i2c::write(const byte_array_t& byte_array)
{
#if defined(BUILD_I2C)
    int result;
    result = bcm2835_i2c_write(reinterpret_cast<const char*>(byte_array.data()),
                               byte_array.size());
    if (result != BCM2835_I2C_REASON_OK)
        return;
#endif
}

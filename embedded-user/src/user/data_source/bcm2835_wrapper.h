/**
 * @file bcm2835_wrapper.h
 * @author UnnamedOrange
 * @brief BCM2835 库的封装。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#if defined(BUILD_I2C)

#include <stdexcept>

#include <bcm2835.h>

namespace user
{
    class bcm2835_wrapper
    {
    public:
        bcm2835_wrapper()
        {
            if (!bcm2835_init())
            {
                throw std::runtime_error(
                    "bcm2835_init failed. Are you running as root?");
            }
            if (!bcm2835_i2c_begin())
            {
                throw std::runtime_error(
                    "bcm2835_i2c_begin failed. Are you running as root?");
            }
        }
        ~bcm2835_wrapper()
        {
            bcm2835_i2c_end();
            bcm2835_close();
        }
    };
    inline bcm2835_wrapper _bcm2835_wrapper;
} // namespace user

#endif

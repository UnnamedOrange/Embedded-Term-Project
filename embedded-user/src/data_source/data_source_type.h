/**
 * @file data_source_type.h
 * @author UnnamedOrange
 * @brief 定义数据源类型。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

namespace user
{
    /**
     * @brief 数据源类型枚举。
     */
    enum class data_source_type
    {
        /**
         * @brief 未知数据源。
         */
        unknown = 0,

        /**
         * @brief 串口数据源。
         */
        serial_port = 1,
        /**
         * @brief I2C 数据源。
         */
        i2c = 2,
        /**
         * @brief SPI 数据源。
         */
        spi = 3,

        /**
         * @brief 虚拟数据源。
         */
        dummy = 255,

        /**
         * @brief 最小的自定义数据源标号。
         */
        user_defined = 256,
    };
} // namespace user

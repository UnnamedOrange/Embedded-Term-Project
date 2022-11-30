/**
 * @file data_source_base.h
 * @author UnnamedOrange
 * @brief 数据源基类。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <string>

#include "data_source_type.h"
#include "data_source_utility.h"

namespace user
{
    /**
     * @brief 数据源基类。
     */
    class data_source_base
    {
    public:
        virtual ~data_source_base() = default;

    public:
        /**
         * @brief 获取数据源类型。
         */
        virtual data_source_type type() const = 0;
        /**
         * @brief 获取数据源名字。
         */
        virtual std::string name() const = 0;

        /**
         * @brief 从数据源读取数据。
         *
         * @note 该函数不会因为数据源没有数据而阻塞。
         *
         * @param size 要读取的字节数。
         * @param mux_address 多路复用地址。
         * @return byte_array_t 读取到的字节流。
         * 不保证长度与 size 相同。
         */
        virtual byte_array_t read(size_t size, int mux_address) = 0;

        /**
         * @brief 向数据源写入数据。
         *
         * @param byte_array 要写入的内容。
         */
        virtual void write(const byte_array_t& byte_array) = 0;
    };
} // namespace user

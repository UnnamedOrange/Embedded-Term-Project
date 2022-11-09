/**
 * @file data_source_base.h
 * @author UnnamedOrange
 * @brief 数据源基类。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include "data_source_type.h"
#include "data_source_utility.h"

namespace user
{
    /**
     * @brief 数据源基类。
     */
    class data_source_base
    {
    private:
        bool working = false; // 是否正在工作。
        int mux_address = 0;  // 多路复用地址。

    public:
        virtual ~data_source_base() = default;

    public:
        /**
         * @brief 获取数据源类型。
         */
        virtual data_source_type type() const = 0;

        /**
         * @brief 从数据源读取数据。
         *
         * @note 该函数不会因为数据源没有数据而阻塞。
         *
         * @return 读取到的字节流。
         */
        virtual byte_array_t read() = 0;
    };
} // namespace user

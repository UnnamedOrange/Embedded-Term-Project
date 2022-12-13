/**
 * @file record_receive_i.h
 * @author UnnamedOrange
 * @brief 接收记录接口。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <any>

namespace user
{
    /**
     * @brief 接收记录接口。
     *
     * @note 接口需要虚继承。
     */
    class record_receive_i
    {
    public:
        virtual ~record_receive_i() = default;

    public:
        /**
         * @brief 当收到新的记录时，该函数会被核心线程调用。
         *
         * @param record 记录。其具体类型由绑定的数据源裁剪器决定。
         */
        virtual void record_receive(const std::any& record) = 0;
    };
} // namespace user

/**
 * @file record_writer_base.h
 * @author UnnamedOrange
 * @brief 数据写入器基类。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include "record_receive_i.h"

namespace user
{
    /**
     * @brief 数据写入器基类。
     *
     * @todo 设计成员变量。
     * @todo 设计成员函数。
     */
    class record_writer_base : virtual public record_receive_i
    {
    public:
        virtual ~record_writer_base() = default;
    };
} // namespace user

/**
 * @file record_writer_dummy.h
 * @author UnnamedOrange
 * @brief 测试用数据写入器。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <any>
#include <string>

#include "record_writer_base.h"

namespace user
{
    /**
     * @brief 测试用数据写入器。将记录看作 32 位无符号整数。
     */
    class record_writer_dummy : public record_writer_base
    {
    public:
        void record_receive(const std::any& record) override
        {
            const auto& value = std::any_cast<const int32_t&>(record);
            write(std::to_string(value) + "\n");
        }
    };
} // namespace user

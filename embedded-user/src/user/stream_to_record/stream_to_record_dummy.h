/**
 * @file stream_to_record_dummy.h
 * @author UnnamedOrange
 * @brief 测试用记录提取器。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <cassert>

#include "stream_to_record_base.h"

namespace user
{
    /**
     * @brief 测试用记录。仅包含一个 32 位有符号整数。
     */
    using record_dummy_t = int32_t;

    /**
     * @brief 测试用记录提取器。
     * 将字节流提取为 @ref record_dummy_t 类型。
     */
    class stream_to_record_dummy : public stream_to_record_base
    {
    public:
        std::any stream_to_record(const byte_array_t& stream) override
        {
            assert(stream.size() == 4);
            // 将字节流看作四字节有符号整数。
            return record_dummy_t{*reinterpret_cast<const int*>(stream.data())};
        }
    };
} // namespace user

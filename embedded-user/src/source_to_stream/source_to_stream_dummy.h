/**
 * @file source_to_stream_dummy.h
 * @author UnnamedOrange
 * @brief 测试用数据流裁剪类。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include "source_to_stream_base.h"

namespace user
{
    /**
     * @brief 测试用数据流裁剪类。
     * 裁剪后的数据流长一个字节，将输入原样输出。
     */
    class source_to_stream_dummy : public source_to_stream_base
    {
    public:
        /**
         * @todo 实现 @ref next 函数。
         */
        std::optional<byte_array_t> next() override
        {
            return byte_array_t();
        }
    };
} // namespace user
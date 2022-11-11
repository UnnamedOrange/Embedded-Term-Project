/**
 * @file source_to_stream_base.h
 * @author UnnamedOrange
 * @brief 数据流裁剪基类。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <optional>

#include <data_source/data_source_utility.h>

namespace user
{
    /**
     * @brief 数据流裁剪基类。
     *
     * @todo 设计成员变量。
     */
    class source_to_stream_base
    {
    private:
        // TODO: 设计成员变量。

    public:
        virtual ~source_to_stream_base() = default;

    public:
        /**
         * @brief 异步发送字节流。
         *
         * @note 子线程调用该函数，将数据保存到缓冲区中，待核心线程处理。
         *
         * @todo 实现异步发送字节流。
         */
        void async_push(const byte_array_t& byte_array) {}

    public:
        /**
         * @brief 工作线程获取裁剪后的下一个字节流。
         *
         * @return std::optional<byte_array_t>
         * 如果缓冲区内的字节流可以完整裁剪，返回裁剪后的字节流。否则返回空。
         */
        virtual std::optional<byte_array_t> next() = 0;
    };
} // namespace user

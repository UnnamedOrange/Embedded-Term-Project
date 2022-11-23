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
#include <vector>

#include <readerwriterqueue.h>

#include <data_source/data_source_utility.h>

namespace user
{
    /**
     * @brief 数据流裁剪基类。
     */
    class source_to_stream_base
    {
    public:
        /**
         * @brief 定义存储回退字节的容器类型。
         */
        using unput_bytes_t = std::vector<byte_t>;

    private:
        moodycamel::ReaderWriterQueue<byte_t> queue; // 来自数据源的字节流。
        unput_bytes_t unput_bytes;                   // 回退的字节。

    public:
        virtual ~source_to_stream_base() = default;

    public:
        /**
         * @brief 异步发送字节流。
         *
         * @note 子线程调用该函数，将数据保存到缓冲区中，待核心线程处理。
         */
        void async_push(const byte_array_t& byte_array)
        {
            for (const auto& byte : byte_array)
                queue.enqueue(byte);
        }

    protected:
        /**
         * @brief 核心线程读取一个字节。
         *
         * @return std::optional<byte_t>
         * 如果缓冲区中存在字节，则返回该字节。否则返回空。
         */
        std::optional<byte_t> next_byte()
        {
            byte_t ret;
            // 先返回回退的字节。
            if (!unput_bytes.empty())
            {
                ret = unput_bytes.back();
                unput_bytes.pop_back();
                return ret;
            }
            // 再尝试从队列中取出字节。
            else if (queue.try_dequeue(ret))
                return ret;
            // 下一个字节尚未到来。
            return std::nullopt;
        }
        /**
         * @brief 核心线程回退一个字节到缓冲区。
         */
        void unput(byte_t byte) { unput_bytes.push_back(byte); }

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

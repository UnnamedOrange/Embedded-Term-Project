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

#include <data_source/data_source_base.h>
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

    protected:
        /**
         * @brief 异步发送字节流。
         *
         * @note 子线程调用该函数，将数据保存到缓冲区中，待核心线程处理。
         */
        void async_push(const byte_array_t& byte_array);

    protected:
        /**
         * @brief 核心线程读取一个字节。
         *
         * @return std::optional<byte_t>
         * 如果缓冲区中存在字节，则返回该字节。否则返回空。
         */
        std::optional<byte_t> next_byte();
        /**
         * @brief 核心线程回退一个字节到缓冲区。
         */
        void unput_byte(byte_t byte);

    public:
        /**
         * @brief 根据当前状态对数据源进行下一次读取。
         */
        virtual void read_data_source(
            const std::shared_ptr<data_source_base>& data_source) = 0;
        /**
         * @brief 核心线程获取裁剪后的下一个字节流。
         *
         * @return std::optional<byte_array_t>
         * 如果缓冲区内的字节流可以完整裁剪，返回裁剪后的字节流。否则返回空。
         */
        virtual std::optional<byte_array_t> next() = 0;
    };
} // namespace user

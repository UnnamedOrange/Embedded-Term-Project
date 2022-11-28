/**
 * @file stream_to_record_base.h
 * @author UnnamedOrange
 * @brief 记录提取器基类。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <any>
#include <memory>
#include <vector>

#include "record_receive_i.h"
#include <data_source/data_source_utility.h>
#include <source_to_stream/source_to_stream_base.h>
#include <utils/interface_register.hpp>

namespace user
{
    /**
     * @brief 记录提取器基类。
     */
    class stream_to_record_base
    {
    private:
        using reg_t = utils::interface_register<record_receive_i>;
        reg_t reg; // 注册的接收记录的对象。
        // 将该数据流裁剪器作为数据流来源。
        std::weak_ptr<source_to_stream_base> source_to_stream_object;

    public:
        virtual ~stream_to_record_base() = default;

    public:
        /**
         * @brief 注册接收记录的对象。
         */
        void register_interface(const reg_t::shared_t& interface)
        {
            reg.push_back(interface);
        }
        /**
         * @brief 取消注册某个接收记录的对象一次。
         * 如果对象未注册，则不进行任何操作。
         */
        void unregister_interface(const reg_t::shared_t& interface)
        {
            reg.unregister_once(interface);
        }

    public:
        /**
         * @brief 设置要读取的数据流裁剪器。
         */
        void set_source_to_stream(
            const std::shared_ptr<source_to_stream_base>& source)
        {
            source_to_stream_object = source;
        }

    public:
        /**
         * @brief 将数据流转换为特定格式的记录，打包后返回。
         */
        virtual std::any stream_to_record(const byte_array_t& stream) = 0;

    public:
        /**
         * @brief 从数据流裁剪器读一次字节流，并将其转换为记录，
         * 通知给所有注册的对象。
         * 如果读取失败，或者没有字节流需要读取，则什么也不做。
         *
         * @return 是否成功得到记录。任何之前的失败都会导致返回 false。
         */
        bool read_once()
        {
            auto shared_source_to_stream = source_to_stream_object.lock();
            if (!shared_source_to_stream) // lock 失败可以统一归为此类。
                return false;

            auto bytes = shared_source_to_stream->next();
            if (!bytes) // 没有读取到字节流。
                return false;

            auto record = stream_to_record(*bytes);
            // 通知所有注册的对象。
            for (const auto& record_receiver : reg)
            {
                auto shared_record_receiver = record_receiver.lock();
                if (!shared_record_receiver)
                    continue;
                shared_record_receiver->record_receive(record);
            }
            return true;
        }

        /**
         * @brief 从数据流裁剪器读字节流，并将其转换为记录，
         * 通知给所有注册的对象，直到单次读取失败。
         *
         * @see read_once
         */
        void read_all()
        {
            while (read_once())
                ;
        }
    };
} // namespace user

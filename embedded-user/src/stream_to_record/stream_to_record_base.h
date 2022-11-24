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
#include <utils/interface_register.hpp>

namespace user
{
    /**
     * @brief 记录提取器基类。
     *
     * @todo 设计成员变量。
     */
    class stream_to_record_base
    {
    private:
        using reg_t = utils::interface_register<record_receive_i>;
        reg_t reg; // 注册的接收记录的对象。
        // TODO: 设计成员变量。

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
         * @brief 将数据流转换为特定格式的记录，打包后返回。
         */
        virtual std::any stream_to_record(const byte_array_t& stream) = 0;
    };
} // namespace user

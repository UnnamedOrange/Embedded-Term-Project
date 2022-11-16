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
        std::vector<std::weak_ptr<record_receive_i>> registered_receivers;
        // TODO: 设计成员变量。

    public:
        virtual ~stream_to_record_base() = default;

    public:
        /**
         * @brief 注册接收记录的对象。
         */
        void register_receiver(std::shared_ptr<record_receive_i> receiver)
        {
            registered_receivers.push_back(receiver);
        }
        /**
         * @brief 取消注册接收记录的对象。如果对象未注册，则不进行任何操作。
         */
        void unregister_receiver(std::shared_ptr<record_receive_i> receiver)
        {
            for (auto it = registered_receivers.begin();
                 it != registered_receivers.end(); ++it)
                if ((*it).expired() || (*it).lock() == receiver)
                {
                    registered_receivers.erase(it);
                    break;
                }
        }

    public:
        /**
         * @brief 将数据流转换为特定格式的记录，打包后返回。
         */
        virtual std::any stream_to_record(const byte_array_t& stream) = 0;
    };
} // namespace user

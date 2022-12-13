/**
 * @file realtime_data.h
 * @author UnnamedOrange
 * @brief 向远程发送信息的接口。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <any>
#include <cstdint>

#include <http/client/requests.h>

#include <stream_to_record/record_receive_i.h>

namespace user
{
    class realtime_data : virtual public record_receive_i
    {
    public:
        void record_receive(const std::any& record) override
        {
            // 将 record 看作 32 位有符号整数。
            const auto& value = std::any_cast<const int32_t&>(record);

            // TODO: 发送数据。
        }
    };
} // namespace user

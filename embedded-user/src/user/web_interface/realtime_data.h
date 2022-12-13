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
#include <chrono>
#include <cstdint>
#include <string>
#include <string_view>
#include <thread>

#include <http/client/requests.h>
#include <readerwriterqueue.h>

#include <stream_to_record/record_receive_i.h>
#include <utils/system_clock.hpp>

namespace user
{
    /**
     * @brief 向远程发送信息的接口。仅作演示，只能接受 32 位有符号整数。
     */
    class realtime_data : virtual public record_receive_i
    {
    private:
        // TODO: 使用实际地址。
        static constexpr std::string_view url = "127.0.0.1:11451";

    private:
        inline static int max_key = 0; // 目前最大的 key，用于生成下一个 key。
        int key{};                     // 用于区分不同的波形。
        int idx{};                     // 数据点序号计数器。

    private:
        moodycamel::ReaderWriterQueue<int32_t> queue; // 待发送的数据。

    private:
        bool exit_requested{};
        std::thread background_thread{&realtime_data::background_routine, this};
        // 后台线程，发送队列中的消息。
        void background_routine()
        {
            while (!exit_requested)
            {
                hv::Json data;
                int32_t value;
                while (queue.try_dequeue(value))
                    data[std::to_string(idx++)] = value;

                http_headers headers;
                headers["Content-Type"] = "application/json";
                headers["data_key"] = key;
                requests::post(url.data(), data.dump(), headers);
                std::this_thread::sleep_for(100ms);
            }
        }

    public:
        ~realtime_data()
        {
            exit_requested = true;
            background_thread.join();
        }

    public:
        void record_receive(const std::any& record) override
        {
            // 将 record 看作 32 位有符号整数。
            const auto& value = std::any_cast<const int32_t&>(record);
            queue.enqueue(value);
        }
    };
} // namespace user

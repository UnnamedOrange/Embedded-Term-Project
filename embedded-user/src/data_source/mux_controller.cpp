/**
 * @file mux_controller.cpp
 * @author UnnamedOrange
 * @brief 数据源多路复用控制器。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "mux_controller.h"

#include <utils/owner_compare.hpp>

using namespace user;

void mux_controller::background_routine()
{
    while (!exit_requested)
    {
        auto delete_expired_bindings = [&]() { // 删除已经过期的绑定。
            for (auto it = bindings.begin(); it != bindings.end();)
            {
                if (it->first.expired() || it->second.expired())
                    it = bindings.erase(it);
                else
                    ++it;
            }
        };
        auto enroll_bindings_to_add =
            [&]() { // 将待新增的数据源-数据流裁剪器加入到绑定列表中。
                key_value_t kv;
                while (bindings_to_add.try_dequeue(kv))
                    bindings.insert(kv);
            };
        auto poll_data_sources = [&]() { // 轮询数据源。
            std::weak_ptr<data_source_base> previous_data_source;
            byte_array_t previous_byte_array;
            for (auto& [data_source, source_to_stream_object] : bindings)
            {
                // 如果数据源是另一个，则读取。
                if (!utils::owner_equal(data_source, previous_data_source))
                {
                    auto data_source_shared = data_source.lock();
                    if (!data_source_shared)
                        continue; // 防止线程安全问题。
                    // 非阻塞读数据源。
                    previous_byte_array = data_source_shared->read();
                    previous_data_source = data_source;
                }
                auto source_to_stream_object_shared =
                    source_to_stream_object.lock();
                if (!source_to_stream_object_shared)
                    continue; // 防止线程安全问题。
                source_to_stream_object_shared->async_push(previous_byte_array);
            }
        };

        delete_expired_bindings();
        enroll_bindings_to_add();
        poll_data_sources();
    }
}

mux_controller::~mux_controller()
{
    exit_requested = true;
    background_thread.join();
}

void mux_controller::bind(
    const std::shared_ptr<data_source_base>& data_source,
    const std::shared_ptr<source_to_stream_base>& source_to_stream_object)
{
    bindings_to_add.enqueue({data_source, source_to_stream_object});
}

/**
 * @file mux_controller.h
 * @author UnnamedOrange
 * @brief 数据源多路复用控制器。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <map>
#include <memory>
#include <thread>
#include <vector>

#include <readerwriterqueue.h>

#include <data_source/data_source_base.h>
#include <source_to_stream/source_to_stream_base.h>

namespace user
{
    /**
     * @brief 数据源多路复用控制器。将创建后台线程不断轮询数据源。
     */
    class mux_controller
    {
    public:
        /**
         * @brief 保存绑定的数据源-数据流裁剪器的多重映射类型。
         */
        using bindings_t =
            std::multimap<std::weak_ptr<data_source_base>,
                          std::weak_ptr<source_to_stream_base>,
                          std::owner_less<std::weak_ptr<data_source_base>>>;
        /**
         * @brief 键值对类型。
         */
        using key_value_t =
            std::pair<bindings_t::key_type, bindings_t::mapped_type>;

    private:
        bindings_t bindings; // 绑定的数据源-数据流裁剪器。
        moodycamel::ReaderWriterQueue<key_value_t>
            bindings_to_add; // 要新增的数据源-数据流裁剪器。

    private:
        bool exit_requested{};
        std::thread background_thread{&mux_controller::background_thread, this};
        // 后台线程。轮询数据源。
        void background_routine()
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
                    for (auto& [data_source, source_to_stream_object] :
                         bindings)
                    {
                        auto owner_equal = [](const auto& a, const auto& b) {
                            return a.owner_before(b) == b.owner_before(a);
                        };
                        // 如果需要，则读取数据源。
                        if (owner_equal(data_source, previous_data_source))
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
                        source_to_stream_object_shared->async_push(
                            previous_byte_array);
                    }
                };

                delete_expired_bindings();
                enroll_bindings_to_add();
                poll_data_sources();
            }
        }

    public:
        ~mux_controller()
        {
            exit_requested = true;
            background_thread.join();
        }

    public:
        /**
         * @brief 绑定数据源和数据流裁剪器。
         */
        void bind(const std::shared_ptr<data_source_base>& data_source,
                  const std::shared_ptr<source_to_stream_base>&
                      source_to_stream_object)
        {
            bindings_to_add.enqueue({data_source, source_to_stream_object});
        }
    };
} // namespace user

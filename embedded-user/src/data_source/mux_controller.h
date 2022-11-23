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
     *
     * @todo 设计数据源多路复用控制器。
     */
    class mux_controller
    {
    private:
        std::multimap<std::weak_ptr<data_source_base>,
                      std::weak_ptr<source_to_stream_base>>
            bindings; // 绑定的数据源-数据流裁剪器。

    private:
        bool exit_requested{};
        std::thread background_thread{&mux_controller::background_thread, this};
        // 后台线程。轮询数据源。
        void background_routine()
        {
            while (!exit_requested)
            {
                // TODO: 实现后台线程。
            }
        }

    public:
        ~mux_controller()
        {
            exit_requested = true;
            background_thread.join();
        }
    };
} // namespace user

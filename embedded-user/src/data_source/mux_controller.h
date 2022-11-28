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
     * @note 内部不保存相关对象的 shared_ptr。
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
        std::thread background_thread{&mux_controller::background_routine,
                                      this};
        // 后台线程。轮询数据源。
        void background_routine();

    public:
        ~mux_controller();

    public:
        /**
         * @brief 绑定数据源和数据流裁剪器。
         */
        void bind(const std::shared_ptr<data_source_base>& data_source,
                  const std::shared_ptr<source_to_stream_base>&
                      source_to_stream_object);
    };
} // namespace user

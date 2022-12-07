/**
 * @file user_main.h
 * @author UnnamedOrange
 * @brief 嵌入式软件核心模块。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <memory>
#include <vector>

#include <data_source/data_source_base.h>
#include <data_source/mux_controller.h>
#include <source_to_stream/source_to_stream_base.h>
#include <stream_to_record/record_writer_base.h>
#include <stream_to_record/stream_to_record_base.h>

namespace user
{
    /**
     * @brief 嵌入式软件核心模块。保存所有的对象。
     */
    class user_main
    {
    protected:
        std::vector<std::shared_ptr<data_source_base>>
            data_sources; // 数据源列表。
        std::vector<std::shared_ptr<source_to_stream_base>>
            source_to_stream_objects; // 数据流裁剪器列表。
        std::vector<std::shared_ptr<stream_to_record_base>>
            stream_to_record_objects; // 记录提取器列表。
        std::vector<std::shared_ptr<record_writer_base>>
            record_writer_objects; // 记录写入器列表。
        mux_controller mux; // 多路复用控制器将数据源和数据流裁剪器连接起来。

    public:
        /**
         * @brief 在多路复用控制器中绑定数据源和数据流裁剪器。
         *
         * @note 需要手动注册数据源和数据流裁剪器，防止被销毁。
         *
         * @see register_object
         */
        void bind(const std::shared_ptr<data_source_base>& data_source,
                  const std::shared_ptr<source_to_stream_base>&
                      source_to_stream_object)
        {
            mux.bind(data_source, source_to_stream_object);
        }

        /**
         * @brief 注册对象，防止对象被销毁。
         */
        void register_object(
            const std::shared_ptr<data_source_base>& data_source)
        {
            data_sources.push_back(data_source);
        }
        /**
         * @brief 注册对象，防止对象被销毁。
         */
        void register_object(const std::shared_ptr<source_to_stream_base>&
                                 source_to_stream_object)
        {
            source_to_stream_objects.push_back(source_to_stream_object);
        }
        /**
         * @brief 注册对象，防止对象被销毁。
         */
        void register_object(const std::shared_ptr<stream_to_record_base>&
                                 stream_to_record_object)
        {
            stream_to_record_objects.push_back(stream_to_record_object);
        }
        /**
         * @brief 注册对象，防止对象被销毁。
         */
        void register_object(
            const std::shared_ptr<record_writer_base>& record_writer_object)
        {
            record_writer_objects.push_back(record_writer_object);
        }

    public:
        /**
         * @brief 调用所有注册的记录提取器的 `read_all` 方法。
         */
        void read_all() const
        {
            for (const auto& t : stream_to_record_objects)
                if (t)
                    t->read_all();
        }
    };
} // namespace user

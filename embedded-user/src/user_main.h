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

namespace user
{
    /**
     * @brief 嵌入式软件核心模块。保存所有的对象。
     */
    class user_main
    {
    private:
        std::vector<std::shared_ptr<data_source_base>>
            data_sources; // 数据源列表。
        std::vector<std::shared_ptr<source_to_stream_base>>
            source_to_stream_objects; // 数据流裁剪器列表。
        mux_controller mux; // 多路复用控制器将数据源和数据流裁剪器连接起来。

    public:
    };
} // namespace user

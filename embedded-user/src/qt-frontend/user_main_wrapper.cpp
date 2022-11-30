/**
 * @file user_main_wrapper.cpp
 * @author UnnamedOrange
 * @brief 真实应用场景下嵌入式软件核心模块的包装。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "user_main_wrapper.h"

// 可用的数据源。
#include <data_source/data_source_dummy.h>

// 可用的数据流裁剪器。
#include <source_to_stream/source_to_stream_dummy.h>

// 可用的记录提取器。
#include <stream_to_record/stream_to_record_dummy.h>

template <typename source_to_stream_t, typename stream_to_record_t>
auto make_generator()
{
    return []() -> source_to_record_t {
        auto source_to_stream = std::make_shared<source_to_stream_t>();
        auto stream_to_record = std::make_shared<stream_to_record_t>();
        stream_to_record->set_source_to_stream(source_to_stream);
        return std::make_pair(source_to_stream, stream_to_record);
    };
}

const reflection_maps_t user_main_wrapper::reflection_maps = {
#include "reflection_def"
};

user_main_wrapper::user_main_wrapper()
{
    // 启动时，注册所有可用的数据源。
    register_available_data_sources();
}

void user_main_wrapper::register_available_data_sources()
{
    register_object(std::make_shared<user::data_source_dummy>());
}

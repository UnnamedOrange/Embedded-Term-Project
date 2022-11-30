/**
 * @file user_main_wrapper.h
 * @author UnnamedOrange
 * @brief 真实应用场景下嵌入式软件核心模块的包装。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <data_source/data_source_base.h>
#include <data_source/data_source_type.h>
#include <source_to_stream/source_to_stream_base.h>
#include <stream_to_record/stream_to_record_base.h>
#include <user_main.h>

using source_to_record_t =
    std::pair<std::shared_ptr<user::source_to_stream_base>,
              std::shared_ptr<user::stream_to_record_base>>;
using generator_t = std::function<source_to_record_t()>;
using reflection_t = std::unordered_map<std::string, generator_t>;
using reflection_maps_t =
    std::unordered_map<user::data_source_type, reflection_t>;

/**
 * @brief 真实应用场景下嵌入式软件核心模块的包装。
 * 准备好可用的数据源、数据流裁剪器、记录提取器。
 */
class user_main_wrapper : public user::user_main
{
public:
    static const reflection_maps_t reflection_maps;

public:
    user_main_wrapper();

private:
    void register_available_data_sources();
};

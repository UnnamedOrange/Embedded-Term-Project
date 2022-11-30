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
 * 辅助记录当前的状态。
 */
class user_main_wrapper : public user::user_main
{
public:
    /**
     * @brief 数据流裁剪器-记录提取器的反射表。由人工指定。
     *
     * @see reflection_def.h
     */
    static const reflection_maps_t reflection_maps;

private:
    // 已经绑定的数据源和数据流裁剪器的下标。
    std::vector<std::vector<std::pair<size_t, size_t>>> binded_pairs;

public:
    user_main_wrapper();

private:
    // 注册某个类型的单个数据源。
    template <typename data_source_t, typename... R>
    void register_data_source(R&&... args)
    {
        register_object(
            std::make_shared<data_source_t>(std::forward<R>(args)...));
    }
    // 注册所有可用的数据源。由人工指定。
    /**
     * @see reflection_def.h
     */
    void register_available_data_sources();

public:
    /**
     * @brief 根据数据源下标和数据流裁剪器-记录提取器反射名绑定。
     * 并更新状态。
     *
     * @param data_source_idx 数据源下标。
     * @see register_available_data_sources
     * @param reflection_name 数据流裁剪器-记录提取器反射名。
     * @see reflection_maps
     * @return std::shared_ptr<user::stream_to_record_base> 记录提取器。
     */
    std::shared_ptr<user::stream_to_record_base> bind(
        size_t data_source_idx, const std::string& reflection_name);

public:
    /**
     * @brief 获取指定下标数据源的类型。
     */
    user::data_source_type get_data_source_type(size_t data_source_idx) const;
    /**
     * @brief 获取数据源名字列表。下标与数据源下标对应。
     */
    const std::vector<std::string> get_data_source_names();
    /**
     * @brief 获取数据源名下的数据流裁剪器列表。
     * 下标与额外保存的数据流裁剪器-记录提取器下标对应。
     */
    const std::vector<std::vector<std::string>> get_source_to_stream_names();
};

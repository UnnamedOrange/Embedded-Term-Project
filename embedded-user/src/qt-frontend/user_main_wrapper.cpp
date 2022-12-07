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

// 可用的记录写入器。
#include <stream_to_record/record_writer_dummy.h>

template <typename source_to_stream_t, typename stream_to_record_t,
          typename record_writer_t>
auto make_generator()
{
    return []() -> source_to_record_t {
        auto source_to_stream = std::make_shared<source_to_stream_t>();
        auto stream_to_record = std::make_shared<stream_to_record_t>();
        stream_to_record->set_source_to_stream(source_to_stream);
        auto record_writer = std::make_shared<record_writer_t>();
        stream_to_record->register_interface(record_writer);
        return std::tuple(source_to_stream, stream_to_record, record_writer);
    };
}

const reflection_maps_t user_main_wrapper::reflection_maps = {
#include "def_reflection.h"
};

user_main_wrapper::user_main_wrapper()
{
    // 启动时，注册所有可用的数据源。
    register_available_data_sources();
    // 注册数据源后，初始化数组大小。
    binded_pairs.resize(data_sources.size());
}

void user_main_wrapper::register_available_data_sources(){
#include "def_data_source.h"
}

std::shared_ptr<user::stream_to_record_base> user_main_wrapper::bind(
    size_t data_source_idx, const std::string& reflection_name)
{
    // 获取数据源。
    auto& data_source = data_sources[data_source_idx];
    // 获取数据源对应的反射函数。
    const auto& generator =
        reflection_maps.at(data_source->type()).at(reflection_name);
    // 生成数据流裁剪器和记录提取器。
    auto [source_to_stream, stream_to_record, record_writer] = generator();
    // 绑定三者。
    stream_to_record->set_source_to_stream(source_to_stream);
    mux.bind(data_source, source_to_stream);

    // 更新状态。
    {
        binded_pairs[data_source_idx].emplace_back(
            source_to_stream_objects.size(), stream_to_record_objects.size());
        source_to_stream_objects.push_back(std::move(source_to_stream));
        stream_to_record_objects.push_back(stream_to_record); // 返回。
        record_writer_objects.push_back(std::move(record_writer));
    }

    return stream_to_record;
}
void user_main_wrapper::erase(size_t data_source_idx,
                              size_t source_to_record_idx)
{
    // 获取数据流裁剪器和记录提取器在数组中的下标。
    const auto& idx_pair = binded_pairs[data_source_idx][source_to_record_idx];
    size_t source_to_stream_idx = idx_pair.first;
    size_t stream_to_record_idx = idx_pair.second;
    // 通过取消强引用进行线程安全的解绑。
    source_to_stream_objects[source_to_stream_idx].reset();
    stream_to_record_objects[stream_to_record_idx].reset();
    record_writer_objects[stream_to_record_idx].reset();
    // TODO: 考虑数组的清扫。

    // 更新状态。
    binded_pairs[data_source_idx].erase(binded_pairs[data_source_idx].begin() +
                                        source_to_record_idx);
}

user::data_source_type user_main_wrapper::get_data_source_type(
    size_t data_source_idx) const
{
    return data_sources[data_source_idx]->type();
}
const std::vector<std::string> user_main_wrapper::get_data_source_names()
{
    std::vector<std::string> names;
    for (const auto& data_source : data_sources)
    {
        names.push_back(data_source->name());
    }
    return names;
}
const std::vector<std::vector<std::string>> user_main_wrapper::
    get_source_to_stream_names()
{
    std::vector<std::vector<std::string>> names(data_sources.size());
    for (size_t i = 0; i < names.size(); i++)
    {
        const auto& idx_vec = binded_pairs[i];
        auto& name_vec = names[i];
        for (size_t j = 0; j < idx_vec.size(); j++)
            name_vec.emplace_back(
                source_to_stream_objects[idx_vec[j].first]->name());
    }
    return names;
}

std::shared_ptr<user::stream_to_record_base> user_main_wrapper::
    get_stream_to_record(size_t data_source_idx,
                         size_t source_to_record_idx) const
{
    return stream_to_record_objects
        [binded_pairs[data_source_idx][source_to_record_idx].second];
}

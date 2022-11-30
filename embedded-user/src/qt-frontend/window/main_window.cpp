/**
 * @file main_window.cpp
 * @author UnnamedOrange
 * @brief Qt 前端主窗口。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include <memory>

#include "main_window.h"

main_window::main_window(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    update_tree_sources();
}

void main_window::update_tree_sources()
{
    // 清空。
    ui.tree_sources->clear();

    // 获取列表。
    auto& data_sources = main_module.get_data_source_names();
    auto& source_to_stream_objects = main_module.get_source_to_stream_names();

    // 插入项。
    for (size_t i = 0; i < data_sources.size(); i++)
    {
        auto data_source_item = new QTreeWidgetItem(ui.tree_sources);
        data_source_item->setText(0, data_sources[i].c_str());
        const auto& vec = source_to_stream_objects[i];
        for (size_t j = 0; j < vec.size(); j++)
        {
            auto source_to_stream_item = new QTreeWidgetItem(data_source_item);
            source_to_stream_item->setText(0, vec[j].c_str());
        }
    }
}

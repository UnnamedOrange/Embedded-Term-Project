/**
 * @file main_window.cpp
 * @author UnnamedOrange
 * @brief Qt 前端主窗口。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "main_window.h"

#include <QTreeWidgetItem>

#include "add_source_to_record.h"

main_window::main_window(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    update_tree_sources();
    // TODO: 解决效果不完美的问题。
    connect(ui.tree_sources, &QTreeWidget::currentItemChanged, this,
            &main_window::_on_tree_sources_selection_changed);
    connect(ui.tree_sources, &QTreeWidget::itemSelectionChanged, this,
            &main_window::_on_tree_sources_selection_changed);
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
        data_source_item->setExpanded(true);
    }
}

void main_window::on_button_remove_source_clicked() {}
void main_window::on_button_add_source_clicked()
{
    // 获取现在选择的数据源的序号。
    auto selected_item = ui.tree_sources->currentItem();
    int data_source_idx = ui.tree_sources->indexOfTopLevelItem(selected_item);
    assert(data_source_idx != -1);

    // 弹出对话框，选择要添加的记录提取器。
    add_source_to_record dialog(
        main_module.get_data_source_type(data_source_idx), this);
    if (dialog.exec() == QDialog::Rejected)
        return;
    auto selected_type = dialog.get_selected_type();

    // 将记录提取器添加到主模块。
    auto stream_to_record = main_module.bind(data_source_idx, selected_type);

    // 更新树。
    update_tree_sources();

    // TODO: 绑定记录接收器。
}
void main_window::_on_tree_sources_selection_changed()
{
    auto item = ui.tree_sources->currentItem();
    if (item == nullptr)
    {
        // 没有选中项目，禁用所有按钮。
        ui.button_remove_source->setEnabled(false);
        ui.button_add_source->setEnabled(false);
        return;
    }
    else if (item->parent() == nullptr)
    {
        // 选中了数据源，启用添加按钮，禁用删除按钮。
        ui.button_remove_source->setEnabled(false);
        ui.button_add_source->setEnabled(true);
    }
    else
    {
        // 选中了数据源的流对象，启用删除按钮，禁用添加按钮。
        ui.button_remove_source->setEnabled(true);
        ui.button_add_source->setEnabled(false);
    }
}

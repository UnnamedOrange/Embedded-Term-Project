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
#include "points_view.h"

main_window::main_window(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);

    // 初始化。
    update_tree_sources();

    // 设置信号槽。
    // TODO: 解决效果不完美的问题。
    {
        connect(ui.tree_sources, &QTreeWidget::currentItemChanged, this,
                &main_window::_on_tree_sources_selection_changed);
        connect(ui.tree_sources, &QTreeWidget::itemSelectionChanged, this,
                &main_window::_on_tree_sources_selection_changed);

        connect(ui.list_views, &QListWidget::currentItemChanged, this,
                &main_window::_on_list_views_selection_changed);
        connect(ui.list_views, &QListWidget::itemSelectionChanged, this,
                &main_window::_on_list_views_selection_changed);
    }

    // 设置定时器。
    background_timer_id = startTimer(0);
}

void main_window::timerEvent(QTimerEvent* event)
{
    if (event->timerId() == background_timer_id)
    {
        main_module.read_all();
    }
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

void main_window::on_button_remove_source_clicked()
{
    // 获取记录提取器对应数据源的序号。
    auto selected_item = ui.tree_sources->currentItem();
    assert(selected_item != nullptr);
    auto parent_item = selected_item->parent();
    assert(parent_item != nullptr);
    auto data_source_index = ui.tree_sources->indexOfTopLevelItem(parent_item);
    auto source_to_stream_index = parent_item->indexOfChild(selected_item);

    // 删除记录提取器。
    main_module.erase(data_source_index, source_to_stream_index);

    // 更新树。
    update_tree_sources();
}
void main_window::on_button_add_source_clicked()
{
    auto selected_item = ui.tree_sources->currentItem();
    assert(selected_item != nullptr);

    // 判断是数据源还是记录提取器。
    if (selected_item->parent() == nullptr) // 数据源。
    {
        // 新建记录提取器。

        // 获取现在选择的数据源的序号。
        int data_source_idx =
            ui.tree_sources->indexOfTopLevelItem(selected_item);
        assert(data_source_idx != -1);

        // 弹出对话框，选择要添加的记录提取器。
        add_source_to_record dialog(
            main_module.get_data_source_type(data_source_idx), this);
        if (dialog.exec() == QDialog::Rejected)
            return;
        auto selected_type = dialog.get_selected_type();

        // 将记录提取器添加到主模块。
        auto stream_to_record =
            main_module.bind(data_source_idx, selected_type);

        // 更新树。
        update_tree_sources();

        // TODO: 绑定记录接收器。
    }
    else // 记录提取器。
    {
        // 新建视图。

        // 获取现在选择的数据源和记录提取器的序号。
        auto parent_item = selected_item->parent();
        int data_source_idx = ui.tree_sources->indexOfTopLevelItem(parent_item);
        int source_to_stream_idx = parent_item->indexOfChild(selected_item);

        // 获取记录提取器的强引用。
        auto stream_to_record = main_module.get_stream_to_record(
            data_source_idx, source_to_stream_idx);

        // 新建视图，并绑定记录提取器。
        auto view = new points_view(stream_to_record, ui.list_views);
        stream_to_record->register_interface(view->get_record_receive());

        // 将项目加入到列表。
        auto item = new QListWidgetItem(ui.list_views);
        item->setSizeHint(view->size());
        ui.list_views->addItem(item);
        ui.list_views->setItemWidget(item, view);
    }
}
void main_window::on_button_remove_view_clicked()
{
    // 获取当前选中的项目。
    auto selected_item = ui.list_views->currentItem();
    assert(selected_item != nullptr);

    // 获取视图。
    auto view =
        dynamic_cast<points_view*>(ui.list_views->itemWidget(selected_item));
    assert(view != nullptr);

    // 删除视图。
    delete view;
    delete selected_item;
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
        // 选中了数据源的流对象，启用删除按钮和添加按钮。
        ui.button_remove_source->setEnabled(true);
        ui.button_add_source->setEnabled(true);
    }
}
void main_window::_on_list_views_selection_changed()
{
    int index = ui.list_views->currentRow();
    if (~index && !ui.list_views->currentItem()->isSelected())
        index = -1;
    // 如果选中了视图，则允许删除视图。
    if (index == -1)
        ui.button_remove_view->setEnabled(false);
    else
        ui.button_remove_view->setEnabled(true);
}

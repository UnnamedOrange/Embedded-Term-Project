/**
 * @file add_source_to_record.cpp
 * @author UnnamedOrange
 * @brief 选择要添加的记录提取器的对话框。
 *
 * @copyright Copyright (c) 2022 UnnamedOrange.
 */

#include "add_source_to_record.h"

#include <data_source/data_source_type.h>
#include <user_main_wrapper.h>

add_source_to_record::add_source_to_record(
    user::data_source_type data_source_idx, QWidget* parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    const auto& m = user_main_wrapper::reflection_maps.at(data_source_idx);
    for (const auto& [k, v] : m)
        ui.list->addItem(k.c_str());
}

void add_source_to_record::on_buttonBox_rejected() { close(); }
void add_source_to_record::on_list_itemDoubleClicked(QListWidgetItem* item)
{
    selected_type = item->text();
    done(QDialog::Accepted);
}

std::string add_source_to_record::get_selected_type()
{
    return selected_type.toStdString();
}

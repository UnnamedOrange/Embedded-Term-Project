/**
 * @file add_source_to_record.h
 * @author UnnamedOrange
 * @brief 选择要添加的记录提取器的对话框。
 *
 * @copyright Copyright (c) 2022 UnnamedOrange.
 */

#pragma once

#include "ui_add_source_to_record.h"

#include <QDialog>
#include <QString>

#include <data_source/data_source_type.h>

class add_source_to_record : public QDialog
{
    Q_OBJECT

private:
    Ui::add_source_to_record ui;

public:
    add_source_to_record(user::data_source_type data_source_idx,
                         QWidget* parent = nullptr);

private slots:
    void on_buttonBox_rejected();
    void on_list_itemDoubleClicked(QListWidgetItem* item);

private:
    QString selected_type;

public:
    std::string get_selected_type();
};

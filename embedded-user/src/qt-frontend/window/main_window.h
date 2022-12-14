/**
 * @file main_window.h
 * @author UnnamedOrange
 * @brief Qt 前端主窗口。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include "ui_main_window.h"

#include <QMainWindow>

#include <user_main_wrapper.h>

class main_window : public QMainWindow
{
    Q_OBJECT

private:
    Ui::main_window ui;

private:
    user_main_wrapper main_module;
    int background_timer_id{}; // 更新 main_module 的定时器。闲时运行。
    int clean_timer_id{};      // 清理 view 的定时器。

public:
    main_window(QWidget* parent = nullptr);

protected:
    void timerEvent(QTimerEvent* event) override;

private slots:
    void on_button_remove_source_clicked();
    void on_button_add_source_clicked();
    void on_button_remove_view_clicked();
    void _on_tree_sources_selection_changed();
    void _on_list_views_selection_changed();

private:
    void update_tree_sources();
};

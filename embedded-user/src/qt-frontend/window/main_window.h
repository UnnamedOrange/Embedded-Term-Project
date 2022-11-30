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

public:
    main_window(QWidget* parent = nullptr);

private:
    void update_tree_sources();
};

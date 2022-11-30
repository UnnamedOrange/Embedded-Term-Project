/**
 * @file points_view.h
 * @author UnnamedOrange
 * @brief 逐点绘制的图。
 *
 * @copyright Copyright (c) 2022 UnnamedOrange.
 */

#pragma once

#include "ui_points_view.h"

#include <QWidget>

class points_view : public QWidget
{
    Q_OBJECT

private:
    Ui::points_view ui;

public:
    points_view(QWidget* parent = nullptr);
};

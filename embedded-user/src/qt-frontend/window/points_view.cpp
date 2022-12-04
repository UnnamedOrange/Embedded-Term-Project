/**
 * @file points_view.cpp
 * @author UnnamedOrange
 * @brief 逐点绘制的图。
 *
 * @copyright Copyright (c) 2022 UnnamedOrange.
 */

#include "points_view.h"

#include <QPainter>

points_view::points_view(
    std::shared_ptr<user::stream_to_record_base> stream_to_record,
    QWidget* parent)
    : QWidget(parent), stream_to_record(stream_to_record)
{
    ui.setupUi(this);
}

std::shared_ptr<user::record_receive_i> points_view::get_record_receive() const
{
    return record_receive;
}

void points_view::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    // 绘制坐标轴。
    painter.drawLine(0, height() / 2, width(), height() / 2);
    painter.drawLine(width() / 2, 0, width() / 2, height());
    // 绘制点。
    for (size_t i = 0; i < points.size(); i++)
    {
        painter.drawPoint(i, points[i]);
    }
}

size_t points_view::capacity() const
{
    // TODO: 计算实际容量。
    return width();
}

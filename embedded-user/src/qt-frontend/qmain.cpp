/**
 * @file qmain.cpp
 * @author UnnamedOrange
 * @brief 嵌入式 Qt 前端程序入口。
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include <QApplication>

#include <window/main_window.h>

int main(int argn, char** argv)
{
    QApplication app(argn, argv);
    main_window window;
    window.showFullScreen();
    return app.exec();
}

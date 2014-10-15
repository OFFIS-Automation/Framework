// OFFIS Automation Framework
// Copyright (C) 2013 OFFIS e.V.
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QDockWidget>
class QXmlStreamWriter;
class QXmlStreamReader;

namespace Ui {
    class VideoWidget;
}

class PortListener;
class VideoDisplayWidget;

class VideoWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit VideoWidget(QWidget *parent = 0);
    ~VideoWidget();

    virtual void writeCurrentConfig(QXmlStreamWriter& writer);
    virtual void readConfig(QXmlStreamReader& reader);

public slots:
    void changeView(VideoDisplayWidget* source, bool updateMaximized = true);
    void updateMaxDisplays(int maxDisplays);
    void startCapture(int fps, int widgetIndex = 0);
    void endCapture(const QString& filename, int widgetIndex = 0);
    void saveScreenshot(const QString& filename, int widgetIndex = 0);

private:
    void createDisplay();
    Ui::VideoWidget *ui;
    bool mMaximized;
    int mMaxVisibleWidgets;
    QList<VideoDisplayWidget*> mWidgets;
};

#endif // VIDEOWIDGET_H

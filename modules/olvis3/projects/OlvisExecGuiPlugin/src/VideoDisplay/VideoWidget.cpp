// OFFIS Automation Framework
// Copyright (C) 2013-2017 OFFIS e.V.
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

#include "VideoWidget.h"
#include "ui_VideoWidget.h"

#include "VideoDisplayScrollArea.h"
#include "VideoDisplayWidget.h"

#include <core/FilterInfo.h>

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include <QSettings>

#include <QDebug>

VideoWidget::VideoWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::VideoWidget),
    mMaximized(false)
{
    ui->setupUi(this);

    foreach (VideoDisplayWidget* widget, mWidgets) {
        connect(widget, SIGNAL(doubleClick(VideoDisplayWidget*)), SLOT(changeView(VideoDisplayWidget*)));
    }
    updateMaxDisplays(QSettings().value("OlvisExecGui/maxDisplays", 4).toInt());
}

VideoWidget::~VideoWidget()
{
    delete ui;
}

void VideoWidget::changeView(VideoDisplayWidget* source, bool updateMaximized)
{
    if(updateMaximized)
        mMaximized = !mMaximized;

    int counter = 0;
    foreach (VideoDisplayWidget* widget, mWidgets) {
        if (!mMaximized) {
            if (counter < mMaxVisibleWidgets) widget->scrollArea()->show();
            else widget->scrollArea()->hide();
        } else {
            if (widget != source) widget->scrollArea()->hide();
        }
        counter++;
    }
    ui->gridLayout->invalidate();


}

void VideoWidget::updateMaxDisplays(int maxDisplays)
{
    mMaxVisibleWidgets = maxDisplays;
    while(mWidgets.size() < maxDisplays)
        createDisplay();
    QSettings().setValue("OlvisExecGui/maxDisplays", maxDisplays);
    changeView(0,mMaximized);
}

void VideoWidget::startCapture(int fps, int widgetIndex)
{
    if(mWidgets.empty())
        return;
    if(widgetIndex > mWidgets.count())
        throw std::runtime_error("widgetIndex out of bounds");

    mWidgets[widgetIndex]->recordVideo(fps);
}

void VideoWidget::endCapture(const QString &filename, int widgetIndex)
{
    if(mWidgets.empty())
        return;
    if(widgetIndex > mWidgets.count())
        throw std::runtime_error("widgetIndex out of bounds");

    mWidgets[widgetIndex]->endVideoRecording(filename);
}

void VideoWidget::saveScreenshot(const QString &filename, int widgetIndex)
{
    if(mWidgets.empty())
        return;
    if(widgetIndex > mWidgets.count())
        throw std::runtime_error("widgetIndex out of bounds");

    mWidgets[widgetIndex]->screenshotWithFileName(filename);
}

void VideoWidget::createDisplay()
{
    VideoDisplayWidget* widget = new VideoDisplayWidget();
    QScrollArea* scrollArea = new VideoDisplayScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setBackgroundRole(QPalette::Shadow);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    widget->setGeometry(QRect(0, 0, 391, 280));
    scrollArea->setWidget(widget);
    //scrollArea->registerWidget(widget);
    // calculate the position, it should be
    // -------------
    // | 0 | 1 | 4 |
    // | 2 | 3 | 5 |
    // | 6 | 7 | 8 |
    //
    int position = mWidgets.size();
    int columns = ui->gridLayout->columnCount();
    int rows = ui->gridLayout->rowCount();
    int available = columns*rows;
    if(position == 0) // add first item
        ui->gridLayout->addWidget(scrollArea, 0, 0);
    else if(position >= available) // add a row or column
    {
        if(columns > rows) // add a row
            ui->gridLayout->addWidget(scrollArea, rows, 0);
        else // add a column
            ui->gridLayout->addWidget(scrollArea, 0, columns);
    }
    else {
        // find the free spot
        int row = rows -1 ;
        int column = columns -1 ;
        if(columns > rows) // find the correct row in the last column
            row = position % (rows*rows);
        else // finmd the correct column in the last row
            column = position % columns;
        ui->gridLayout->addWidget(scrollArea, row, column);
    }
    mWidgets << widget;
    widget->setScrollArea(scrollArea);

    connect(widget, SIGNAL(doubleClick(VideoDisplayWidget*)), SLOT(changeView(VideoDisplayWidget*)));

}

void VideoWidget::writeCurrentConfig(QXmlStreamWriter& writer)
{
    foreach (VideoDisplayWidget* widget, mWidgets) {
        writer.writeStartElement("widget");
        widget->writeCurrentConfig(writer);
        writer.writeEndElement();
    }
}

void VideoWidget::readConfig(QXmlStreamReader& reader)
{
    foreach (VideoDisplayWidget* widget, mWidgets) {
        if (!reader.readNextStartElement()) {
            break;
        }
        if (reader.name() == "widget") {
            widget->readConfig(reader);
        }
    }
}

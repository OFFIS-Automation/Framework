#ifndef SCRATCHWIDGET_H
#define SCRATCHWIDGET_H

#include <QDockWidget>

namespace Ui {
class ScratchWidget;
}

class ScratchWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit ScratchWidget(QWidget *parent = 0);
    ~ScratchWidget();

private slots:
    void updateRcUnits(bool partial);
    void onDockLocationChanged(const Qt::DockWidgetArea &area);

private:
    Ui::ScratchWidget *ui;
};

#endif // SCRATCHWIDGET_H

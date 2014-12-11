#ifndef CUSTOMRCUNITGUI_H
#define CUSTOMRCUNITGUI_H

#include <QWidget>


namespace Ui {
    class CustomRcUnitGui;
}

class CustomRcUnit;

class CustomRcUnitGui : public QWidget
{
    Q_OBJECT

public:
    explicit CustomRcUnitGui(CustomRcUnit* control, QWidget *parent = 0);
    ~CustomRcUnitGui();
signals:
    void connectClicked();
    void disconnectClicked();

private slots:
    void on_connectButton_clicked(bool checked);

private:
    CustomRcUnit* mController;
    Ui::CustomRcUnitGui *ui;
};

#endif // CUSTOMRCUNITGUI_H

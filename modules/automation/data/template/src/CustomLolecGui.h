#ifndef CUSTOMLOLECGUI_H
#define CUSTOMLOLECGUI_H

#include <QWidget>


namespace Ui {
    class CustomLolecGui;
}

class CustomLolec;

class CustomLolecGui : public QWidget
{
    Q_OBJECT

public:
    explicit CustomLolecGui(CustomLolec* control, QWidget *parent = 0);
    ~CustomLolecGui();
signals:
    void connectClicked();
    void disconnectClicked();

private slots:
    void on_connectButton_clicked(bool checked);

private:
    CustomLolec* mController;
    Ui::CustomLolecGui *ui;
};

#endif // CUSTOMLOLECGUI_H

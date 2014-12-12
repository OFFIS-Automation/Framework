#ifndef RCFLAGWIDGETSETTINGSDIALOG_H
#define RCFLAGWIDGETSETTINGSDIALOG_H

#include <QDialog>

class RcFlagWidgetSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit RcFlagWidgetSettingsDialog(QWidget *parent = 0);

signals:
    void hidden();
public slots:
    void setVisible(bool visible);
protected:

};

#endif // RCFLAGWIDGETSETTINGSDIALOG_H

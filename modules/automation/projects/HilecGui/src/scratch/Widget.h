#ifndef SCRATCHWIDGET_H
#define SCRATCHWIDGET_H

#include <memory>

#include <QDockWidget>

#include "ControlScene.h"

namespace Ui
{
class ScratchWidget;
}

namespace Scratch
{

class Widget : public QDockWidget
{
    Q_OBJECT

	public:
		Widget(QWidget* parent = nullptr);

	protected:
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);

	private slots:
		void updateRcUnits(bool);
		void onDockLocationChanged(const Qt::DockWidgetArea&);

	private:
		std::unique_ptr<Ui::ScratchWidget> m_ui;

		ControlScene m_controlFlowScene;
		ControlScene m_utilityScene;
		ControlScene m_functionBlockScene;
};

} // namespace Scratch

#endif // SCRATCHWIDGET_H

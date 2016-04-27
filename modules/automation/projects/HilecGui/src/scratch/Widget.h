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

class StartBlock;

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

		std::unique_ptr<QGraphicsScene> m_programScene;
		std::unique_ptr<ControlScene> m_controlScene;
		std::unique_ptr<ControlScene> m_rcUnitScene;

		StartBlock* m_startBlock{nullptr};

};

} // namespace Scratch

#endif // SCRATCHWIDGET_H

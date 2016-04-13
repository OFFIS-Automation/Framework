#ifndef SCRATCHWIDGET_H
#define SCRATCHWIDGET_H

#include <memory>

#include <QDockWidget>

#include "ProgramScene.h"
#include "ControlScene.h"

// Forward declarations
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

	private slots:
		void updateRcUnits(bool);
		void onDockLocationChanged(const Qt::DockWidgetArea& area);

	private:
		std::unique_ptr<Ui::ScratchWidget> m_ui;

		std::unique_ptr<ProgramScene> m_programScene;
		std::unique_ptr<ControlScene> m_controlScene;

};

} // namespace Scratch

#endif // SCRATCHWIDGET_H

#ifndef SCRATCHWIDGET_H
#define SCRATCHWIDGET_H

#include <memory>
#include <vector>

#include <QDockWidget>

#include "../OlvisSingleton.h"

#include "Gui/ControlScene.h"

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
		void updateTabs(bool partialReload = false);
		void filterCreated(const FilterInfo& info, int insertBefore);

		void onDockLocationChanged(const Qt::DockWidgetArea&);

	private:
		std::unique_ptr<Ui::ScratchWidget> m_ui;

		ControlScene m_controlFlowScene;
		ControlScene m_utilityScene;
		ControlScene m_functionBlockScene;
		size_t m_defaultTabCount;

		std::vector<FilterInfo> m_filter;
};

} // namespace Scratch

#endif // SCRATCHWIDGET_H

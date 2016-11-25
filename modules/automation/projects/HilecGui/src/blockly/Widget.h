#ifndef BLOCKLYWIDGET_H
#define BLOCKLYWIDGET_H

#include <memory>

#include <QDockWidget>

#include "../OlvisSingleton.h"

namespace Ui
{
class BlocklyWidget;
}

namespace Blockly
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
		void updateRcUnits(bool partialReload = false);

		void onDockLocationChanged(const Qt::DockWidgetArea&);

	private:
		std::unique_ptr<Ui::BlocklyWidget> m_ui;
};

} // namespace Blockly

#endif // BLOCKLYWIDGET_H

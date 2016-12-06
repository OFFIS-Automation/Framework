#ifndef BLOCKLYWIDGET_H
#define BLOCKLYWIDGET_H

#include <memory>

#include <QDockWidget>
#include <QtWebChannel>

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

	private:
		static const std::string blockIDPrefix;
		static const std::string fileName;

	public:
		Widget(QWidget* parent = nullptr);
		~Widget();

	protected:
		void keyPressEvent(QKeyEvent *event);
		void keyReleaseEvent(QKeyEvent *event);

	private slots:
		void highlightBlock(QString fileName, int lineNumber);
		void updateRcUnits(bool partialReload = false);
		void onDockLocationChanged(const Qt::DockWidgetArea&);

	public slots:
		void generateCode(const bool debug, const bool execute);

	private:
		std::unique_ptr<Ui::BlocklyWidget> m_ui;
		QWebChannel* webChannel;
};

} // namespace Blockly

#endif // BLOCKLYWIDGET_H

#ifndef FUNCTIONVIEW_H
#define FUNCTIONVIEW_H

#include <memory>

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QGridLayout>

#include "ControlScene.h"
#include "Item.h"
#include "Parameter.h"

namespace Ui {
class ScratchFunctionView;
}

namespace Scratch
{

class FunctionTabWidget: public QTabWidget
{
	Q_OBJECT

	public:
		FunctionTabWidget(QWidget* parent);

		int addTab(QWidget* widget, const QString &label);
		int addTab(QWidget* widget, const QIcon& icon, const QString &label);

	private:
		void addFunction(int index);
};

class StartBlock;

class FunctionView : public QWidget
{
	Q_OBJECT

	friend FunctionTabWidget;

	public:
		using Parameters = std::vector<std::pair<std::string, Item::Type>>;
		using Function = std::tuple<std::string, Item::Type, Parameters>;

	public:
		explicit FunctionView(QWidget *parent = 0, const std::string& name = "main");

		void generateFile();

	private:
		Parameter& addVariable(std::string name, Item::Type type);

	private:
		std::unique_ptr<Ui::ScratchFunctionView> m_ui;

		QGraphicsScene m_programScene;
		ControlScene m_VariablesScene;

		int variableY = 0;

		StartBlock* m_startBlock{nullptr};
};

}

#endif // FUNCTIONVIEW_H

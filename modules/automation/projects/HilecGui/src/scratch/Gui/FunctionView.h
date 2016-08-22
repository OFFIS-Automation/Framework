#ifndef FUNCTIONVIEW_H
#define FUNCTIONVIEW_H

#include <memory>

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QGridLayout>

#include "ControlScene.h"
#include "../Items/Item.h"
#include "../Items/Parameter/Parameter.h"
#include "../Items/Argument/ArgumentItem.h"

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

		void setFunctionBlocks(ControlScene* functionBlocks);

	signals:
		void newFunctionBlock(ArgumentItem* item);

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

		void print(std::ostream& stream);

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

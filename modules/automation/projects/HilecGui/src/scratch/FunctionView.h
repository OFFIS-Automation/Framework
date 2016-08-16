#ifndef FUNCTIONVIEW_H
#define FUNCTIONVIEW_H

#include <memory>

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QGridLayout>

#include "ControlScene.h"
#include "Item.h"

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
		explicit FunctionView(QWidget *parent = 0);

		void generateFile();

	private:
		void addVariable(std::string name, Item::Type type);

	private:
		std::unique_ptr<Ui::ScratchFunctionView> m_ui;

		QGraphicsScene m_programScene;
		ControlScene m_VariablesScene;

		int variableY = 0;

		StartBlock* m_startBlock{nullptr};
};

}

#endif // FUNCTIONVIEW_H

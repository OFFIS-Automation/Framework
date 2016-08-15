#ifndef FUNCTIONVIEW_H
#define FUNCTIONVIEW_H

#include <memory>

#include <QWidget>

#include "ControlScene.h"

namespace Ui {
class ScratchFunctionView;
}

namespace Scratch
{

class StartBlock;

class FunctionView : public QWidget
{
		Q_OBJECT

	public:
		explicit FunctionView(QWidget *parent = 0);

		void generateFile();

	private slots:
		void newVariable();

	private:
		std::unique_ptr<Ui::ScratchFunctionView> m_ui;

		QGraphicsScene m_programScene;
		ControlScene m_VariablesScene;

		int variableY = 0;

		StartBlock* m_startBlock{nullptr};
};

}

#endif // FUNCTIONVIEW_H

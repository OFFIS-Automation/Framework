#include "Widget.h"

#include "ui_ScratchWidget.h"
#include "../HilecSingleton.h"

#include "core/RcUnitHelp.h"

#include <QKeyEvent>

#include "FunctionView.h"
#include "NewVariableDialog.h"

#include "WhileBlock.h"
#include "IfElseBlock.h"
#include "PassBlock.h"
#include "ArgumentBlock.h"
#include "ArgumentCondition.h"
#include "ArgumentNumber.h"
#include "TrueCondition.h"
#include "PiNumber.h"

namespace Scratch
{

Widget::Widget(QWidget *parent)
	: QDockWidget(parent),
	m_ui(std::make_unique<Ui::ScratchWidget>())
{
	m_ui->setupUi(this);

	m_ui->controlFlowView->setScene(&m_controlFlowScene);
	m_ui->utilityView->setScene(&m_utilityScene);

	auto functionView = new FunctionView(m_ui->main);
	m_ui->gridLayout_5->addWidget(functionView);

	auto whileBlock = new WhileBlock();
	m_controlFlowScene.addItem(whileBlock);

	auto ifElseBlock = new IfElseBlock();
	ifElseBlock->setPos(whileBlock->m_width + 30, 0);
	m_controlFlowScene.addItem(ifElseBlock);

	auto passBlock = new TrueCondition();
	passBlock->setPos(ifElseBlock->pos().x() + ifElseBlock->m_width + 30, 0);
	m_utilityScene.addItem(passBlock);

	auto trueCondiftion = new PassBlock();
	trueCondiftion->setPos(passBlock->pos().x() + passBlock->m_width + 30, 0);
	m_utilityScene.addItem(trueCondiftion);

	auto piNumber = new PiNumber();
	piNumber->setPos(trueCondiftion->pos().x() + trueCondiftion->m_width + 30, 0);
	m_utilityScene.addItem(piNumber);

	// Signal / slot connections
	connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateRcUnits(bool)));
	connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), SLOT(onDockLocationChanged(Qt::DockWidgetArea)));
}

void Widget::keyPressEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_G)
		event->accept();
	else
		event->ignore();
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
	if(event->key() == Qt::Key_G)
	{
		event->accept();

		// TODO
	}

	event->ignore();
}

void Widget::updateRcUnits(bool)
{
	const auto& hilec = *HilecSingleton::hilec();

	for (size_t i = 2; i < m_ui->controlWidget->count(); ++i)
		m_ui->controlWidget->removeTab(i);

	for (const auto& name : hilec.rcUnits())
	{
		auto help = hilec.getUnitHelp(name);

		if (help.methods.empty())
			continue;

		auto view = new QGraphicsView(this);
		auto scene = new ControlScene(this);

		view->setScene(scene);
		m_ui->controlWidget->addTab(view, help.unitName);

		int y = 0;

		for (const auto& method : help.methods)
		{
			if (method.hiddenForScratch)
				continue;

			auto initilizeArgumentItem = [&](auto &argumentItem, auto&& position)
			{
				argumentItem->setToolTip(method.longDesc);

				for (auto parameter : method.parameters)
					if (parameter.typeName == "bool")
						argumentItem->addArgument(parameter.name.toStdString(), Item::Type::Condition);
					else if (parameter.typeName == "double")
						argumentItem->addArgument(parameter.name.toStdString(), Item::Type::Number);
					else
						return false;

				argumentItem->setPos(position);

				scene->addItem(argumentItem);

				return true;
			};

			auto argumentBlock =
				new ArgumentBlock((help.unitName + "." + method.name).toStdString());

			if (!initilizeArgumentItem(argumentBlock, QPoint(0, y)))
			{
				delete argumentBlock;

				continue;
			}

			if (!method.hasReturn)
			{
				ArgumentItem* argumentItem;

				if (method.returnParameter.typeName == "bool")
					argumentItem = new ArgumentCondition((help.unitName + "." + method.name).toStdString());
				else if (method.returnParameter.typeName == "double")
					argumentItem = new ArgumentNumber((help.unitName + "." + method.name).toStdString());

				initilizeArgumentItem(argumentItem, QPoint(argumentBlock->m_width + 30, y));
			}

			y += argumentBlock->m_height + 30;
		}
	}
}

void Widget::onDockLocationChanged(const Qt::DockWidgetArea&)
{}

} // namespace Scratch

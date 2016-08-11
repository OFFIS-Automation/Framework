#include "Widget.h"

#include "ui_ScratchWidget.h"
#include "../HilecSingleton.h"

#include <iostream>
#include <sstream>

#include <core/RcUnitHelp.h>
#include <QKeyEvent>

#include "FrameBlocks.h"
#include "WhileBlock.h"
#include "IfElseBlock.h"
#include "PassBlock.h"
#include "ArgumentBlock.h"
#include "ArgumentCondition.h"
#include "ArgumentNumber.h"
#include "TrueCondition.h"
#include "PiNumber.h"

#define ROOT_TYPE_OFFSET 1
#define CHILD_TYPE_OFFSET 2

namespace Scratch
{

Widget::Widget(QWidget *parent)
	: QDockWidget(parent),
	m_ui(std::make_unique<Ui::ScratchWidget>()),
	m_programScene(std::make_unique<QGraphicsScene>(this)),
	m_controlFlowScene(std::make_unique<ControlScene>(this)),
	m_utilityScene(std::make_unique<ControlScene>(this))
{
	m_ui->setupUi(this);
	m_ui->programView->setScene(m_programScene.get());
	m_ui->controlFlowView->setScene(m_controlFlowScene.get());
	m_ui->utilityView->setScene(m_utilityScene.get());

	// Program scene
	m_startBlock = new StartBlock();
	m_programScene->addItem(m_startBlock);

	auto endBlock = new EndBlock();
	m_startBlock->addBelow(*endBlock);
	m_programScene->addItem(m_startBlock);

	// Control scene
	auto whileBlock = new WhileBlock();
	m_controlFlowScene->addItem(whileBlock);

	auto ifElseBlock = new IfElseBlock();
	ifElseBlock->setPos(whileBlock->m_width + 30, 0);
	m_controlFlowScene->addItem(ifElseBlock);

	auto passBlock = new TrueCondition();
	passBlock->setPos(ifElseBlock->pos().x() + ifElseBlock->m_width + 30, 0);
	m_utilityScene->addItem(passBlock);

	auto trueCondiftion = new PassBlock();
	trueCondiftion->setPos(passBlock->pos().x() + passBlock->m_width + 30, 0);
	m_utilityScene->addItem(trueCondiftion);

	auto piNumber = new PiNumber();
	piNumber->setPos(trueCondiftion->pos().x() + trueCondiftion->m_width + 30, 0);
	m_utilityScene->addItem(piNumber);

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
	if(event->key() != Qt::Key_G)
	{
		event->ignore();

		return;
	}

	event->accept();

	std::stringstream generatedFile;

	auto printMethodForAllRCUnits = [&](auto & methodName)
	{
		const auto& hilec = *HilecSingleton::hilec();

		for (const auto& name : hilec.rcUnits())
		{
			auto help = hilec.getUnitHelp(name);

			if (help.methods.empty())
				continue;

			for (const auto& method : help.methods)
			{
				if (method.hiddenForScratch)
					continue;

				generatedFile << help.unitName.toStdString() + "." + methodName + "()\n";

				break;
			}

			break;
		}
	};

	printMethodForAllRCUnits("acquire");
	m_startBlock->print(generatedFile);
	printMethodForAllRCUnits("release");

	std::cout << generatedFile.str();
	std::cout.flush();
}

void Widget::updateRcUnits(bool)
{
	const auto& hilec = *HilecSingleton::hilec();

	m_ui->rcUnitsView->clear();

	for (const auto& name : hilec.rcUnits())
	{
		auto help = hilec.getUnitHelp(name);

		if (help.methods.empty())
			continue;

		auto view = new QGraphicsView(this);
		auto scene = new ControlScene(this);

		view->setScene(scene);
		m_ui->rcUnitsView->addTab(view, help.unitName);

		int y = 0;

		for (const auto& method : help.methods)
		{
			if (method.hiddenForScratch)
				continue;

			auto initilizeArgumentItem = [&](auto &argumentItem, auto&& position)
			{
				for (auto parameter : method.parameters)
					if (parameter.typeName == "bool")
						argumentItem->addArgument(parameter.name.toStdString(), Item::Type::Condition);
					else if (parameter.typeName == "double")
						argumentItem->addArgument(parameter.name.toStdString(), Item::Type::Number);

				argumentItem->setPos(position);

				scene->addItem(argumentItem);
			};

			auto argumentBlock =
				new ArgumentBlock((help.unitName + "." + method.name).toStdString());

			initilizeArgumentItem(argumentBlock, QPoint(0, y));

			//if (!method.hasReturn)
			//{
				ArgumentItem* argumentItem;

				//if (method.returnParameter.typeName == "bool")
					argumentItem = new ArgumentCondition((help.unitName + "." + method.name).toStdString());
				//else if (method.returnParameter.typeName == "double")
				//	argumentItem = new ArgumentNumber((help.unitName + "." + method.name).toStdString());

				initilizeArgumentItem(argumentItem, QPoint(argumentBlock->m_width + 30, y));
			//}

			y += argumentBlock->m_height + 30;
		}
	}
}

void Widget::onDockLocationChanged(const Qt::DockWidgetArea&)
{}

} // namespace Scratch

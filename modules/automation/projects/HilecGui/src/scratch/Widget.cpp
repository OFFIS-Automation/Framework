#include "Widget.h"

#include <sstream>

#include "ui_ScratchWidget.h"
#include "../HilecSingleton.h"

#include "core/RcUnitHelp.h"

#include <QKeyEvent>

#include "Gui/FunctionView.h"
#include "Gui/Dialogs/NewVariableDialog.h"

#include "Items/ControlFlow/WhileBlock.h"
#include "Items/ControlFlow/IfElseBlock.h"
#include "Items/PassBlock.h"

#include "Items/Argument/ArgumentBlock.h"
#include "Items/Argument/ArgumentItem.h"
#include "Items/Argument/Operation.h"

#include "Items/VariableItems.h"

#include "Items/Parameter/Point.h"

namespace Scratch
{

Widget::Widget(QWidget *parent)
	: QDockWidget(parent),
	m_ui(std::make_unique<Ui::ScratchWidget>())
{
	m_ui->setupUi(this);

	m_defaultTabCount = m_ui->blocks->count();

	m_ui->controlFlowView->setScene(&m_controlFlowScene);
	m_ui->operationsView->setScene(&m_operationsScene);
	m_ui->utilityView->setScene(&m_utilityScene);
	m_ui->functionBlockView->setScene(&m_functionBlockScene);

	m_ui->blocks->setTabEnabled(3, false);

	// Control flow

	auto whileBlock = new WhileBlock();
	m_controlFlowScene.addItem(whileBlock);

	auto ifElseBlock = new IfElseBlock();
	ifElseBlock->setPos(whileBlock->m_width + 30, 0);
	m_controlFlowScene.addItem(ifElseBlock);

	// Operations

	auto smaller = new Operation<Condition>("<", Item::Type::Number);
	smaller->setPos(0, 0);
	m_operationsScene.addItem(smaller);

	auto add = new Operation<Number>("+", Item::Type::Number);
	add->setPos(0, smaller->pos().y() + smaller->m_height + 30);
	m_operationsScene.addItem(add);

	auto times = new Operation<Number>("*", Item::Type::Number);
	times->setPos(0, add->pos().y() + add->m_height + 30);
	m_operationsScene.addItem(times);

	auto divide = new Operation<Number>("/", Item::Type::Number);
	divide->setPos(0, times->pos().y() + times->m_height + 30);
	m_operationsScene.addItem(divide);

	auto subtractPoint = new Operation<Point>("-", Item::Type::Point);
	subtractPoint->setPos(0, divide->pos().y() + divide->m_height + 30);
	m_operationsScene.addItem(subtractPoint);

	auto timesPoint = new Operation<Point>("*", Item::Type::Point, Item::Type::Number);
	timesPoint->setPos(0, subtractPoint->pos().y() + subtractPoint->m_height + 30);
	m_operationsScene.addItem(timesPoint);

	// Utility

	auto point = new Argument<Point>("types.Point");
	point->addArgument("x", Item::Type::Number);
	point->addArgument("y", Item::Type::Number);
	point->setPos(0, 0);
	m_utilityScene.addItem(point);

	auto getX = new Argument<Number>("getX");
	getX->addArgument("", Item::Type::Point);
	getX->setPos(0, point->pos().y() + point->m_height + 30);
	m_utilityScene.addItem(getX);

	auto getY = new Argument<Number>("getY");
	getY->addArgument("", Item::Type::Point);
	getY->setPos(0, getX->pos().y() + getX->m_height + 30);
	m_utilityScene.addItem(getY);

	auto pass = new PassBlock();
	pass->setPos(0, getY->pos().y() + getY->m_height + 30);
	m_utilityScene.addItem(pass);

	auto raise = new Argument<Block>("raise Exception");
	raise->setPos(0, pass->pos().y() + pass->m_height + 30);
	m_utilityScene.addItem(raise);

	auto sleep = new Argument<Block>("sleep");
	sleep->addArgument("seconds", Item::Type::Number);
	sleep->setPos(0, raise->pos().y() + raise->m_height + 30);
	m_utilityScene.addItem(sleep);


	// Signal / slot connections
	connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateTabs(bool)));
	connect(&OlvisSingleton::instance(), SIGNAL(filterCreated(const FilterInfo&, int)),
		SLOT(filterCreated(const FilterInfo&, int)));
	connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
		SLOT(onDockLocationChanged(Qt::DockWidgetArea)));

	connect(m_ui->functions, &FunctionTabWidget::newFunctionBlock, this,
		[&](auto* item)
		{
			item->setPos(0, m_functionBlockScene.height() + 30);
			m_functionBlockScene.addItem(item);

			m_ui->blocks->setTabEnabled(3, true);
		});
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

		auto& hilec = *HilecSingleton::hilec();

		const auto& basedir = hilec.getBaseDir();
		const auto filename = "scratch.py";


		QFile outputFile(basedir + "/" + filename);
		outputFile.remove();

		outputFile.open(QFile::ReadWrite);
		QTextStream outputStream(&outputFile);

		std::stringstream generatedFile;

		generatedFile << "from time import *" << std::endl;
		generatedFile << "from offis import *" << std::endl;
		generatedFile << std::endl;

		generatedFile << "# Helper functions for point type" << std::endl;
		generatedFile << "def getX(p):" << std::endl << "\t" << "return p.x" << std::endl;
		generatedFile << std::endl;
		generatedFile << "def getY(p):" << std::endl << "\t" << "return p.y" << std::endl;
		generatedFile << std::endl;

		for (const auto& name : hilec.rcUnits())
		{
			auto help = hilec.getUnitHelp(name);

			generatedFile << help.unitName.toStdString() + " = rc.getUnit(\""
				+ help.unitName.toStdString() + "\")" << std::endl;
		}

		generatedFile << std::endl;

		for (const auto& name : hilec.rcUnits())
		{
			const auto& help = hilec.getUnitHelp(name);

			if (help.type == UserRcUnitType::HwRcUnitType
				|| help.type == UserRcUnitType::RobotRcUnitType)
				generatedFile << help.unitName.toStdString() + ".acquire()" << std::endl;
			else if (help.type == UserRcUnitType::VisionRcUnitType)
				generatedFile << help.unitName.toStdString() + ".start()" << std::endl;
		}

		generatedFile << std::endl;

		for (int i = 0; i < m_ui->functions->count() - 1; ++i)
		{
			static_cast<FunctionView*>(m_ui->functions->widget(i))->print(generatedFile);
			generatedFile << std::endl;
		}

		generatedFile << "if __name__ == \"__main__\": main()" << std::endl;

		outputStream << generatedFile.str().c_str();
		outputStream.flush();

		hilec.runFile(filename);
	}

	event->ignore();
}

void Widget::updateTabs(bool partialReload)
{
	const auto& hilec = *HilecSingleton::hilec();

	for (int i = m_ui->blocks->count() - 1; i >= m_defaultTabCount; --i)
		m_ui->blocks->removeTab(i);

	auto color = Qt::lightGray;

	for (const auto& name : hilec.rcUnits())
	{
		color = Qt::GlobalColor(color + 1);

		if (color == Qt::yellow)
			color = Qt::red;

		auto help = hilec.getUnitHelp(name);
		auto& methods = help.methods;

		auto tab = new QWidget();
		auto gridLayout = new QGridLayout(tab);
		auto view = new QGraphicsView(tab);
		auto scene = new ControlScene(this);

		view->setScene(scene);

		gridLayout->addWidget(view, 0, 0, 1, 1);
		m_ui->blocks->addTab(tab, name);

		int y = 0;

		if (help.type == VisionRcUnitType)
		{
			for (const auto& filter : m_filter)
				for (const PortInfo& output : filter.typeInfo.outputs)
				{
					Parameter* argumentItem;

					if (output.typeName == "Boolean")
					{
						argumentItem = new VariableParameter<Condition>(
							(help.unitName + ".get" + output.name).toStdString());
					}
					else if(output.typeName == "Real")
					{
						argumentItem = new VariableParameter<Number>(
							(help.unitName + ".getPortValue(\"" + filter.name + "\", \""
								+ output.name + "\")").toStdString());
					}
					else if(output.typeName == "Point")
					{
						argumentItem = new VariableParameter<Point>(
							(help.unitName + ".getPortValue(\"" + filter.name + "\", \""
								+ output.name + "\")").toStdString());
					}
					else
						continue;

					argumentItem->m_fillStyle.setHsl(QColor(color).hue(), 50, 100);

					argumentItem->setPos(QPoint(0, y));
					scene->addItem(argumentItem);

					y += argumentItem->m_height + 30;
				}

			continue;
		}

		if (methods.empty())
			continue;

		for (const auto& method : methods)
		{
			if (method.hiddenForScratch)
				continue;

			auto initilizeArgumentItem = [&](auto& argumentItem, auto&& position)
			{
				argumentItem.m_fillStyle.setHsl(QColor(color).hue(), 50, 100);

				argumentItem.setToolTip(method.longDesc);

				for (auto parameter : method.parameters)
				{
					const auto& type = Item::nameToItemType.find(parameter.typeName.toStdString());

					if (type == Item::nameToItemType.cend())
						return false;

					argumentItem.addArgument(parameter.name.toStdString(), type->second);
				}

				argumentItem.setPos(position);

				scene->addItem(&argumentItem);

				return true;
			};

			auto name = (help.unitName + "." + method.name).toStdString();

			auto argumentBlock =
				new Argument<Block>(name);

			if (!initilizeArgumentItem(*argumentBlock, QPoint(0, y)))
			{
				delete argumentBlock;

				continue;
			}

			y += argumentBlock->m_height + 30;


			if (!method.hasReturn)
			{
				ArgumentItem* argumentItem;

				if (method.returnParameter.typeName == "bool")
					argumentItem = new Argument<Condition>(name);
				else if (method.returnParameter.typeName == "double")
					argumentItem = new Argument<Number>(name);
				else if (method.returnParameter.typeName == "Pointf")
					argumentItem = new Argument<Point>(name);
				else
					continue;

				initilizeArgumentItem(*argumentItem,
					argumentBlock->pos() + QPoint(argumentBlock->m_width + 30, 0));
			}
		}
	}
}

void Widget::filterCreated(const FilterInfo& info, int insertBefore)
{
	m_filter.push_back(info);

	updateTabs();
}

void Widget::onDockLocationChanged(const Qt::DockWidgetArea&)
{}

} // namespace Scratch

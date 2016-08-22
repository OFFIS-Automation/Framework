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
	m_ui->utilityView->setScene(&m_utilityScene);
	m_ui->functionBlockView->setScene(&m_functionBlockScene);

	m_ui->blocks->setTabEnabled(2, false);

	auto whileBlock = new WhileBlock();
	m_controlFlowScene.addItem(whileBlock);

	auto ifElseBlock = new IfElseBlock();
	ifElseBlock->setPos(whileBlock->m_width + 30, 0);
	m_controlFlowScene.addItem(ifElseBlock);

	auto point = new Argument<Point>("types.Point");
	point->addArgument("x", Item::Type::Number);
	point->addArgument("y", Item::Type::Number);
	point->setPos(0, 0);
	m_utilityScene.addItem(point);

	auto passBlock = new PassBlock();
	passBlock->setPos(0, point->m_height + 30);
	m_utilityScene.addItem(passBlock);

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

			m_ui->blocks->setTabEnabled(2, true);
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

		generatedFile << "from offis import *" << std::endl;
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

	for (const auto& name : hilec.rcUnits())
	{
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

						y += argumentItem->m_height + 30;
					}
					else if(output.typeName == "Point")
					{
						argumentItem = new VariableParameter<Point>(
							(help.unitName + ".getPortValue(\"" + filter.name + "\", \""
								+ output.name + "\")").toStdString());
					}
					else
						continue;

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

			auto initilizeArgumentItem = [&](auto &argumentItem, auto&& position)
			{
				argumentItem->setToolTip(method.longDesc);

				for (auto parameter : method.parameters)
				{
					const auto& type = Item::nameToItemType.find(parameter.typeName.toStdString());

					if (type == Item::nameToItemType.cend())
						return false;

					argumentItem->addArgument(parameter.name.toStdString(), type->second);
				}

				argumentItem->setPos(position);

				scene->addItem(argumentItem);

				return true;
			};

			auto argumentBlock =
				new Argument<Block>((help.unitName + "." + method.name).toStdString());

			if (!initilizeArgumentItem(argumentBlock, QPoint(0, y)))
			{
				delete argumentBlock;

				continue;
			}

			if (!method.hasReturn)
			{
				ArgumentItem* argumentItem;

				if (method.returnParameter.typeName == "bool")
					argumentItem = new Argument<Condition>((help.unitName + "." + method.name).toStdString());
				else if (method.returnParameter.typeName == "double")
					argumentItem = new Argument<Number>((help.unitName + "." + method.name).toStdString());
				else if (method.returnParameter.typeName == "Pointf")
					argumentItem = new Argument<Point>((help.unitName + "." + method.name).toStdString());

				initilizeArgumentItem(argumentItem, QPoint(argumentBlock->m_width + 30, y));
			}

			y += argumentBlock->m_height + 30;
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

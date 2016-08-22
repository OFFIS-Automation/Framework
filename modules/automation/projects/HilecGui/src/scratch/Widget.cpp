#include "Widget.h"

#include <sstream>

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

#include "VariableItems.h"

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

	auto passBlock = new PassBlock();
	passBlock->setPos(ifElseBlock->pos().x() + ifElseBlock->m_width + 30, 0);
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

			if (help.type != UserRcUnitType::HwRcUnitType
				&& help.type != UserRcUnitType::RobotRcUnitType)
				continue;

			generatedFile << help.unitName.toStdString() + ".acquire()" << std::endl;
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
					if (output.typeName == "Boolean")
					{
						auto argumentItem = new VariableParameter<Condition>(
							(help.unitName + ".get" + output.name).toStdString());

						argumentItem->setPos(QPoint(0, y));
						scene->addItem(argumentItem);

						y += argumentItem->m_height + 30;
					}
					else if(output.typeName == "Real")
					{
						auto argumentItem = new VariableParameter<Number>(
							(help.unitName + ".getPortValue(\"" + filter.name + "\", \""
								+ output.name + "\")").toStdString());

						argumentItem->setPos(QPoint(0, y));
						scene->addItem(argumentItem);

						y += argumentItem->m_height + 30;
					}
					else if(output.typeName == "Point")
					{
						auto argumentItemX = new VariableParameter<Number>(
							(help.unitName + ".getPortValue(\"" + filter.name + "\", \""
								+ output.name + "\").x").toStdString());

						argumentItemX->setPos(QPoint(0, y));
						scene->addItem(argumentItemX);

						y += argumentItemX->m_height + 30;

						auto argumentItemY = new VariableParameter<Number>(
							(help.unitName + ".getPortValue(\"" + filter.name + "\", \""
								+ output.name + "\").y").toStdString());

						argumentItemY->setPos(QPoint(0, y));
						scene->addItem(argumentItemY);

						y += argumentItemY->m_height + 30;
					}
					else
						continue;
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

void Widget::filterCreated(const FilterInfo& info, int insertBefore)
{
	m_filter.push_back(info);

	updateTabs();
}

void Widget::onDockLocationChanged(const Qt::DockWidgetArea&)
{}

} // namespace Scratch

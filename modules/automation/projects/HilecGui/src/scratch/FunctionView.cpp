#include <iostream>
#include <sstream>
#include <type_traits>

#include <QFile>

#include "../HilecSingleton.h"
#include "core/RcUnitHelp.h"

#include "FunctionView.h"
#include "ui_ScratchFunctionView.h"

#include "NewVariableDialog.h"
#include "NewFunctionDialog.h"

#include "FrameBlocks.h"
#include "VariableItems.h"

namespace Scratch
{

FunctionTabWidget::FunctionTabWidget(QWidget *parent)
:	QTabWidget(parent)
{
	setTabsClosable(true);

	addTab(new QWidget(), "+");
	addTab(new FunctionView(), "main");
	setCurrentIndex(0);

	tabBar()->tabButton(0, QTabBar::ButtonPosition::RightSide)->resize(0, 0);
	tabBar()->tabButton(1, QTabBar::ButtonPosition::RightSide)->resize(0, 0);

	connect(this, &FunctionTabWidget::tabBarClicked, this, &FunctionTabWidget::addFunction);
}

int FunctionTabWidget::addTab(QWidget* widget, const QString &label)
{
	return insertTab(count() - 1, widget, label);
}

int FunctionTabWidget::addTab(QWidget *widget, const QIcon& icon, const QString &label)
{
	return insertTab(count() - 1, widget, icon, label);
}

void FunctionTabWidget::addFunction(int index)
{
	if (index != count() - 1)
		return;

	NewFunctionDialog newFunctionDialog(this);

	if (!newFunctionDialog.exec())
	{
		auto currentChangedhandler = [&, oldIndex = currentIndex()](int index)
		{
			disconnect(this, &FunctionTabWidget::currentChanged, this, nullptr);

			setCurrentIndex(oldIndex);
		};

		connect(this, &FunctionTabWidget::currentChanged, this, currentChangedhandler);

		return;
	}

	std::string name;
	Item::Type returnType;
	std::vector<std::pair<std::string, Item::Type>> parameters;

	auto function = newFunctionDialog.get();

	std::tie(name, returnType, parameters) = function;

	auto functionView = new FunctionView(this);

	addTab(functionView, name.c_str());
	setCurrentIndex(index);

	for (auto const& parameter : parameters)
		functionView->addVariable(parameter.first, parameter.second);
}

FunctionView::FunctionView(QWidget *parent) :
	QWidget(parent),
	m_ui(std::make_unique<Ui::ScratchFunctionView>()),
	m_programScene(this),
	m_VariablesScene(this)
{
	m_ui->setupUi(this);

	m_ui->programView->setScene(&m_programScene);
	m_ui->variablesView->setScene(&m_VariablesScene);

	// Program scene
	m_startBlock = new StartBlock();
	m_programScene.addItem(m_startBlock);

	auto endBlock = new EndBlock();
	m_startBlock->addBelow(*endBlock);
	m_programScene.addItem(m_startBlock);

	connect(m_ui->pushButton, &QPushButton::clicked, this,
		[&]()
		{
			NewVariableDialog newVariableDialog(this);

			if (!newVariableDialog.exec())
				return;

			std::string name;
			Item::Type type;

			std::tie(name, type) = newVariableDialog.get();

			addVariable(std::move(name), std::move(type));
		});
	}

void FunctionView::addVariable(std::string name, Item::Type type)
{
	auto setBlock = new SetVariableBlock(name, type);
	setBlock->setPos(0, variableY);

	m_VariablesScene.addItem(setBlock);

	Parameter* variableBlock;

	if (type == Item::Type::Number)
		variableBlock = new VariableParameter<Number>(name);
	else
		variableBlock = new VariableParameter<Condition>(name);

	m_VariablesScene.addItem(variableBlock);
	variableBlock->setPos(
		setBlock->pos().x() + setBlock->m_width + 30,
		setBlock->pos().y() + setBlock->m_height / 2 - variableBlock->m_height / 2);

	variableY += setBlock->m_height + 30;
}

void FunctionView::generateFile()
{
	const auto filename = "generated.py";

	auto& hilec = *HilecSingleton::hilec();

	QFile outputFile(filename);
	outputFile.remove(filename);
	outputFile.open(QFile::ReadWrite);
	QTextStream outputStream(&outputFile);

	std::stringstream generatedFile;

	auto printMethodForAllRCUnits = [&](auto & methodName)
	{
		for (const auto& name : hilec.rcUnits())
		{
			auto help = hilec.getUnitHelp(name);

			generatedFile << help.unitName.toStdString() + "." + methodName + "()" << std::endl;
		}
	};

	generatedFile << "from offis import *" << std::endl;
	generatedFile << std::endl;

	for (const auto& name : hilec.rcUnits())
	{
		auto help = hilec.getUnitHelp(name);

		generatedFile << help.unitName.toStdString() + " = rc.getUnit(\"" + help.unitName.toStdString() + "\")" << std::endl;
	}

	generatedFile << std::endl;

	printMethodForAllRCUnits("acquire");
	generatedFile << std::endl;
	m_startBlock->print(generatedFile);
	generatedFile << std::endl;
	printMethodForAllRCUnits("release");

	outputStream << generatedFile.str().c_str();
	outputStream.flush();

	hilec.runFile(filename);
	outputFile.remove();
}

}

#include <iostream>
#include <sstream>

#include <QFile>

#include "../HilecSingleton.h"
#include "core/RcUnitHelp.h"

#include "FunctionView.h"
#include "ui_ScratchFunctionView.h"

#include "NewVariableDialog.h"

#include "FrameBlocks.h"
#include "VariableItems.h"

namespace Scratch
{

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

	connect(m_ui->pushButton, SIGNAL(clicked()), SLOT(newVariable()));
}

void FunctionView::newVariable()
{
	NewVariableDialog newVariableDialog(this);
	newVariableDialog.exec();

	std::string name;
	Item::Type type;

	std::tie(name, type) = newVariableDialog.get();

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

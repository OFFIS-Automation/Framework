#include "Widget.h"

#include <map>
#include <sstream>
#include <locale>
#include <iostream>

#include "ui_BlocklyWidget.h"
#include "../HilecSingleton.h"

#include "core/RcUnitHelp.h"

#include <QKeyEvent>

namespace Blockly
{

Widget::Widget(QWidget *parent)
	: QDockWidget(parent),
	m_ui(std::make_unique<Ui::BlocklyWidget>())
{
	m_ui->setupUi(this);

	const auto& webView = m_ui->webView;

	webView->setUrl(QUrl("qrc:/index.html"));
	webView->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
	webView->show();

	connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateRcUnits(bool)));
	connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
		SLOT(onDockLocationChanged(Qt::DockWidgetArea)));
}

void Widget::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_G || event->key() == Qt::Key_S || event->key() == Qt::Key_L)
		event->accept();
	else
		event->ignore();
}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
	const auto& page = m_ui->webView->page();

	if (event->key() == Qt::Key_G)
	{
		event->accept();

		page->runJavaScript("Blockly.Python.workspaceToCode(workspace)",
			[](const QVariant& variant)
			{
				auto& hilec = *HilecSingleton::hilec();

				const auto& basedir = hilec.getBaseDir();
				const auto filename = "blockly.py";

				QFile outputFile(basedir + "/" + filename);
				outputFile.remove();

				outputFile.open(QFile::ReadWrite);
				QTextStream outputStream(&outputFile);

				std::stringstream preamble;

				preamble << "from time import *" << std::endl;
				preamble << "from offis import *" << std::endl;
				preamble << std::endl;

				for (const auto& name : hilec.rcUnits())
				{
					const auto& help = hilec.getUnitHelp(name);

					preamble << help.unitName.toStdString() << " = rc.getUnit(\""
						<< help.unitName.toStdString() + "\")" << std::endl;

					if (help.type == UserRcUnitType::HwRcUnitType
						|| help.type == UserRcUnitType::RobotRcUnitType)
						preamble << help.unitName.toStdString() + ".acquire()" << std::endl;
					else if (help.type == UserRcUnitType::VisionRcUnitType)
						preamble << help.unitName.toStdString() + ".start()" << std::endl;

					preamble << std::endl;
				}

				outputStream << preamble.str().c_str();
				outputStream << variant.toString();

				outputStream.flush();

				hilec.runFile(filename);
			});
	}
	else if (event->key() == Qt::Key_S)
	{
		event->accept();

		page->runJavaScript(
			"Blockly.Xml.domToText(Blockly.Xml.workspaceToDom(workspace))",
			[](const QVariant& variant)
			{
				auto& hilec = *HilecSingleton::hilec();

				const auto& basedir = hilec.getBaseDir();
				const auto filename = "blockly.xml";

				QFile outputFile(basedir + "/" + filename);
				outputFile.remove();

				outputFile.open(QFile::ReadWrite);
				QTextStream outputStream(&outputFile);

				outputStream << variant.toString();

				outputStream.flush();
			});
	}
	else if (event->key() == Qt::Key_L)
	{
		event->accept();

		auto& hilec = *HilecSingleton::hilec();

		const auto& basedir = hilec.getBaseDir();
		const auto filename = "blockly.xml";

		QFile inputFile(basedir + "/" + filename);

		inputFile.open(QFile::ReadOnly | QFile::Text);
		QTextStream inputStream(&inputFile);

		auto xml = inputStream.readAll().toStdString();

		// Escape all quotes
		for (size_t n = xml.find("\"", 0); n != std::string::npos; n = xml.find("\"", n))
		{
			xml.insert(n, "\\");
			n+=2;
		}

		page->runJavaScript(("workspace.clear();Blockly.Xml.domToWorkspace(Blockly.Xml.textToDom(\""
			+ xml + "\"), workspace);").c_str());
	}

	event->ignore();
}

void Widget::updateRcUnits(bool)
{
	static const auto getType = [](const auto& input)
	{
		static const std::map<std::string, std::string> typeLookup =
		{
			{"Variant", "null"},
			{"bool", "\"Boolean\""},
			{"int", "\"Number\""},
			{"double", "\"Number\""},
			{"string", "\"String\""},
			{"bool", "\"Boolean\""}
		};

		const auto& output = typeLookup.find(input);

		return output != typeLookup.cend()? output->second : "\"" + input + "\"";
	};

	static const auto makeHumanReadable = [](const auto& input)
	{
		std::stringstream humanReadable;

		for (const auto& character : input)
			if (character == '_')
				humanReadable << " ";
			else if (std::isupper(character, std::locale()))
				humanReadable << " " << std::tolower(character, std::locale());
			else
				humanReadable << character;

		return humanReadable.str();
	};

	const auto& hilec = *HilecSingleton::hilec();

	std::stringstream toolbox, block, python;

	toolbox << "document.getElementById(\"rc-units\").innerHTML = \"\";";

	int rcUnitColorHue = 0;

	for (const auto& name : hilec.rcUnits())
	{
		auto help = hilec.getUnitHelp(name);
		auto& methods = help.methods;

		if (methods.empty())
			continue;

		toolbox << "var category = document.createElement(\"category\");"
			<< "category.setAttribute(\"name\", \"" << name.toStdString() << "\");"
			<< "category.setAttribute(\"colour\", \"" << rcUnitColorHue << "\");"
			<< "document.getElementById(\"rc-units\").appendChild(category);";

		for (const auto& method : methods)
		{
			if (method.hiddenForScratch)
				continue;

			std::stringstream blockName;

			blockName << help.unitName.toStdString() << "_"
				<< method.name.toStdString();

			for (const auto& parameter : method.parameters)
				blockName << "_" << parameter.typeName.toStdString();

			block << "Blockly.Blocks['" << blockName.str() << "'] = {"
				<< "init: function() {"
				<< "this.setTooltip(\"" << method.longDesc.toStdString() << "\");"
				<< "this.setColour(" << rcUnitColorHue << ");";

			python << "Blockly.Python['" << blockName.str() << "'] = function(block) {";

			const auto& name = help.unitName.toStdString() + " "
				+ makeHumanReadable(method.name.toStdString());

			block << "this.appendDummyInput().appendField(\"" << name;

			if (method.parameters.size() != 0)
				block << " with";

			block << "\");";

			std::stringstream code;

			code << help.unitName.toStdString() << "." << method.name.toStdString() << "('";

			for (const auto& parameter : method.parameters)
			{
				const auto& parameterName = parameter.name.toStdString();

				block << "this.appendValueInput(\"" << parameterName << "\")"
					<< ".appendField(\"" << makeHumanReadable(parameterName) << "\")"
					<< ".setCheck(" << getType(parameter.typeName.toStdString()) << ");";

				const auto& valueName = "value_" + parameterName;

				python << "var " << valueName << " = Blockly.Python.valueToCode(block, '"
					<< parameterName << "', Blockly.Python.ORDER_ATOMIC);";

				code << " + " << valueName;

				if (method.parameters.last().name != parameter.name)
					code << " + ', '";
			}

			python << "var code = '" << code.str() << " + ')";

			if (method.hasReturn && method.returnParameter.typeName.toStdString() != "void")
			{
				block << "this.setOutput(true, "
					<< getType(method.returnParameter.typeName.toStdString())
					<< ");";

				python << "';"
					<< "return [code, Blockly.Python.ORDER_ATOMIC];";
			}
			else
			{
				block << "this.setPreviousStatement(true, null);"
					<< "this.setNextStatement(true, null);";

				python << "\\n';"
					<< "return code;";
			}

			python << "};";

			toolbox << "var block = document.createElement(\"block\");"
				<< "block.setAttribute(\"type\", \"" << blockName.str() << "\");"
				<< "category.appendChild(block);";

			block << "}"
				<< "};";
		}

		rcUnitColorHue = (rcUnitColorHue + 50) % 360;
	}

	toolbox << "workspace.updateToolbox(toolbox);";

	const auto& page = m_ui->webView->page();

	page->runJavaScript(block.str().c_str());
	page->runJavaScript(python.str().c_str());
	page->runJavaScript(toolbox.str().c_str());
}

void Widget::onDockLocationChanged(const Qt::DockWidgetArea&)
{}

} // namespace Blockly

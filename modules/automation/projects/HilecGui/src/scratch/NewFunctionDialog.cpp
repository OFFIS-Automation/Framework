#include "NewFunctionDialog.h"
#include "ui_ScratchNewFunctionDialog.h"

#include <string>

#include <QComboBox>
#include <QHeaderView>

namespace Scratch
{

NewFunctionDialog::NewFunctionDialog(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::ScratchNewFunctionDialog)
{
	m_ui->setupUi(this);

	connect(m_ui->addParameterButton, &QPushButton::clicked, this, &NewFunctionDialog::addParameter);
}

void NewFunctionDialog::addParameter()
{
	auto& parameters = *m_ui->parameters;

	auto index = parameters.rowCount();

	auto type = new QComboBox(&parameters);
	type->addItem("Number");
	type->addItem("Condition");

	parameters.insertRow(index);
	parameters.setItem(index, 0, new QTableWidgetItem("parameter" + QString::number(index + 1)));
	parameters.setCellWidget(index, 1, type);
}

NewFunctionDialog::~NewFunctionDialog()
{
	delete m_ui;
}

FunctionView::Function NewFunctionDialog::get()
{
	FunctionView::Parameters parameters;
	parameters.reserve(m_ui->parameters->rowCount());

	for (size_t i = 0; i < m_ui->parameters->rowCount(); ++i)
		parameters.emplace_back(
			m_ui->parameters->item(i, 0)->text().toStdString(),
			Item::nameToItemType.at(static_cast<QComboBox*>(
				m_ui->parameters->cellWidget(i, 1))->currentText().toStdString()));

	return std::make_tuple(
		m_ui->name->text().toStdString(),
		Item::nameToItemType.at(m_ui->type->currentText().toStdString()),
		std::move(parameters));
}

}

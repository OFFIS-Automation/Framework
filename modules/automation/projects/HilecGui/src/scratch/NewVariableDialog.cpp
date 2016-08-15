#include "NewVariableDialog.h"
#include "ui_ScratchNewVariableDialog.h"

namespace Scratch
{

const std::unordered_map<std::string, Item::Type> NewVariableDialog::nameToItemType =
	{{"Number", Item::Type::Number}, {"Condition", Item::Type::Condition}};

NewVariableDialog::NewVariableDialog(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::ScratchNewVariableDialog)
{
	m_ui->setupUi(this);
}

NewVariableDialog::~NewVariableDialog()
{
	delete m_ui;
}

std::tuple<std::string, Item::Type> NewVariableDialog::get()
{
	return std::make_tuple(m_ui->name->text().toStdString(),
		nameToItemType.at(m_ui->type->currentText().toStdString()));
}

}

#include "NewVariableDialog.h"
#include "ui_ScratchNewVariableDialog.h"

namespace Scratch
{

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
		Item::nameToItemType.at(m_ui->type->currentText().toStdString()));
}

}

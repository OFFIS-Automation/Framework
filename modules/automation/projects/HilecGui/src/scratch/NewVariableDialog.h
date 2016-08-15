#ifndef NEWVARIABLE_H
#define NEWVARIABLE_H

#include <tuple>
#include <string>
#include <unordered_map>

#include <QDialog>

#include "Item.h"

namespace Ui {
class ScratchNewVariableDialog;
}

namespace Scratch
{

class NewVariableDialog : public QDialog
{
		Q_OBJECT

	private:
		static const std::unordered_map<std::string, Item::Type> nameToItemType;

	public:
		explicit NewVariableDialog(QWidget *parent = 0);
		~NewVariableDialog();

		std::tuple<std::string, Item::Type> get();

	private:
		Ui::ScratchNewVariableDialog *m_ui;
};

}

#endif // NEWVARIABLE_H

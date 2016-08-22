#ifndef NEWVARIABLE_H
#define NEWVARIABLE_H

#include <tuple>
#include <string>

#include <QDialog>

#include "../../Items/Item.h"

namespace Ui {
class ScratchNewVariableDialog;
}

namespace Scratch
{

class NewVariableDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit NewVariableDialog(QWidget *parent = 0);
		~NewVariableDialog();

		std::tuple<std::string, Item::Type> get();

	private:
		Ui::ScratchNewVariableDialog *m_ui;
};

}

#endif // NEWVARIABLE_H

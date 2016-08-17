#ifndef NEWFUNCTIONDIALOG_H
#define NEWFUNCTIONDIALOG_H

#include <tuple>
#include <string>
#include <vector>

#include <QDialog>

#include "Item.h"
#include "FunctionView.h"

namespace Ui {
class ScratchNewFunctionDialog;
}

namespace Scratch
{

class NewFunctionDialog : public QDialog
{
		Q_OBJECT

	public:
		explicit NewFunctionDialog(QWidget *parent = 0);
		~NewFunctionDialog();

		FunctionView::Function get();

	private:
		void addParameter();

	private:
		Ui::ScratchNewFunctionDialog *m_ui;
};

}

#endif // NEWFUNCTIONDIALOG_H

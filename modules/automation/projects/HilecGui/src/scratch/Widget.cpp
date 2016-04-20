#include "Widget.h"

#include "ui_ScratchWidget.h"
#include "../HilecSingleton.h"

#include <iostream>

#include <core/RcUnitHelp.h>
#include <QKeyEvent>

#include "FrameBlocks.h"

#include "WhileBlock.h"
#include "IfElseBlock.h"
#include "PassBlock.h"
#include "TrueCondition.h"

#define ROOT_TYPE_OFFSET 1
#define CHILD_TYPE_OFFSET 2

namespace Scratch
{

Widget::Widget(QWidget *parent)
	: QDockWidget(parent),
	m_ui(std::make_unique<Ui::ScratchWidget>()),
	m_programScene(std::make_unique<QGraphicsScene>(this)),
	m_controlScene(std::make_unique<ControlScene>(this))
{
	m_ui->setupUi(this);
	m_ui->programView->setScene(m_programScene.get());
	m_ui->controlView->setScene(m_controlScene.get());

	// Program scene
	m_startBlock = new StartBlock();
	m_programScene->addItem(m_startBlock);

	auto endBlock = new EndBlock();
	m_startBlock->addBelow(*endBlock);
	m_programScene->addItem(m_startBlock);

	// Control scene
	auto whileBlock = new WhileBlock();
	m_controlScene->addItem(whileBlock);

	auto ifElseBlock = new IfElseBlock();
	ifElseBlock->setPos(whileBlock->m_width + 30, 0);
	m_controlScene->addItem(ifElseBlock);

	auto passBlock = new TrueCondition();
	passBlock->setPos(ifElseBlock->pos().x() + ifElseBlock->m_width + 30, 0);
	m_controlScene->addItem(passBlock);

	auto trueCondiftion = new PassBlock();
	trueCondiftion->setPos(passBlock->pos().x() + passBlock->m_width + 30, 0);
	m_controlScene->addItem(trueCondiftion);

	// Signal / slot connections
	connect(HilecSingleton::hilec(), SIGNAL(rcUnitsChanged(bool)), SLOT(updateRcUnits(bool)));
	connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), SLOT(onDockLocationChanged(Qt::DockWidgetArea)));
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
	if(event->key() != Qt::Key_G)
	{
		event->ignore();

		return;
	}

	event->accept();

	m_startBlock->print(std::cout);
	std::cout.flush();
}


void Widget::updateRcUnits(bool)
{
	const auto& hilec = *HilecSingleton::hilec();

	for (const auto& name : hilec.rcUnits())
	{
		auto help = hilec.getUnitHelp(name);

		if (help.methods.empty())
			continue;

		QTreeWidgetItem *treeItem = new QTreeWidgetItem(m_ui->treeWidget);

		treeItem->setFlags(treeItem->flags() ^ Qt::ItemIsDragEnabled);
		treeItem->setText(0, name);

		for (const auto& method : help.methods)
		{
			//if (method.hiddenForScratch)
			//	continue;

			// Get Parameter names
			QStringList parameterNames;

			for (auto parameter : method.parameters)
				parameterNames << tr("%1 (%2)").arg(parameter.name).arg(QString(parameter.typeName));

			QTreeWidgetItem *childTreeItem = new QTreeWidgetItem(QTreeWidgetItem::UserType + CHILD_TYPE_OFFSET);

			childTreeItem->setText(0, method.name);
			childTreeItem->setText(1, parameterNames.isEmpty() ? tr("None") : parameterNames.join(", "));
			childTreeItem->setText(2, method.shortDesc);

			treeItem->addChild(childTreeItem);
		}
	}

	m_ui->treeWidget->expandAll();
}

void Widget::onDockLocationChanged(const Qt::DockWidgetArea& area)
{
	// Unused arguments
	static_cast<void>(area);
}

} // namespace Scratch

#include <iostream>

#include <QPushButton>

#include "RCUnits.h"

#include "ControlScene.h"

namespace Scratch
{

RCUnits::RCUnits(QWidget* parent)
	: QTabWidget(parent)
{
	connect(this, &RCUnits::currentChanged, this, &RCUnits::onCurrentChanged);
}

void RCUnits::addRcUnit()
{
	insertTab(count() - 1, new QWidget(this), "");
	setCurrentIndex(count() - 2);
}

void RCUnits::onAddButtonClicked(bool)
{
	addRcUnit();
}

void RCUnits::onCurrentChanged(int index)
{
	if (count() == 1)
	{
		auto addButton = currentWidget()->findChild<QPushButton*>("addRCUnitButton");
		connect(addButton, &QPushButton::clicked, this, &RCUnits::onAddButtonClicked);

		return;
	}

	if (index == count() - 1)
		addRcUnit();
}

} // namespace Scratch

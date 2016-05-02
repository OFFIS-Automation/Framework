#ifndef RCUNITS_H
#define RCUNITS_H

#include <QTabWidget>

namespace Scratch
{

class RCUnits : public QTabWidget
{
	public:
		RCUnits(QWidget* parent = nullptr);

		void addRcUnit();

		void onAddButtonClicked(bool);
		void onCurrentChanged(int index);
};

} // namespace Scratch

#endif // RCUNITS_H

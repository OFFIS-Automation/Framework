#ifndef NUMBERTEXTITEM_H
#define NUMBERTEXTITEM_H

#include <QGraphicsTextItem>

#include "../Parameter.h"

namespace Scratch
{

class NumberTextItem : public QGraphicsTextItem
{
	public:
		NumberTextItem(Parameter& parent);

	protected:
		void keyPressEvent(QKeyEvent* event);

	private:
		Parameter& m_parent;
};

}

#endif // NUMBERTEXTITEM_H

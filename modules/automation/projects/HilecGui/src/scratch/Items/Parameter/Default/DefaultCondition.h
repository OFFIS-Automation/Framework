#ifndef DEFAULTCONDITION_H
#define DEFAULTCONDITION_H


#include <string>

#include <QGraphicsTextItem>

#include "../Condition.h"

namespace Scratch
{

class DefaultCondition : public Condition
{
	private:
		class ConditionTextItem : public QGraphicsTextItem
		{
			public:
				ConditionTextItem(DefaultCondition& parent);

			protected:
				void mousePressEvent(QGraphicsSceneMouseEvent* event);

			private:
				DefaultCondition& m_parent;
		};

	public:
		DefaultCondition(const bool enable = false);

		Item& clone() const;
		void print(std::ostream& stream) const;

		bool updateItem();

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent* event);

	private:
		ConditionTextItem m_text;
};

} // namespace Scratch

#endif // DEFAULTCONDITION_H

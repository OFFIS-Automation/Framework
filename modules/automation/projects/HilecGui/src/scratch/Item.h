#ifndef ITEM_H
#define ITEM_H

#include <cassert>

#include <ostream>
#include <functional>
#include <memory>
#include <iostream>

#include <QGraphicsItem>
#include <QPen>
#include <QFont>

namespace Scratch
{

class Item : public QGraphicsItem
{
	public:
		static const int s_margin = 15;

		static Item& unpackItem(const QGraphicsSceneDragDropEvent& event);

		enum class Type
		{
			Block,
			Condition,
			Number
		};

	public:
		virtual Type itemType() const = 0;

		virtual Item& clone() const = 0;

		void setParent(Item* parent);
		virtual void remove();

		virtual bool updateItem();

		int m_width;
		int m_height;

	protected:
		Item(const int width, const int height);

		void mousePressEvent(QGraphicsSceneMouseEvent* event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

		void addItem(Item& item);

		int m_horizontalMargin = s_margin;

		QFont m_font;
		QPen m_outlineStyle;
		QPen m_textStyle;
		QColor m_fillStyle;

		Item* m_parent{nullptr};
};

} // namespace Scratch

#endif // ITEM_H

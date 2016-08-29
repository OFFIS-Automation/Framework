#ifndef ITEM_H
#define ITEM_H

#include <cassert>

#include <ostream>
#include <functional>
#include <memory>
#include <iostream>
#include <unordered_map>

#include <QGraphicsItem>
#include <QPen>
#include <QFont>

namespace Scratch
{

class Item : public QGraphicsItem
{
	public:
		static const int s_margin = 10;

		enum class Type
		{
			Block,
			Condition,
			Number,
			Point
		};

		static const std::unordered_map<std::string, Item::Type> nameToItemType;

		static Item& unpackItem(const QGraphicsSceneDragDropEvent& event);
		static void printIndentation(std::ostream& stream, const unsigned& depth);

	public:
		virtual Type itemType() const = 0;

		virtual Item& clone() const = 0;

		void setParent(Item* parent);
		virtual void remove();

		virtual int defaultHeight();
		virtual bool updateItem();

		int m_width;
		int m_height;

		QColor m_fillStyle;

	protected:
		Item();

		void mousePressEvent(QGraphicsSceneMouseEvent* event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

		void addItem(Item& item);

		int m_horizontalMargin = s_margin;

		QFont m_font;
		QPen m_outlineStyle;
		QPen m_textStyle;

		Item* m_parent{nullptr};
};

} // namespace Scratch

#endif // ITEM_H

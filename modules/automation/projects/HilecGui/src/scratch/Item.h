#ifndef ITEM_H
#define ITEM_H

#include <ostream>
#include <functional>
#include <memory>

#include <QGraphicsItem>
#include <QPen>

namespace Scratch
{

class Item : public QGraphicsItem
{
	public:
		static Item& unpackItem(const QGraphicsSceneDragDropEvent& event);

	protected:
		enum class Type
		{
			Block,
			Condition
		};

	public:
		virtual Type itemType() const = 0;

		virtual Item& clone() const = 0;

		void setParent(Item* parent);
		virtual void remove();
		virtual void resizeBy(int dx, int dy, const QPointF&);

		int m_width;
		int m_height;

	protected:
		Item(const int width, const int height);

		void mousePressEvent(QGraphicsSceneMouseEvent* event);
		void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

		void addItem(Item& item);

		QPen m_outlineStyle;
		QPen m_textStyle;
		QColor m_fillStyle;

		const int m_defaultWidth;
		const int m_defaultHeight;

		Item* m_parent{nullptr};
};

} // namespace Scratch

#endif // ITEM_H

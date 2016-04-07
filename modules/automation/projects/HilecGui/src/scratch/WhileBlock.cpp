#include <stdexcept>
#include <ostream>
#include <array>
#include <iostream>

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneEvent>

#include <QMimeData>
#include <QByteArray>

#include "WhileBlock.h"

namespace Scratch
{

WhileBlock::WhileBlock()
{
	m_width = s_defaultWidth;
	m_height = s_defaultHeight;

	setAcceptDrops(true);
}

QRectF WhileBlock::boundingRect() const
{
	return QRectF(
		0, -s_connectorActivationRange,
		m_width, m_height + 2 * s_connectorActivationRange);
}

void WhileBlock::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	QPolygon polygon;

	const auto connectorWidthOffset = (s_connectorWidth - s_connectorMidsegmentWidth) / 2;

	polygon
		<< QPoint(0, 0)

		// Upper conector
		<< QPoint(
			s_connectorMargin,
			0)
		<< QPoint(
			s_connectorMargin + connectorWidthOffset,
			s_connectorHeight)
		<< QPoint(
			s_connectorMargin + s_connectorWidth - connectorWidthOffset,
			s_connectorHeight)
		<< QPoint(
			s_connectorMargin + s_connectorWidth,
			0)

		<< QPoint(m_width, 0)
		<< QPoint(m_width, m_headerHeight)

		// Upper body conector
		<< QPoint(
			s_shaftHeight + s_connectorMargin + s_connectorWidth,
			m_headerHeight)
		<< QPoint(
			s_shaftHeight + s_connectorMargin + s_connectorWidth - connectorWidthOffset,
			m_headerHeight + s_connectorHeight)
		<< QPoint(
			s_shaftHeight + s_connectorMargin + connectorWidthOffset,
			m_headerHeight + s_connectorHeight)
		<< QPoint(
			s_shaftHeight + s_connectorMargin,
			m_headerHeight)

		<< QPoint(s_shaftHeight, m_headerHeight)
		<< QPoint(s_shaftHeight, m_height - s_shaftHeight)

		// Lower body conector
		<< QPoint(
			s_shaftHeight + s_connectorMargin,
			m_height - s_shaftHeight)
		<< QPoint(
			s_shaftHeight + s_connectorMargin + connectorWidthOffset,
			m_height - s_shaftHeight + s_connectorHeight)
		<< QPoint(
			s_shaftHeight + s_connectorMargin + s_connectorWidth - connectorWidthOffset,
			m_height - s_shaftHeight + s_connectorHeight)
		<< QPoint(
			s_shaftHeight + s_connectorMargin + s_connectorWidth,
			m_height - s_shaftHeight)

		<< QPoint(m_width, m_height - s_shaftHeight)
		<< QPoint(m_width, m_height)

		// Lower conector
		<< QPoint(
			s_connectorMargin + s_connectorWidth,
			m_height)
		<< QPoint(
			s_connectorMargin + s_connectorWidth - connectorWidthOffset,
			m_height + s_connectorHeight)
		<< QPoint(
			s_connectorMargin + connectorWidthOffset,
			m_height + s_connectorHeight)
		<< QPoint(
			s_connectorMargin,
			m_height)

		<< QPoint(0, m_height);

	painter->drawPolygon(polygon);
}

void WhileBlock::dragMoveEvent(QGraphicsSceneDragDropEvent* event)
{
	if (event->pos().y() > - s_connectorActivationRange
			&& event->pos().y() < s_connectorActivationRange
		|| event->pos().y() > m_height - s_connectorActivationRange
			 && event->pos().y() < m_height + s_connectorActivationRange
		|| event->pos().y() > m_headerHeight - s_connectorActivationRange
			 && event->pos().y() < m_headerHeight + s_connectorActivationRange)
	{
		event->accept();

		return;
	}

	event->ignore();
}

void WhileBlock::dropEvent(QGraphicsSceneDragDropEvent* event)
{
	auto byteArray = event->mimeData()->data("Block");
	auto& item = **reinterpret_cast<Block**>(byteArray.data());

	auto &clone = item.clone();

	if (event->pos().y() > - s_connectorActivationRange
			&& event->pos().y() < s_connectorActivationRange)
		addAbove(clone);
	else if (event->pos().y() > m_height - s_connectorActivationRange
			 && event->pos().y() < m_height + s_connectorActivationRange)
		addBelow(clone);
	else if (event->pos().y() > m_headerHeight - s_connectorActivationRange
			 && event->pos().y() < m_headerHeight + s_connectorActivationRange)
		addBody(clone);
}

Block& WhileBlock::clone()
{
	return *(new WhileBlock());
}

void WhileBlock::print(std::ostream& stream, unsigned indentationDepth = 0)
{
	if (!m_condition)
		throw std::invalid_argument("Not all conditions are defined.");

	if (!m_body)
		throw std::invalid_argument("Not all loop bodies are defined.");

	// while
	for (unsigned i = 0; i < indentationDepth; ++i)
		stream << "\t";

	stream << "while " << *m_condition << ":\n";

	m_body->print(stream, indentationDepth + 1);

	// next
	if (m_successor)
		m_successor->print(stream, indentationDepth);
}

void WhileBlock::addBody(Block& block)
{
	const auto& dHeight = block.m_height
		- !m_body * (s_defaultHeight - m_headerHeight - s_shaftHeight);

	block.setPredecessorsReference(&m_body);
	block.setSuccessor(m_body);
	block.setParent(this);

	m_body = &block;

	block.setPos(s_shaftHeight, m_headerHeight);
	resizeBy(0, dHeight);

	scene()->addItem(&block);
}

} // namespace Scratch

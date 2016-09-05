#ifndef SCRATCHCONTROLFLOWBLOCK_H
#define SCRATCHCONTROLFLOWBLOCK_H

#include <ostream>
#include <memory>
#include <vector>
#include <algorithm>

#include "../Argument/ArgumentBlock.h"

namespace Scratch
{

class ControlFlowBlock : public Argument<Block>
{
	protected:
		struct Body
		{
			int height;
			Block* block;
		};

	public:
		void paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*);

		bool updateItem();

	protected:
		ControlFlowBlock(const std::string& name, const size_t& numberOfBodies);

		bool inBodyRange(const QPoint& position);

		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

		void addBody(Block& block, Block*& bodyBlock, const QPoint &offset);

		int m_headerHeight;
		std::vector<Body> m_bodies{};
};

} // namespace Scratch

#endif // SCRATCHCONTROLFLOWBLOCK_H

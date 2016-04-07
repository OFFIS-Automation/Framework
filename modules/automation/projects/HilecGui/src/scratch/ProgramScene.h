#ifndef SCRATCHPROGRAMSCENE_H
#define SCRATCHPROGRAMSCENE_H

#include <QGraphicsScene>

#include "Block.h"

namespace Scratch
{

class ProgramScene : public QGraphicsScene
{
	public:
		ProgramScene(QObject* parent = nullptr);

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);

	private:
		Block* m_firstBlock{nullptr};
};

} // namespace Scratch

#endif // SCRATCHPROGRAMSCENE_H

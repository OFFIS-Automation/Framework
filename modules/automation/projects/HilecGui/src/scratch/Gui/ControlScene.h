#ifndef CONTROLSCENE_H
#define CONTROLSCENE_H

#include <QGraphicsScene>

namespace Scratch
{

class ControlScene : public QGraphicsScene
{
	public:
		ControlScene(QObject* parent = nullptr);

	protected:
		void dragMoveEvent(QGraphicsSceneDragDropEvent* event);
		void dropEvent(QGraphicsSceneDragDropEvent* event);
};

} // namespace Scratch

#endif // CONTROLSCENE_H

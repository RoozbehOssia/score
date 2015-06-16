#include "AbstractConstraintView.hpp"
#include "AbstractConstraintPresenter.hpp"
#include "Document/State/StateView.hpp"

AbstractConstraintView::AbstractConstraintView(
        AbstractConstraintPresenter& presenter,
        QGraphicsItem *parent) :
    QGraphicsObject {parent},
    m_presenter{presenter}
{
    setAcceptHoverEvents(true);
    m_dashPen.setDashPattern({2., 4.});

    m_startState = new StateView(this);
    m_endState = new StateView{this};
}

void AbstractConstraintView::setConnections()
{
    // NOTE : crash if in ctr ...
        connect(m_startState, &StateView::pressed,
                &m_presenter, &AbstractConstraintPresenter::pressed);
        connect(m_startState, &StateView::moved,
                &m_presenter,    &AbstractConstraintPresenter::moved);
        connect(m_startState,   &StateView::released,
                &m_presenter,    &AbstractConstraintPresenter::released);

        connect(m_endState, &StateView::pressed,
                &m_presenter, &AbstractConstraintPresenter::pressed);
        connect(m_endState, &StateView::moved,
                &m_presenter,    &AbstractConstraintPresenter::moved);
        connect(m_endState,   &StateView::released,
                &m_presenter,    &AbstractConstraintPresenter::released);
}

void AbstractConstraintView::setInfinite(bool infinite)
{
    prepareGeometryChange();

    m_infinite = infinite;
    update();
}

void AbstractConstraintView::setDefaultWidth(double width)
{
    prepareGeometryChange();
    m_defaultWidth = width;
}

void AbstractConstraintView::setMaxWidth(bool infinite, double max)
{
    prepareGeometryChange();

    setInfinite(infinite);
    if(!infinite)
    {
        //qDebug() << max;
        m_maxWidth = max;
    }

}

void AbstractConstraintView::setMinWidth(double min)
{
    prepareGeometryChange();
    m_minWidth = min;
}

void AbstractConstraintView::setHeight(double height)
{
    prepareGeometryChange();
    m_height = height;
}

void AbstractConstraintView::setPlayWidth(double width)
{
    m_playWidth = width;
    update();
}

void AbstractConstraintView::setValid(bool val)
{
    m_validConstraint = val;
}

#include <QGraphicsSceneMouseEvent>
void AbstractConstraintView::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    emit m_presenter.pressed(event->scenePos());
}

void AbstractConstraintView::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    emit m_presenter.moved(event->scenePos());
}

void AbstractConstraintView::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    emit m_presenter.released(event->scenePos());
}
bool AbstractConstraintView::warning() const
{
    return m_warning;
}

void AbstractConstraintView::setWarning(bool warning)
{
    m_warning = warning;
}


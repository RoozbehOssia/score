#include "TimeNodePresenter.hpp"

#include "Document/TimeNode/TimeNodeModel.hpp"
#include "Document/TimeNode/TimeNodeView.hpp"
#include <QGraphicsScene>

TimeNodePresenter::TimeNodePresenter(TimeNodeModel *model,
									 TimeNodeView *view,
									 QObject *parent):
	NamedObject{"EventPresenter", parent},
	m_model{model},
	m_view{view}
{

}

TimeNodePresenter::~TimeNodePresenter()
{
    if(m_view)
	{
		auto sc = m_view->scene();
		if(sc) sc->removeItem(m_view);
		m_view->deleteLater();
    }
}

id_type<TimeNodeModel> TimeNodePresenter::id() const
{
	return m_model->id();
}

TimeNodeModel *TimeNodePresenter::model()
{
	return m_model;
}

TimeNodeView *TimeNodePresenter::view()
{
	return m_view;
}


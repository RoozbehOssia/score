#pragma once
#include <score/widgets/SignalUtils.hpp>

#include <ossia/detail/math.hpp>

#include <QApplication>
#include <QDesktopWidget>
#include <QDoubleSpinBox>
#include <QEventLoop>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QWindow>
#include <QScreen>

namespace score
{
struct DefaultGraphicsKnobImpl
{
  static inline double origValue{};
  static inline double currentDelta{};
  static inline QRectF currentGeometry{};

  template <typename T, typename U>
  static void paint(
      T& self,
      const U& skin,
      const QString& text,
      QPainter* painter,
      QWidget* widget)
  {
    static const QPen darkPen{skin.HalfDark.color()};
    static const QPen grayPen{skin.HalfLight.color()};

    painter->setRenderHint(QPainter::Antialiasing, true);

    constexpr const double adj = 6.;
    constexpr const double space = 50.;
    constexpr const double start = (270. - space) * 16.;
    constexpr const double totalSpan = (360. - 2. * space) * 16.;

    const QRectF srect = self.boundingRect();
    const QRectF r = srect.adjusted(adj, adj, -adj, -adj);

    // Draw knob
    painter->setPen(QPen(skin.Background1.color(), 1));
    painter->setBrush(QBrush(skin.Background1.color()));
    painter->drawChord(r, start, -totalSpan);
    painter->setPen(QPen(skin.Base1.color(), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    const double valueSpan = -self.m_value * totalSpan;
    painter->drawArc(r, start, valueSpan);

    // Draw knob indicator
    const double r1 = 0.5 * r.width();
    const double x0 = r.center().x();
    const double y0 = r.center().y();
    const double theta = - 0.0174533 * (start + valueSpan) / 16.;
    const double x1 = r.center().x() + r1 * cos(theta);
    const double y1 = r.center().y() + r1 * sin(theta);

    painter->drawLine(QPointF{x0, y0}, QPointF{x1, y1});

    // Draw text
    painter->setPen(grayPen);
    painter->setFont(skin.MonoFont);
    painter->drawText(
        QRectF{0., srect.height() - 8, srect.width(), 10.},
        text,
        QTextOption(Qt::AlignCenter));

    painter->setRenderHint(QPainter::Antialiasing, false);
  }

  template <typename T>
  static void mousePressEvent(T& self, QGraphicsSceneMouseEvent* event)
  {
    if (event->button() == Qt::LeftButton)
    {
      self.m_grab = true;
      self.setCursor(QCursor(Qt::BlankCursor));
      origValue = self.m_value;
      currentDelta = 0.;
      currentGeometry = qApp->primaryScreen()->availableGeometry();
    }

    event->accept();
  }

  template <typename T>
  static void mouseMoveEvent(T& self, QGraphicsSceneMouseEvent* event)
  {
    if ((event->buttons() & Qt::LeftButton) && self.m_grab)
    {
      auto delta = (event->screenPos().y() - event->lastScreenPos().y());
      double ratio = qApp->keyboardModifiers() & Qt::CTRL ? .2 : 1.;
      if(std::abs(delta) < 500)
        currentDelta += ratio * delta;

      if(event->screenPos().y() <= 0)
        QCursor::setPos(QPoint(event->screenPos().x(), currentGeometry.height()));
      else if(event->screenPos().y() >= currentGeometry.height())
        QCursor::setPos(QPoint(event->screenPos().x(), 0));

      double v = origValue - currentDelta / currentGeometry.height();
      if(v <= 0.)
      {
        currentDelta = origValue * currentGeometry.height();
      }
      else if(v >= 1.)
      {
        currentDelta = (origValue - 1.) * currentGeometry.height();
      }
      double curPos = ossia::clamp(v, 0., 1.);
      if (curPos != self.m_value)
      {
        self.m_value = curPos;
        self.valueChanged(self.m_value);
        self.sliderMoved();
        self.update();
      }
    }
    event->accept();
  }

  template <typename T>
  static void mouseReleaseEvent(T& self, QGraphicsSceneMouseEvent* event)
  {
    QCursor::setPos(event->buttonDownScreenPos(Qt::LeftButton));

    self.unsetCursor();
    if (event->button() == Qt::LeftButton)
    {
      if (self.m_grab)
      {
        auto delta = (event->screenPos().y() - event->lastScreenPos().y());
        double ratio = qApp->keyboardModifiers() & Qt::CTRL ? .2 : 1.;
        if(std::abs(delta) < 500)
          currentDelta += ratio * delta;

        double v = origValue - currentDelta / currentGeometry.height();
        double curPos = ossia::clamp(v, 0., 1.);
        if (curPos != self.m_value)
        {
          self.m_value = curPos;
          self.valueChanged(self.m_value);
          self.update();
        }
        self.m_grab = false;
      }
      self.sliderReleased();
    }
    else if (event->button() == Qt::RightButton)
    {
      QTimer::singleShot(0, [&, pos = event->scenePos()] {
        auto w = new DoubleSpinboxWithEnter;
        w->setRange(self.map(self.min), self.map(self.max));

        w->setDecimals(6);
        w->setValue(self.map(self.m_value * (self.max - self.min) + self.min));
        auto obj = self.scene()->addWidget(
            w, Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
        obj->setPos(pos);

        QTimer::singleShot(0, w, [w] { w->setFocus(); });

        QObject::connect(
            w,
            SignalUtils::QDoubleSpinBox_valueChanged_double(),
            &self,
            [=, &self](double v) {
              self.m_value
                  = (self.unmap(v) - self.min) / (self.max - self.min);
              self.valueChanged(self.m_value);
              self.sliderMoved();
              self.update();
            });

        QObject::connect(
            w,
            &DoubleSpinboxWithEnter::editingFinished,
            &self,
            [obj, &self]() mutable {
              if (obj != nullptr)
              {
                self.sliderReleased();
                QTimer::singleShot(0, obj, [scene = self.scene(), obj] {
                  scene->removeItem(obj);
                  delete obj;
                });
              }
              obj = nullptr;
            });
      });
    }
    event->accept();
  }

  template <typename T>
  static void mouseDoubleClickEvent(T& self, QGraphicsSceneMouseEvent* event)
  {
  }
};
}

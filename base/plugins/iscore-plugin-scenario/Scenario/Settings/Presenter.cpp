#include "Presenter.hpp"
#include "Model.hpp"
#include "View.hpp"
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/command/Dispatchers/ICommandDispatcher.hpp>
#include <QApplication>
#include <QStyle>
#include <iscore/command/SettingsCommand.hpp>

namespace Scenario
{
namespace Settings
{
Presenter::Presenter(
        Model& m,
        View& v,
        QObject *parent):
    iscore::SettingsDelegatePresenterInterface{m, v, parent}
{
    con(v, &View::skinChanged,
        this, [&] (const auto& val) {
        if(val != m.getSkin())
        {
            m_disp.submitCommand<SetSkin>(this->model(this), val);
        }
    });

    con(m, &Model::skinChanged, &v, &View::setSkin);
    v.setSkin(m.getSkin());
}

QString Presenter::settingsName()
{
    return tr("Theme");
}

QIcon Presenter::settingsIcon()
{
    return QApplication::style()->standardIcon(QStyle::SP_DesktopIcon);
}


}
}

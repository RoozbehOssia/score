#include "InspectorPlugin.hpp"
#include "Panel/InspectorPanelFactory.hpp"
using namespace iscore;

#include "InspectorInterface/InspectorWidgetFactoryInterface.hpp"
#include <InspectorControl.hpp>

InspectorPlugin::InspectorPlugin() :
    QObject {},
        iscore::PanelFactoryInterface_QtInterface {},
        m_inspectorControl {new InspectorControl}
{
    setObjectName("InspectorPlugin");
}

QStringList InspectorPlugin::panel_list() const
{
    return {"Inspector Panel"};
}

PanelFactoryInterface* InspectorPlugin::panel_make(QString name)
{
    if(name == "Inspector Panel")
    {
        return new InspectorPanelFactory;
    }

    return nullptr;
}

QVector<FactoryFamily> InspectorPlugin::factoryFamilies_make()
{
    return {{"Inspector",
            std::bind(&InspectorControl::on_newInspectorWidgetFactory,
            m_inspectorControl,
            std::placeholders::_1)
        }
    };
}


QStringList InspectorPlugin::control_list() const
{
    return {"InspectorControl"};
}

PluginControlInterface* InspectorPlugin::control_make(QString s)
{
    if(s == "InspectorControl")
    {
        return m_inspectorControl;
    }

    return nullptr;
}

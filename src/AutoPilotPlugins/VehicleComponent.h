/****************************************************************************
 *
 * (c) 2009-2024 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

#pragma once

#include "AutoPilotPlugin.h"

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QVariant>
#include <QtCore/QStringList>

class Vehicle;
class QQuickItem;
class QQmlContext;

/// A vehicle component is an object which abstracts the physical portion of a vehicle into a set of
/// configurable values and user interface.

class VehicleComponent : public QObject
{
    Q_OBJECT
    
    Q_PROPERTY(QString  name                    READ name                   CONSTANT)
    Q_PROPERTY(QString  description             READ description            CONSTANT)
    Q_PROPERTY(bool     requiresSetup           READ requiresSetup          CONSTANT)
    Q_PROPERTY(bool     setupComplete           READ setupComplete          STORED false NOTIFY setupCompleteChanged)
    Q_PROPERTY(QString  iconResource            READ iconResource           CONSTANT)
    Q_PROPERTY(QUrl     setupSource             READ setupSource            NOTIFY setupSourceChanged)
    Q_PROPERTY(QUrl     summaryQmlSource        READ summaryQmlSource       CONSTANT)
    Q_PROPERTY(bool     allowSetupWhileArmed    READ allowSetupWhileArmed   CONSTANT)
    Q_PROPERTY(bool     allowSetupWhileFlying   READ allowSetupWhileFlying  CONSTANT)
    Q_PROPERTY(AutoPilotPlugin::KnownVehicleComponent KnownVehicleComponent READ KnownVehicleComponent CONSTANT)

public:
    VehicleComponent(Vehicle* vehicle, AutoPilotPlugin* autopilot, AutoPilotPlugin::KnownVehicleComponent KnownVehicleComponent, QObject* parent = nullptr);
    ~VehicleComponent();
    
    virtual QString name(void) const = 0;
    virtual QString description(void) const = 0;
    virtual QString iconResource(void) const = 0;
    virtual bool requiresSetup(void) const = 0;
    virtual bool setupComplete(void) const = 0;
    virtual QUrl setupSource(void) const = 0;
    virtual QUrl summaryQmlSource(void) const = 0;

    // @return true: Setup panel can be shown while vehicle is armed
    virtual bool allowSetupWhileArmed(void) const { return false; } // Defaults to false
    
    // @return true: Setup panel can be shown while vehicle is flying (and armed)
    virtual bool allowSetupWhileFlying(void) const { return false; } // Defaults to false

    virtual void addSummaryQmlComponent(QQmlContext* context, QQuickItem* parent);
    
    /// @brief Returns an list of parameter names for which a change should cause the setupCompleteChanged
    ///         signal to be emitted.
    virtual QStringList setupCompleteChangedTriggerList(void) const = 0;

    /// Should be called after the component is created (but not in constructor) to setup the
    /// signals which are used to track parameter changes which affect setupComplete state.
    virtual void setupTriggerSignals(void);

    AutoPilotPlugin::KnownVehicleComponent KnownVehicleComponent(void) const { return _KnownVehicleComponent; }

signals:
    void setupCompleteChanged   (void);
    void setupSourceChanged     (void);

protected slots:
    void _triggerUpdated(QVariant value);

protected:
    Vehicle*                            _vehicle;
    AutoPilotPlugin*                    _autopilot;
    AutoPilotPlugin::KnownVehicleComponent    _KnownVehicleComponent;
};

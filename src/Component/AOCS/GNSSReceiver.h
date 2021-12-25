#pragma once

#include "../../Interface/LogOutput/ILoggable.h"
#include "../Abstract/ComponentBase.h"
#include "../../Library/math/NormalRand.hpp"
#include "../../Dynamics/Dynamics.h"

using libra::Vector;

class GNSSReceiver : public ComponentBase, public ILoggable
{
  public:
    GNSSReceiver(const int id, const Vector<3> ant_direction, const Vector<3> noise_std, const Dynamics *dynamics);
    void MainRoutine(int count);
    virtual string GetLogHeader() const;
    virtual string GetLogValue() const;

  private:
    //Parameters for receiver
    const int id_;                // ID
    libra::NormalRand nrs_eci_x_, nrs_eci_y_, nrs_eci_z_;     // Random Error for each axis
    Vector<3> antenna_direction_;  // GNSS antenna direction

    //Calculated values
    int is_gnss_sats_visible_;
    Vector<3> position_eci_;
    
    //References
    const Dynamics* dynamics_;

    //Internal Functions
    void CheckAntenna(Vector<3> location_true);
    void AddNoise(Vector<3> location_true);
};




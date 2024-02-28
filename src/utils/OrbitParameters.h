//
// Created by referencecat on 10.02.24.
//

#ifndef UNNAMEDSPACESTRATEGY_ORBITPARAMETERS_H
#define UNNAMEDSPACESTRATEGY_ORBITPARAMETERS_H

struct OrbitalParametersEcliptic {
    double average_angular_velocity, period;
};

struct OrbitalParametersHyperbolic {
    double hyperbolic_excess_speed, impact_parameter;
};

enum class OrbitType {
    ecliptic, parabolic, hyperbolic
};
struct OrbitalParameters {
    OrbitType type;
    double eccentricity, // e >= 0
    semimajor_axis, // in meters
    argument_of_periapsis = 0, // from OX axis
    mean_anomaly_at_epoch0 = 0;

    bool directionCounterClockwise = true;
    union {
        OrbitalParametersEcliptic ecliptic;
        OrbitalParametersHyperbolic hyperbolic;
    };
};

#endif //UNNAMEDSPACESTRATEGY_ORBITPARAMETERS_H

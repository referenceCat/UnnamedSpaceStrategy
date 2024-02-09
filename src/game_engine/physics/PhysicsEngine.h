//
// Created by referencecat on 07.02.24.
//

#ifndef UNNAMEDSPACESTRATEGY_PHYSICSENGINE_H
#define UNNAMEDSPACESTRATEGY_PHYSICSENGINE_H
#include <cstdint>
#define MAX_CELESTIAL_BODIES 100

struct OrbitalParameters {
    double eccentricity = 0, // e >= 0
    semimajor_axis, // in meters
    argument_of_periapsis = 0, // from OX axis
    med_anomaly_epoch_0 = 0,
            average_angular_velocity = -1,
            period = -1,
            hyperbolic_excess_speed = -1,
            impact_parameter = -1;
};

struct CelestialBody {
    int id, parent_id;
    OrbitalParameters orbitParameters;
    double mass, radius, soi_radius;

};


class PhysicsEngine {
    uint64_t time = 0; // ms since 1970
    CelestialBody celestialBodies [MAX_CELESTIAL_BODIES];

public:
    void init();
    int addCelestialBody(int parent_id, OrbitalParameters& orbitalParameters, double mass, double radius);





};


#endif //UNNAMEDSPACESTRATEGY_PHYSICSENGINE_H

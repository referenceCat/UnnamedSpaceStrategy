//
// Created by referencecat on 07.02.24.
//

#ifndef UNNAMEDSPACESTRATEGY_PHYSICSENGINE_H
#define UNNAMEDSPACESTRATEGY_PHYSICSENGINE_H
#include <cstdint>
#include <cmath>
#include <cassert>
#include <cfloat>
#include "src/utils/Vector3d.h"
#define MAX_CELESTIAL_BODIES 100
#define MIN_CELESTIAL_BODY_MASS 1000000
#define MIN_CELESTIAL_BODY_RADIUS 100
#define ECCENTRIC_ANOMALY_ITERATIONS 100
#define HYPERBOLIC_ECCENTRIC_ANOMALY_ITERATIONS 1000
#include "../utils/OrbitParameters.h"

struct CelestialBody {
    int parent_id;
    OrbitalParameters orbitParameters;
    double mass, radius, soi_radius;
    Vector3d position;

};


class PhysicsEngine {
    uint64_t cur_time = 0; // ms since 1970
    CelestialBody celestialBodies [MAX_CELESTIAL_BODIES]; // star always have index 0
    int celestialBodiesN = 0;
    CelestialBody * findCelestialBody(int id);
    double eccentricAnomaly(OrbitalParameters& orbitalParameters, uint64_t time) const;
    double medianAnomaly(OrbitalParameters& orbitalParameters, uint64_t time) const;
    static double averageAngularVelocity(OrbitalParameters& orbitalParameters, double parent_body_mass);
    static double period(OrbitalParameters& orbitalParameters);
    static double periapsis(OrbitalParameters& orbitalParameters) ;
    double apoapsis(OrbitalParameters& orbitalParameters) const;
    Vector3d relativePosition(OrbitalParameters& orbitalParameters, uint64_t time);

public:
    constexpr const static double G_const = 6.67430E-11;
    void init();
    void update(uint64_t delta_time);
    int setupStar(double mass, double radius);
    int addCelestialBody(int parent_id, OrbitalParameters& orbitalParameters, double mass, double radius);
    double getCelestialBodyMass(int id);
    double getCelestialBodyRadius(int id);
    double getCelestialBodySOI(int id);
    OrbitalParameters getOrbitalParametersOfCelestialBody(int id);
    Vector3d getRelativeCelestialBodyPositionAtTime(int id, uint64_t time);
    Vector3d getCelestialBodyPosition(int id);
    int getCelestialBodiesNumber();
    int getParentId(int id);
    uint64_t getTime();
};


#endif //UNNAMEDSPACESTRATEGY_PHYSICSENGINE_H

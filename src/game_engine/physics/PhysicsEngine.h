//
// Created by referencecat on 07.02.24.
//

#ifndef UNNAMEDSPACESTRATEGY_PHYSICSENGINE_H
#define UNNAMEDSPACESTRATEGY_PHYSICSENGINE_H
#include <cstdint>
#include "iostream"
#include <cmath>
#include <cassert>
#include <cfloat>
#include "../utils/Vector3d.h"
#define MAX_CELESTIAL_BODIES 100
#define MAX_OBJECTS 1000
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

struct Object {
    int id;
    int celestial_body_id;
    OrbitalParameters orbitParameters;
    Vector3d position;

};


class PhysicsEngine {
    uint64_t cur_time = 0; // ms since 1970

    CelestialBody celestialBodies [MAX_CELESTIAL_BODIES]; // star always have index 0
    int celestial_body_number = 0;

    Object objects [MAX_OBJECTS]; // star always have index 0
    int objects_number = 0;

    CelestialBody * findCelestialBody(int id);
    double eccentricAnomaly(OrbitalParameters& orbitalParameters, uint64_t time, int parent_mass) const;
    double medianAnomaly(OrbitalParameters& orbitalParameters, uint64_t time, int parent_mass) const;
    double trueAnomaly(OrbitalParameters& orbitalParameters, uint64_t time, int parent_mass) const;
    static double averageAngularVelocity(OrbitalParameters& orbitalParameters, double parent_body_mass);
    static double period(OrbitalParameters& orbitalParameters);
    static double periapsis(OrbitalParameters& orbitalParameters) ;
    double apoapsis(OrbitalParameters& orbitalParameters) const;
    Vector3d relativePosition(OrbitalParameters& orbitalParameters, uint64_t time, int parent_mass);
    static double hyperbolicExcessSpeed(OrbitalParameters &orbitalParameters, double parent_body_mass);
    static double impactParameter(OrbitalParameters &orbitalParameters);
    Object* findObject(int id);
    double meanAnomalyFromTrueAnomaly(double true_anomaly, OrbitalParameters& orbitalParameters);
    double eccentricAnomalyFromTrueAnomaly(double true_anomaly, OrbitalParameters& orbitalParameters);
    double meanAnomalyFromEccentricAnomaly(double eccentric_anomaly, OrbitalParameters& orbitalParameters);
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
    int getCelestialBodyParent(int id);
    uint64_t getTime();

    int addObject(int parent_id, OrbitalParameters& orbitalParameters);
    OrbitalParameters getOrbitalParametersOfObject(int id);
    Vector3d getRelativeObjectPositionAtTime(int id, uint64_t time);
    Vector3d getObjectPosition(int id);
    int getObjectsNumber();
    int getObjectParent(int id);
    void applyAcceleration(int id, Vector3d acceleration);
    void applyAcceleration(int id, Vector3d acceleration, bool debug);
    Vector3d getObjectVelocity(int id, uint64_t time, bool debug);
    Vector3d getObjectVelocity(int id, uint64_t time);
    Vector3d getObjectVelocity(int id);
};


#endif //UNNAMEDSPACESTRATEGY_PHYSICSENGINE_H

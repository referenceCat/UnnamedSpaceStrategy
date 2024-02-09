//
// Created by referencecat on 09.02.24.
//
#include "PhysicsEngine.h"

CelestialBody * PhysicsEngine::findCelestialBody(int id) {
    assert((id >= celestialBodiesN) && "there is no body with this id");
    return &celestialBodies[id];
}

void PhysicsEngine::init() {

}

int PhysicsEngine::addCelestialBody(int parent_id, OrbitalParameters &orbitalParameters, double mass, double radius) {
    assert((mass >= MIN_CELESTIAL_BODY_MASS) && "celestial body mass is too low");
    assert((radius >= MIN_CELESTIAL_BODY_RADIUS) && "celestial body radius is too low");
    assert((orbitalParameters.type == OrbitType::ecliptic) && "celestial body orbit must be ecliptic");
    // todo check how soi of both bodies collide

    celestialBodies[celestialBodiesN].mass = mass;
    celestialBodies[celestialBodiesN].radius = radius;
    celestialBodies[celestialBodiesN].soi_radius = orbitalParameters.semimajor_axis * pow(mass / findCelestialBody(parent_id)->mass, 0.4);
    celestialBodies[celestialBodiesN].orbitParameters = orbitalParameters;
    celestialBodies[celestialBodiesN].orbitParameters.parent_mass = findCelestialBody(parent_id)->mass;
    return celestialBodiesN++;
}

double PhysicsEngine::getMass(int id) {
    return findCelestialBody(id)->mass;
}

double PhysicsEngine::getRadius(int id) {
    return findCelestialBody(id)->radius;
}

double PhysicsEngine::getSOI(int id) {
    return findCelestialBody(id)->soi_radius;
}

int PhysicsEngine::setupStar(double mass, double radius) {
    assert((mass >= MIN_CELESTIAL_BODY_MASS) && "celestial body mass is too low");
    assert((radius >= MIN_CELESTIAL_BODY_RADIUS) && "celestial body radius is too low");
    celestialBodies[0].mass = mass;
    celestialBodies[0].radius = radius;
    celestialBodies[0].soi_radius = DBL_MAX; // todo
    celestialBodiesN++;

    return 0;
}

void PhysicsEngine::tick(uint64_t delta_time) {

}

OrbitalParameters PhysicsEngine::getOrbitalParametersOfCelestialBody(int id) {
    assert((id != 0) && "star doesnt have orbital parameters");
    return findCelestialBody(id)->orbitParameters;
}

Vector3d PhysicsEngine::relativePosition(OrbitalParameters &orbitalParameters, uint64_t time) { //todo for non ecliptic orbits
    double x, y, eccentric_anomaly_value;
    eccentric_anomaly_value = eccentricAnomaly(orbitalParameters, time);
    if (orbitalParameters.type == OrbitType::ecliptic) {
        x = orbitalParameters.semimajor_axis * (cos(eccentric_anomaly_value) - orbitalParameters.eccentricity);
        y = sqrt(1 - orbitalParameters.eccentricity * orbitalParameters.eccentricity) * orbitalParameters.semimajor_axis * sin(eccentric_anomaly_value);
    } else if (orbitalParameters.type == OrbitType::hyperbolic) {
        x = cosh(eccentric_anomaly_value) * orbitalParameters.semimajor_axis - orbitalParameters.semimajor_axis  +
                periapsis(orbitalParameters);
        y = orbitalParameters.semimajor_axis * sqrt(orbitalParameters.eccentricity * orbitalParameters.eccentricity - 1) * sinh(eccentric_anomaly_value);

    } else {
        assert(false); // todo
    }

    return Vector3d(x, y, 0).rotateAroundZ(orbitalParameters.argument_of_periapsis);
}

double PhysicsEngine::eccentricAnomaly(OrbitalParameters &orbitalParameters, uint64_t time) const {
    double M = medianAnomaly(orbitalParameters, time), E = M;
    if (orbitalParameters.type == OrbitType::ecliptic) {
        for (int i = 0; i < ECCENTRIC_ANOMALY_ITERATIONS; i++) E = orbitalParameters.eccentricity * sin(E) + M; //TODO ECCENTRIC_ANOMALY_ITERATIONS value?
    } else{
        for (int i = 0; i < HYPERBOLIC_ECCENTRIC_ANOMALY_ITERATIONS; i++) E = asinh((E + M) / orbitalParameters.eccentricity); //TODO HYPERBOLIC_ECCENTRIC_ANOMALY_ITERATIONS value?
    }

    return E;
}

double PhysicsEngine::medianAnomaly(OrbitalParameters &orbitalParameters, uint64_t time) const {
    if (orbitalParameters.type == OrbitType::ecliptic) return fmod((orbitalParameters.med_anomaly_epoch_0 + time * (long double) averageAngularVelocity(orbitalParameters) / 1000), (M_PI * 2)); //TODO sus to double conv
    if (orbitalParameters.type == OrbitType::hyperbolic) return orbitalParameters.med_anomaly_epoch_0 + sqrt(- (((orbitalParameters.parent_mass / orbitalParameters.semimajor_axis) * G_const) / orbitalParameters.semimajor_axis) / orbitalParameters.semimajor_axis) * time / 1000;
}

double PhysicsEngine::averageAngularVelocity(OrbitalParameters &orbitalParameters) {
    assert(orbitalParameters.type == OrbitType::ecliptic && "cannot get aav of non ecliptic orbit");
    return sqrt(orbitalParameters.parent_mass * G_const / pow(orbitalParameters.semimajor_axis, 3));
}

double PhysicsEngine::period(OrbitalParameters &orbitalParameters) {
    assert(orbitalParameters.type == OrbitType::ecliptic && "cannot get period of non ecliptic orbit");
    return 2 * M_PI / averageAngularVelocity(orbitalParameters);
}

double PhysicsEngine::periapsis(OrbitalParameters &orbitalParameters) {
    return (1 - orbitalParameters.eccentricity) * orbitalParameters.semimajor_axis;
}

double PhysicsEngine::apoapsis(OrbitalParameters &orbitalParameters) const {
    assert((orbitalParameters.type == OrbitType::ecliptic) && "cannot to get apoapsis of non ecliptic orbit");
    return (1 + orbitalParameters.eccentricity) * orbitalParameters.semimajor_axis;
}

Vector3d PhysicsEngine::celestialBodyPosition(int id, uint64_t time) {
    if (id == 0) return {0, 0};
    CelestialBody* body = findCelestialBody(id);
    return relativePosition(body->orbitParameters, time) + celestialBodyPosition(body->parent_id, time);
}


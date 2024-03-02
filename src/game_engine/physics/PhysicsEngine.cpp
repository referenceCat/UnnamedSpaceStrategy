//
// Created by referencecat on 09.02.24.
//
#include "PhysicsEngine.h"

CelestialBody * PhysicsEngine::findCelestialBody(int id) {
    assert((id <= celestial_body_number) && "there is no body with this id");
    return &celestialBodies[id];
}

void PhysicsEngine::init() {

}

int PhysicsEngine::addCelestialBody(int parent_id, OrbitalParameters &orbitalParameters, double mass, double radius) {
    assert((mass >= MIN_CELESTIAL_BODY_MASS) && "celestial body mass is too low");
    assert((radius >= MIN_CELESTIAL_BODY_RADIUS) && "celestial body radius is too low");
    assert((orbitalParameters.type == OrbitType::ecliptic) && "celestial body orbit must be ecliptic");
    // todo check how soi of both bodies collide

    double parent_mass = findCelestialBody(parent_id)->mass;
    celestialBodies[celestial_body_number].parent_id = parent_id;
    celestialBodies[celestial_body_number].mass = mass;
    celestialBodies[celestial_body_number].radius = radius;
    celestialBodies[celestial_body_number].soi_radius = orbitalParameters.semimajor_axis * pow(mass / parent_mass, 0.4);
    celestialBodies[celestial_body_number].orbitParameters = orbitalParameters;
    celestialBodies[celestial_body_number].orbitParameters.ecliptic.average_angular_velocity = averageAngularVelocity(celestialBodies[celestial_body_number].orbitParameters, parent_mass);
    celestialBodies[celestial_body_number].orbitParameters.ecliptic.period = period(celestialBodies[celestial_body_number].orbitParameters);
    return celestial_body_number++;
}

double PhysicsEngine::getCelestialBodyMass(int id) {
    return findCelestialBody(id)->mass;
}

double PhysicsEngine::getCelestialBodyRadius(int id) {
    return findCelestialBody(id)->radius;
}

double PhysicsEngine::getCelestialBodySOI(int id) {
    return findCelestialBody(id)->soi_radius;
}

int PhysicsEngine::setupStar(double mass, double radius) {
    assert((mass >= MIN_CELESTIAL_BODY_MASS) && "celestial body mass is too low");
    assert((radius >= MIN_CELESTIAL_BODY_RADIUS) && "celestial body radius is too low");
    celestialBodies[0].mass = mass;
    celestialBodies[0].radius = radius;
    celestialBodies[0].soi_radius = DBL_MAX; // todo
    celestial_body_number++;

    return 0;
}

void PhysicsEngine::update(uint64_t delta_time) {
    current_time_milliseconds += delta_time;
    for (int i = 1; i < celestial_body_number; i++) {
        celestialBodies[i].position = getRelativeCelestialBodyPositionAtTime(i, current_time_milliseconds) + getCelestialBodyPosition(celestialBodies[i].parent_id);
    }

    for (int i = 0; i < objects_number; i++) {
        objects[i].position = getRelativeObjectPositionAtTime(i, current_time_milliseconds) + getCelestialBodyPosition(objects[i].celestial_body_id);
    }

}

OrbitalParameters PhysicsEngine::getOrbitalParametersOfCelestialBody(int id) {
    assert((id != 0) && "star doesnt have orbital parameters");
    return findCelestialBody(id)->orbitParameters;
}

Vector3d PhysicsEngine::relativePosition(OrbitalParameters &orbitalParameters, uint64_t time, double parent_mass) { //todo for non ecliptic orbits
    double x, y, eccentric_anomaly_value;
    eccentric_anomaly_value = eccentricAnomaly(orbitalParameters, time, parent_mass);
    if (orbitalParameters.type == OrbitType::ecliptic) {
        x = orbitalParameters.semimajor_axis * (cos(eccentric_anomaly_value) - orbitalParameters.eccentricity);
        y = sqrt(1 - orbitalParameters.eccentricity * orbitalParameters.eccentricity) * orbitalParameters.semimajor_axis * sin(eccentric_anomaly_value) * (orbitalParameters.directionCounterClockwise ? 1 : -1);
    } else if (orbitalParameters.type == OrbitType::hyperbolic) {
        x = cosh(eccentric_anomaly_value) * orbitalParameters.semimajor_axis - orbitalParameters.semimajor_axis  +
                periapsis(orbitalParameters);
        y = - orbitalParameters.semimajor_axis * sqrt(orbitalParameters.eccentricity * orbitalParameters.eccentricity - 1) * sinh(eccentric_anomaly_value) * (orbitalParameters.directionCounterClockwise ? 1 : -1);;

    } else {
        assert(false); // todo
    }

    return Vector3d(x, y, 0).rotateAroundZ(orbitalParameters.argument_of_periapsis);
}

double PhysicsEngine::eccentricAnomaly(OrbitalParameters &orbitalParameters, uint64_t time, double parent_mass) const {
    double M = meanAnomaly(orbitalParameters, time, parent_mass), E = M;
    if (orbitalParameters.type == OrbitType::ecliptic) {
        for (int i = 0; i < ECCENTRIC_ANOMALY_ITERATIONS; i++) E = orbitalParameters.eccentricity * sin(E) + M; //TODO ECCENTRIC_ANOMALY_ITERATIONS value?
    } else{
        for (int i = 0; i < HYPERBOLIC_ECCENTRIC_ANOMALY_ITERATIONS; i++) E = asinh((E + M) / orbitalParameters.eccentricity); //TODO HYPERBOLIC_ECCENTRIC_ANOMALY_ITERATIONS value?
    }

    return E;
}

double PhysicsEngine::meanAnomaly(OrbitalParameters &orbitalParameters, uint64_t time, double parent_mass) const {
    if (orbitalParameters.type == OrbitType::ecliptic) return fmod((orbitalParameters.mean_anomaly_at_epoch0 + time * (long double) orbitalParameters.ecliptic.average_angular_velocity / 1000), (M_PI * 2)); //TODO sus to double conv
    if (orbitalParameters.type == OrbitType::hyperbolic) return orbitalParameters.mean_anomaly_at_epoch0 + sqrt(- (((parent_mass / orbitalParameters.semimajor_axis) * G_const) / orbitalParameters.semimajor_axis) / orbitalParameters.semimajor_axis) * time / 1000;
}

double PhysicsEngine::averageAngularVelocity(OrbitalParameters &orbitalParameters, double parent_body_mass) {
    assert(orbitalParameters.type == OrbitType::ecliptic && "cannot get aav of non ecliptic orbit");
    return sqrt(parent_body_mass * G_const / pow(orbitalParameters.semimajor_axis, 3));
}

double PhysicsEngine::period(OrbitalParameters &orbitalParameters) {
    assert(orbitalParameters.type == OrbitType::ecliptic && "cannot get period of non ecliptic orbit");
    return 2 * M_PI / orbitalParameters.ecliptic.average_angular_velocity;
}

double PhysicsEngine::periapsis(OrbitalParameters &orbitalParameters) {
    return (1 - orbitalParameters.eccentricity) * orbitalParameters.semimajor_axis;
}

double PhysicsEngine::apoapsis(OrbitalParameters &orbitalParameters) const {
    assert((orbitalParameters.type == OrbitType::ecliptic) && "cannot to get apoapsis of non ecliptic orbit");
    return (1 + orbitalParameters.eccentricity) * orbitalParameters.semimajor_axis;
}

Vector3d PhysicsEngine::getRelativeCelestialBodyPositionAtTime(int id, uint64_t time) {
    if (id == 0) return {0, 0};
    CelestialBody* body = findCelestialBody(id);
    return relativePosition(body->orbitParameters, time, findCelestialBody(body->parent_id)->mass);
}

Vector3d PhysicsEngine::getCelestialBodyPosition(int id) {
    return findCelestialBody(id)->position;
}

int PhysicsEngine::getCelestialBodiesNumber() {
    return celestial_body_number;
}

int PhysicsEngine::getCelestialBodyParent(int id) {
    assert(id != 0 && "star doesnt have parent celestial body");
    return findCelestialBody(id)->parent_id;
}

uint64_t PhysicsEngine::getTime() {
    return current_time_milliseconds;
}

Object *PhysicsEngine::findObject(int id) {
    assert((id <= objects_number) && "there is no object with this id");
    return &objects[id];
}

int PhysicsEngine::addObject(int parent_id, OrbitalParameters &orbitalParameters) {
    double parent_mass = findCelestialBody(parent_id)->mass;
    objects[objects_number].celestial_body_id = parent_id;
    objects[objects_number].orbitalParameters = orbitalParameters;
    if (orbitalParameters.type == OrbitType::ecliptic) {
        objects[objects_number].orbitalParameters.ecliptic.average_angular_velocity = averageAngularVelocity(objects[objects_number].orbitalParameters, parent_mass);
        objects[objects_number].orbitalParameters.ecliptic.period = period(objects[objects_number].orbitalParameters);
    } else if (orbitalParameters.type == OrbitType::hyperbolic) {
        objects[objects_number].orbitalParameters.hyperbolic.hyperbolic_excess_speed = hyperbolicExcessSpeed(objects[objects_number].orbitalParameters, parent_mass);
        objects[objects_number].orbitalParameters.hyperbolic.impact_parameter = impactParameter(objects[objects_number].orbitalParameters);
    }
    return objects_number++;
}

OrbitalParameters PhysicsEngine::getOrbitalParametersOfObject(int id) {
    return findObject(id)->orbitalParameters;
}

Vector3d PhysicsEngine::getRelativeObjectPositionAtTime(int id, uint64_t time) {
    return relativePosition(findObject(id)->orbitalParameters, time, findCelestialBody(findObject(id)->celestial_body_id)->mass);
}

Vector3d PhysicsEngine::getObjectPosition(int id) {
    return findObject(id)->position;
}

int PhysicsEngine::getObjectsNumber() {
    return objects_number;
}

int PhysicsEngine::getObjectParent(int id) {
    return findObject(id)->celestial_body_id;
}

void PhysicsEngine::applyAcceleration(int id, Vector3d acceleration, bool debug) {
    Object* object = findObject(id);
    Vector3d position = getRelativeObjectPositionAtTime(id, current_time_milliseconds);
    double parent_mass = findCelestialBody(object->celestial_body_id)->mass;
    Vector3d expected_velocity = getObjectVelocity(id) + acceleration;
    object->orbitalParameters = calculateOrbitalParameters(position, expected_velocity, parent_mass, current_time_milliseconds);
    if (debug) {
        Vector3d result_velocity = getObjectVelocity(id, current_time_milliseconds, debug);
        if (abs(expected_velocity.x - result_velocity.x) > 0.3 || abs(expected_velocity.y - result_velocity.y) > 0.2 || result_velocity.x == NAN || result_velocity.y == NAN) {
            std::cout << expected_velocity.x << ' ' << expected_velocity.y << ' ' << result_velocity.x << ' ' << result_velocity.y << std::endl;
            Vector3d real_velocity = (relativePosition(object->orbitalParameters, current_time_milliseconds + 10, parent_mass) - relativePosition(object->orbitalParameters, current_time_milliseconds, parent_mass)) * 100;
            std::cout << real_velocity.x << ' ' << result_velocity.y << std::endl;
        }
    }
}

double PhysicsEngine::hyperbolicExcessSpeed(OrbitalParameters& orbitalParameters, double parent_body_mass) {
    assert(orbitalParameters.type==OrbitType::hyperbolic && "cannot get hes for non hyperbolic orbit");
    double hyperbolic_excess_speed = sqrt(- G_const * parent_body_mass / orbitalParameters.semimajor_axis);
    return hyperbolic_excess_speed;
}

double PhysicsEngine::impactParameter(OrbitalParameters& orbitalParameters) {
    assert(orbitalParameters.type==OrbitType::hyperbolic  && "cannot get impact parameter for non hyperbolic orbit");
    double impact_parameter = -orbitalParameters.semimajor_axis * (orbitalParameters.eccentricity * orbitalParameters.eccentricity  - 1);
    return impact_parameter;
}

double PhysicsEngine::trueAnomaly(OrbitalParameters &orbitalParameters, uint64_t time, double parent_mass) const {
    double true_anomaly,  eccentric_anomaly = eccentricAnomaly(orbitalParameters, time, parent_mass);
    if (orbitalParameters.type == OrbitType::ecliptic) {
        double beta = orbitalParameters.eccentricity /
                      (1 + sqrt(1 - orbitalParameters.eccentricity * orbitalParameters.eccentricity));
        true_anomaly = eccentric_anomaly + 2 * atan2(beta * sin(eccentric_anomaly), 1 - beta * cos(eccentric_anomaly));
    } else {
        true_anomaly = atan(sqrt((orbitalParameters.eccentricity + 1) / (orbitalParameters.eccentricity - 1)) * tanh(eccentric_anomaly/2)) * 2;
    }
    true_anomaly *= (orbitalParameters.directionCounterClockwise ? 1: -1);
    return true_anomaly;
}



Vector3d PhysicsEngine::getObjectVelocity(int id, uint64_t time) {
    return getObjectVelocity(id, time, false);
}

Vector3d PhysicsEngine::getObjectVelocity(int id) {
    return getObjectVelocity(id, current_time_milliseconds);
}

double PhysicsEngine::meanAnomalyFromTrueAnomaly(double true_anomaly, OrbitalParameters& orbitalParameters) {
    return meanAnomalyFromEccentricAnomaly(eccentricAnomalyFromTrueAnomaly(true_anomaly, orbitalParameters), orbitalParameters);
}

double PhysicsEngine::eccentricAnomalyFromTrueAnomaly(double true_anomaly, OrbitalParameters& orbitalParameters) {
    return atan2(sqrt(1 - orbitalParameters.eccentricity * orbitalParameters.eccentricity) * sin(true_anomaly), cos(true_anomaly) + orbitalParameters.eccentricity);
}

double PhysicsEngine::meanAnomalyFromEccentricAnomaly(double eccentric_anomaly, OrbitalParameters& orbitalParameters) {
    return eccentric_anomaly - orbitalParameters.eccentricity * sin(eccentric_anomaly);
}

void PhysicsEngine::applyAcceleration(int id, Vector3d acceleration) {
    applyAcceleration(id, acceleration, false);
}

Vector3d PhysicsEngine::getObjectVelocity(int id, uint64_t time, bool debug) {
    Object* object = findObject(id);
    double parent_mass = findCelestialBody(object->celestial_body_id)->mass;
    double true_anomaly = trueAnomaly(object->orbitalParameters, time, parent_mass);
    Vector3d position = relativePosition(object->orbitalParameters, time, parent_mass);
    double velocity = sqrt(G_const * parent_mass * (2 / position.mag() - 1 / object->orbitalParameters.semimajor_axis));
    double periapsis_velocity = sqrt(G_const * parent_mass * (2 / periapsis(object->orbitalParameters) - 1 / object->orbitalParameters.semimajor_axis));
    double flight_path_angle = acos(periapsis(object->orbitalParameters) * periapsis_velocity / velocity / position.mag()) * (true_anomaly > 0 && true_anomaly < M_PI ? 1 : -1);
    Vector3d velocityVec;
    velocityVec.x = velocity * cosl((long double) true_anomaly + object->orbitalParameters.argument_of_periapsis + M_PI / 2 - flight_path_angle);
    velocityVec.y = velocity * sinl((long double) true_anomaly + object->orbitalParameters.argument_of_periapsis + M_PI / 2 - flight_path_angle);
    velocityVec.x *= (object->orbitalParameters.directionCounterClockwise ? 1 : -1);
    velocityVec.y *= (object->orbitalParameters.directionCounterClockwise ? 1 : -1);
    return velocityVec;
}

OrbitalParameters
PhysicsEngine::calculateOrbitalParameters(Vector3d localPosition, Vector3d localVelocity, double parent_mass,
                                          uint64_t time) {
    OrbitalParameters orbitalParameters;
    // double old_true_anomaly = trueAnomaly(object->orbitalParameters, current_time_milliseconds, parent_mass);
    // double old_argument_of_periapsis = object->orbitalParameters.argument_of_periapsis;
    Vector3d h_vec = localPosition ^ localVelocity; // specific angular momentum
    if (h_vec.z > 0) orbitalParameters.directionCounterClockwise = false;

    Vector3d eccentricityVec = (localVelocity ^ h_vec) / (G_const * parent_mass) - localPosition.norm();
    orbitalParameters.eccentricity = (eccentricityVec).mag();
    if(orbitalParameters.eccentricity < 1) orbitalParameters.type = OrbitType::ecliptic;
    double specific_orbital_energy = localVelocity.mag() * localVelocity.mag() / 2 - G_const * parent_mass / localPosition.mag(); // specific orbital energy
    orbitalParameters.semimajor_axis = G_const * parent_mass / (-2 * specific_orbital_energy);
    orbitalParameters.ecliptic.average_angular_velocity = averageAngularVelocity(orbitalParameters, parent_mass);
    orbitalParameters.ecliptic.period = period(orbitalParameters);
    orbitalParameters.argument_of_periapsis = atanl((long double) eccentricityVec.y / (long double) eccentricityVec.x) + ((eccentricityVec.x < 0 ? M_PI : 0));

    double true_anomaly = atanl((long double) localPosition.y / (long double) localPosition.x) + ((localPosition.x < 0 ? M_PI : 0));
    true_anomaly -= orbitalParameters.argument_of_periapsis;
    if (!orbitalParameters.directionCounterClockwise) true_anomaly *= -1;
    true_anomaly = fmod(true_anomaly + M_PI * 4, M_PI * 2);


    double mean_anomaly_absolute_value = std::abs(fmod(meanAnomalyFromTrueAnomaly(true_anomaly, orbitalParameters), M_PI * 2));
    double mean_anomaly = mean_anomaly_absolute_value;
    double cosine_of_fpa = localPosition.norm() * localVelocity.norm();

    if (localPosition.norm() * localVelocity.norm() < -0.01) mean_anomaly = mean_anomaly_absolute_value * -1;
    else if (localPosition.norm() * localVelocity.norm() < 0.01) {
        // cant determine mean_anomaly sign because vertical velocity and |FPA| are too low
        if (true_anomaly > M_PI || true_anomaly < 0) mean_anomaly = mean_anomaly_absolute_value * -1;
    }

    // double new_true_anomaly = old_true_anomaly + old_argument_of_periapsis - object->orbitalParameters.argument_of_periapsis;
    // new_true_anomaly *= (object->orbitalParameters.directionCounterClockwise ? 1 : -1);
    // if (new_true_anomaly < 0) new_true_anomaly += M_PI * 2;
    orbitalParameters.mean_anomaly_at_epoch0 = mean_anomaly - current_time_milliseconds * (long double) orbitalParameters.ecliptic.average_angular_velocity / 1000;
    // assert(mean_anomaly > 0);
    return orbitalParameters;
}

Vector3d PhysicsEngine::positionDifferentialTimes1000(int id, uint64_t time) {
    Object* object = findObject(id);
    double parent_mass = findCelestialBody(object->celestial_body_id)->mass;
    return (relativePosition(object->orbitalParameters, time + 1, parent_mass) - relativePosition(object->orbitalParameters, time, parent_mass)) * 1000;
}

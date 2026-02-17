#include "engine/app.h"
#include "sim/particle.h"

int main() {
    Simulation sim = particle_sim();
    sim.init();

    AppConfig config = {800, 600, "Physics Test"};
    app_setup(config, sim.physics, sim.render);

    return 0;
}

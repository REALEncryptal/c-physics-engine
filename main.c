#include "engine/app.h"
#include "sim/particle.h"

int main() {
    AppConfig config = {800, 600, "Physics Test"};
    Simulation sim = particle_sim();
    app_setup(config, &sim);

    return 0;
}

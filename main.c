#include "engine/app.h"

void physics(float dt) {
    (void)dt;
}

void render(void) {
}

int main() {
    AppConfig config = {800, 600, "Physics Test"};
    app_setup(config, physics, render);
    return 0;
}
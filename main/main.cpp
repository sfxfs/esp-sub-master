#include "execution/thruster.h"
#include "execution/pca9685.h"

extern "C" void app_main(void)
{
    // Create an instance of the Thruster class
    SubMaster::Thruster thruster(SubMaster::PCA9685(nullptr, 0, 50));
}

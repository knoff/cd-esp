#include <drv_valve/drv_valve.hpp>
#include <rpk_pump/rpk_pump.hpp>
namespace cd {
namespace rpk {
namespace pump {
void init() { cd::drv::valve_close(); /* остальное позже */ }
} // namespace pump
} // namespace rpk
} // namespace cd

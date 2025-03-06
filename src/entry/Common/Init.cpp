#include "Init.h"
#include "State.h"

namespace vr
{
namespace
{
XWinState xWinState;
}

bool init(MainArgs)
{
    xWinState = XWinState(MainArgsVars);
    return true;
}
const XWinState& getXWinState() { return xWinState; }
}

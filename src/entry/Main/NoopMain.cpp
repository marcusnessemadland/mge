#include "../Common/Init.h"
#include "Main.h"

#ifndef BX_PLATFORM_WINDOWS
int main(int argc, char** argv)
{
    xwin::init(argc, (const char**)argv);
    _main_(argc, (const char**)argv);
    return 0;
}
#endif // !BX_PLATFORM_WINDOWS

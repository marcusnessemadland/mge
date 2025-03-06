#include "../Init.h"
#include "../Main.h"

int main(int argc, char** argv)
{
    vr::init(argc, (const char**)argv);
    _main_(argc, (const char**)argv);
    return 0;
}
#include "../log.h"
int main()
{
    LOGGER.Initialize("test_log","./",100*1024*1024,10,spdlog::level::level_enum::debug);
    trace_LOG<<"I am trace log!"; // we won't see this line in log file.
    debug_LOG<<"I am debug log!";
    info_LOG<<"I am info log!";
    error_LOG<<"I am error log!";
    fatal_LOG<<"I am critical log!";
    info_LOG<<"int:"<<1<<",float:"<<2.33<<",double:"<<2.3333333333<<"\n another line!";
    return 0;
}
#include "FrameHeader.h"

ProfilerData Profiler::mData[MAX_POINTS];
double Profiler::mTickToNS = 0.0;
double Profiler::mTickToMS = 0.0;
ullong Profiler::mTickPerMS = 0;
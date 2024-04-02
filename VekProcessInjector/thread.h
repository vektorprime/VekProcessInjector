#pragma once

#include <thread>

#include "config.h"

//used as a bridge to launch different kinds of injection threads

void startInjectionThreadBeforeProc(Config config);
void startInjectionThreadAfterProc(Config config);
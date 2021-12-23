#pragma once

#include <xtl.h>
#include <xboxmath.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <time.h>
#include <vector>
#include <array>
#include <ppcintrinsics.h>
#include <math.h>
#include <fstream>
#include <cstdio>
#include <tchar.h>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <xhttp.h>
#include <string>
#include <xaudio2.h>
#include <xact3wb.h>
#include <functional>
#include "xkelib.h"

using namespace std;

#define S_FAIL -1
#define KERNEL_VERSION 0x4497

#include "Global.h"
#include "Log.h"
#include "Utils.h"
#include "SMC.h"
#include "Hypervisor.h"

#include "Requests.h"
#include "Network.h"

#include "Hooking.h"

#include "TitleHooks.h"
#include "systemHooks.h"
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <memory>
#include <algorithm>
#include <list>
#include <functional>
#include <ctime>
#include <sstream>
#include <stdlib.h>
#include <thread>
#include <atomic>
#include <ctime>
#include <chrono>
#include <exception>
#include <boost\thread.hpp>
#include <boost\atomic.hpp>

namespace
{
	const char _BOT_KNOWLEDGE_RELATIVE_PATH_[] = ""; // must end with "/" if any
	const char _BOT_DATA_RELATIVE_PATH_[] = ""; // must end with "/" if any
	const char _BOT_AI_RELATIVE_PATH_[] = ""; // must end with "/" if any
	const double CLOCKS_PER_MILLISEC = double(CLOCKS_PER_SEC) / 1000.0;
}

inline static int mod(int number, int modulo)
{
	int ret = number % modulo;
	
	if (ret < 0)
	{
		ret += modulo;
	}

	return ret;
}
#ifndef _DA_GUARDIAN
#define _DA_GUARDIAN


#include <stdio.h>
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
#include <future>
#include <exception>
#include <ratio>

namespace
{
	const char _BOT_KNOWLEDGE_RELATIVE_PATH_[] = "../data/ktables/"; // must end with "/" if any
	const char _BOT_AI_RELATIVE_PATH_[] = "../data/bots/"; // must end with "/" if any
	const char _RULZ_RELATIVE_PATH_[] = "../data/rules/"; // must end with "/" if any
	const char _LOG_RELATIVE_PATH_[] = "../data/logs/"; // must end with "/" if any
	const char _GAME_DATA_RELATIVE_PATH_[] = "../data/games/"; // must end with "/" if any
	const char _RESULTS_RELATIVE_PATH_[] = "../data/results/"; // must end with "/" if any
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

#endif // DA_GUARDIAN

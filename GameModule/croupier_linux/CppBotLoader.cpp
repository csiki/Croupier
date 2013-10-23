#include "stdafx.h"
#include "CppBotLoader.h"

Bot* CppBotLoader::loadBot(BotData* botData)
{
	// load the dll library
	std::string libPath = _BOT_AI_RELATIVE_PATH_;
	libPath += botData->src;
	void* botLib = dlopen(libPath.c_str(), RTLD_NOW);

	if (!botLib)
	{
		throw BotLoaderException(dlerror());
		return nullptr;
	}

	// reset errors
	dlerror();

	/// load factory functions
	// create
	create_bot* create = (create_bot*) dlsym(botLib, "create");
	const char* dlsym_error = dlerror();
	if (dlsym_error)
	{
		throw BotLoaderException(dlsym_error);
		return nullptr;
	}

	// destroy
	destroy_bot* destroy = (destroy_bot*) dlsym(botLib, "destroy");
	dlsym_error = dlerror();
	if (dlsym_error)
	{
		throw BotLoaderException(dlsym_error);
		return nullptr;
	}

	// create an instance of the class
	Bot* bot = create(botData->communicator, botData->id, botData->name, botData->lang);

	// append to destroyers
	this->destroyers.insert(std::pair<Bot*, destroy_bot*>(bot, destroy));

	return bot;
}

Process* CppBotLoader::initProcess()
{
	// no extra process needed
	return nullptr;
}
#include "stdafx.h"
#include "Bot.h"

/**	Returns AI's id.
*/
int Bot::getID() const
{
	return this->id;
}

/** Returns the programming language the AI is written in.
*/
int Bot::getLang() const
{
	return this->lang;
}

/**	Returns AI's name.
*/
string Bot::getName() const
{
	return this->name;
}
#include "../Header/Player.h"
#include "../Header/Scripter.h"

void Player::_Bomb()
{
	Scripter::scr.eventExecute(SCR_EVENT_PLAYERBOMB, bombtimer);
}
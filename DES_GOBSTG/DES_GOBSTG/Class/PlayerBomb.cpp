#include "Player.h"
#include "Scripter.h"

void Player::_Bomb()
{
	scr.eventExecute(SCR_EVENT_PLAYERBOMB, bombtimer);
}
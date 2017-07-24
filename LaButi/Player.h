#pragma once

#include "VeryData.h"

struct IAPlayer
{
	virtual TrumfoCantat CantaTrumfo() = 0;
	virtual TrumfoCantat CantaTrumfoDelegat() = 0;

	virtual bool DecidirContrar() = 0;
	virtual bool DecidirRecontrar() = 0;
	virtual bool DecidirStVicenc() = 0;

	virtual Carta JugaCarta(Carta* ma, int ma_size, int player, InfoBasa& basa) = 0;
};

#pragma once

#include "VeryData.h"

struct IAPlayer
{
	virtual TrumfoCantat CantaTrumfo() = 0;
	virtual TrumfoCantat CantaTrumfoDelegat() = 0;

	virtual bool DecidirContrar() = 0;
	virtual bool DecidirRecontrar() = 0;
	virtual bool DecidirStVicenc() = 0;

	virtual Carta JugaCarta(InfoPartida& partida, int id, InfoBasa& basa) = 0;
};

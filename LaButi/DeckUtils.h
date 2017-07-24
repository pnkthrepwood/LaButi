#pragma once

#include "VeryData.h"
#include "Random.h"

namespace DeckUtils
{
	void OrdenaBaralla(Baralla& deck)
	{
		char* pal_char = "BCEO";
		for (int i = 0; i < 12 * 4; ++i)
		{
			deck[i].valor = (Valor)(2 + ((i % 12)));
			deck[i].pal = (Pal)(pal_char[(i / 12)]);
		}
	}

	void init()
	{
		OrdenaBaralla(deck_ordenat);
	}

	void Remena(Baralla& deck)
	{
		bool comptats[48];
		for (int i = 0; i < 48; ++i) comptats[i] = false;

		int deck_next = 0;
		while (deck_next < 48)
		{
			int rnd = Random::Roll(0, 48);
			if (!comptats[rnd])
			{
				comptats[rnd] = true;
				deck[deck_next].valor = deck_ordenat[rnd].valor;
				deck[deck_next].pal = deck_ordenat[rnd].pal;
				deck_next++;
			}
		}
	}

	void Talla(Baralla& deck)
	{
		//Que talli pel mig mes o menys
		int n = 18 + (rand() % 10);

		Carta aux[48];
		for (int i = 0; i < n; ++i)
		{
			aux[i].valor = deck[i].valor;
			aux[i].pal = deck[i].pal;
		}
		for (int i = 0; i < 48 - n; ++i)
		{
			deck[i].valor = deck[i + n].valor;
			deck[i].pal = deck[i + n].pal;
		}
		for (int i = 0; i < n; ++i)
		{
			deck[i + 48 - n].valor = aux[i].valor;
			deck[i + 48 - n].pal = aux[i].pal;
		}
	}

	void Reparteix(Jugador* players, Baralla& deck)
	{
		for (int i = 0; i < 4; ++i)
		{
			players[i].ma_size = 12;
		}

		int deck_counter = 0;
		for (int i = 0; i < 3; ++i) //3 packs de 4 cartes a cada player
		{
			for (int j = 0; j < 4; ++j) //Un pack per player
			{
				for (int k = 0; k < 4; ++k) //Les cartes
				{
					players[j].ma[i * 4 + k].valor = deck[deck_counter].valor;
					players[j].ma[i * 4 + k].pal = deck[deck_counter].pal;
					deck_counter++;
				}
			}
		}
	}
}

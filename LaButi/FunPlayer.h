#pragma once

#include "Player.h"
#include "Random.h"

struct FunPlayer : IAPlayer
{

	bool DecidirContrar()
	{
		return Random::Roll(0, 100) < 10;
	}

	bool DecidirRecontrar()
	{
		return Random::Roll(0, 100) < 5;
	}

	bool DecidirStVicenc()
	{
		return Random::Roll(0, 100) < 1;
	}

	TrumfoCantat CantaTrumfo()
	{
		int r = Random::Roll(0, 5);
		if (r == 0) return TrumfoCantat::BASTOS;
		if (r == 1) return TrumfoCantat::COPES;
		if (r == 2) return TrumfoCantat::ESPASES;
		if (r == 3) return TrumfoCantat::OROS;
		if (r == 4) return TrumfoCantat::BUTI;
		return TrumfoCantat::DELEGO;
	}

	TrumfoCantat CantaTrumfoDelegat()
	{
		TrumfoCantat r = CantaTrumfo();
		while (r == TrumfoCantat::DELEGO) r = CantaTrumfo();
		return r;
	}

	Carta JugaCarta(Carta* ma, int ma_size, int player, InfoBasa& basa)
	{
		// Es poden jugar totes de moment
		bool jugables[12];
		for (int i = 0; i < 12; ++i)
		{
			jugables[i] = true;
		}

		// Basa començada
		if (basa.num_carta > 0)
		{
			Pal pal_basa = basa.cartes[0].pal;

			// Si tinc pal de basa, he de jugar pal de basa
			if (JugadorTePal(pal_basa, ma, ma_size))
			{
				for (int i = 0; i < ma_size; ++i)
				{
					if (ma[i].pal != pal_basa)
					{
						jugables[i] = false;
					}
				}
			}

			// Si puc guanyar, guanyo
			bool puc_guanyar = false;
			for (int i = 0; i < ma_size; ++i)
			{
				if (jugables[i]
					&&  basa.guanyadora.pal == pal_basa
					&&	ma[i].pal == pal_basa
					&&	ma[i].valor > basa.guanyadora.valor)
				{
					puc_guanyar = true;
					break;
				}
				if (jugables[i]
					&& basa.trumfo != (TrumfoCantat::BUTI)
					&& basa.guanyadora.pal == (Pal)basa.trumfo
					&& ma[i].pal == (Pal)basa.trumfo
					&& ma[i].valor > basa.guanyadora.valor)
				{
					puc_guanyar = true;
					break;
				}
				if (jugables[i]
					&& basa.trumfo != (TrumfoCantat::BUTI)
					&& basa.guanyadora.pal != (Pal)basa.trumfo
					&& ma[i].pal == (Pal)basa.trumfo)
				{
					puc_guanyar = true;
					break;
				}
			}

			if (puc_guanyar)
			{
				for (int i = 0; i < ma_size; ++i)
				{
					if (jugables[i]
						&& basa.guanyadora.pal == pal_basa
						&&	ma[i].pal == pal_basa
						&&	ma[i].valor > basa.guanyadora.valor)
					{
						continue;
					}
					else if (jugables[i]
						&& basa.trumfo != (TrumfoCantat::BUTI)
						&& basa.guanyadora.pal == (Pal)basa.trumfo
						&& ma[i].pal == (Pal)basa.trumfo
						&& ma[i].valor > basa.guanyadora.valor)
					{
						continue;
					}
					else if (jugables[i]
						&& basa.trumfo != (TrumfoCantat::BUTI)
						&& basa.guanyadora.pal != (Pal)basa.trumfo
						&& ma[i].pal == (Pal)basa.trumfo)
					{
						continue;
					}
					else
					{
						jugables[i] = false;
					}
				}
			}
		}

		// Juga una carta
		int r = Random::Roll(0, ma_size-1);
		while (!jugables[r])
		{
			r = Random::Roll(0, ma_size-1);
		}

		Carta jugada;
		jugada.pal = ma[r].pal;
		jugada.valor = ma[r].valor;

		return jugada;
	}
};









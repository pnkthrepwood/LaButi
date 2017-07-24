#pragma once

#include "Player.h"
#include "Random.h"

struct FunPlayer : IAPlayer
{

	bool DecidirContrar()
	{
		return false;
	}

	bool DecidirRecontrar()
	{
		return false;
	}

	bool DecidirStVicenc()
	{
		return false;
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

	Carta JugaCarta(InfoPartida& partida, int player, InfoBasa& basa)
	{
		Carta* ma = partida.players[player].ma;
		int& ma_size = partida.players[player].ma_size;

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
					&& basa.guanyadora.pal == pal_basa
					&&	ma[i].pal == pal_basa
					&&	ma[i].valor > basa.guanyadora.valor)
				{
					puc_guanyar = true;
					break;
				}
				if (jugables[i]
					&& partida.trumfo != (TrumfoCantat::BUTI)
					&& basa.guanyadora.pal == (Pal)partida.trumfo
					&& ma[i].pal == (Pal)partida.trumfo
					&& ma[i].valor > basa.guanyadora.valor)
				{
					puc_guanyar = true;
					break;
				}
				if (jugables[i]
					&& partida.trumfo != (TrumfoCantat::BUTI)
					&& basa.guanyadora.pal != (Pal)partida.trumfo
					&& ma[i].pal == (Pal)partida.trumfo)
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
						&& partida.trumfo != (TrumfoCantat::BUTI)
						&& basa.guanyadora.pal == (Pal)partida.trumfo
						&& ma[i].pal == (Pal)partida.trumfo
						&& ma[i].valor > basa.guanyadora.valor)
					{
						continue;
					}
					else if (jugables[i]
						&& partida.trumfo != (TrumfoCantat::BUTI)
						&& basa.guanyadora.pal != (Pal)partida.trumfo
						&& ma[i].pal == (Pal)partida.trumfo)
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

		basa.cartes[basa.num_carta].pal = ma[r].pal;
		basa.cartes[basa.num_carta].valor = ma[r].valor;

		{ // Treu la jugada
			ma[r].pal = ma[ma_size - 1].pal;
			ma[r].valor = ma[ma_size - 1].valor;
			ma_size--;
		}

		Carta jugada;
		jugada.pal = basa.cartes[basa.num_carta].pal;
		jugada.valor = basa.cartes[basa.num_carta].valor;


		// Update la mes guanyadora
		if (basa.num_carta == 0)
		{
			basa.guanyadora.pal = basa.cartes[basa.num_carta].pal;
			basa.guanyadora.valor = basa.cartes[basa.num_carta].valor;
			basa.va_guanyant = partida.players[player].equip;
		}
		else if (basa.guanyadora.pal == basa.cartes[basa.num_carta].pal
			&&	basa.guanyadora.valor < basa.cartes[basa.num_carta].valor)
		{
			basa.guanyadora.valor = basa.cartes[basa.num_carta].valor;

			if (basa.va_guanyant != partida.players[player].equip)
			{
				basa.va_guanyant = partida.players[player].equip;
			}

		}
		else if (partida.trumfo != (TrumfoCantat::BUTI)
			&& basa.guanyadora.pal == (Pal)partida.trumfo
			&& basa.cartes[basa.num_carta].pal == (Pal)partida.trumfo
			&& basa.guanyadora.valor < basa.cartes[basa.num_carta].valor)
		{
			basa.guanyadora.valor = basa.cartes[basa.num_carta].valor;

			if (basa.va_guanyant != partida.players[player].equip)
			{
				basa.va_guanyant = partida.players[player].equip;
			}
		}
		else if (partida.trumfo != (TrumfoCantat::BUTI)
			&& basa.guanyadora.pal != (Pal)partida.trumfo
			&& basa.cartes[basa.num_carta].pal == (Pal)partida.trumfo)
		{
			basa.guanyadora.pal = basa.cartes[basa.num_carta].pal;
			basa.guanyadora.valor = basa.cartes[basa.num_carta].valor;

			if (basa.va_guanyant != partida.players[player].equip)
			{
				basa.va_guanyant = partida.players[player].equip;
			}
		}

		return jugada;
	}
};









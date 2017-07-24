#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

#include "VeryData.h"
#include "DeckUtils.h"
#include "Player.h"

#include "FunPlayer.h"

void EscriuCarta(Carta carta)
{
	int valor = (int)(carta.valor);
	if (valor >= Valor::SOTA && valor <= Valor::REI) valor = valor + 1;
	else if (valor == Valor::AS) valor = 1;
	else if (valor == Valor::MANILLA) valor = 9;
	cout << "[" << valor << "." << (char)(carta.pal) << "] ";
}

void EscriuMa(Carta* ma, int ma_size, int player)
{
	cout << "#" << player << ": ";
	for (int i = 0; i < ma_size; ++i)
	{
		EscriuCarta(ma[i]);
	}
	cout << endl;
}

void EscriuDeck(Baralla& deck)
{
	cout << "Deck:" << endl;
	for (int i = 0; i < 48; ++i)
	{
		EscriuCarta(deck[i]);
		if ((i+1) % 12 == 0) cout << endl;
	}
	cout << endl;
}

void JugaPartida(InfoPartida& partida)
{
	partida.punts[0] = 0;
	partida.punts[1] = 0;

	// Canta trumfo
	TrumfoCantat trumfo = partida.players[partida.player_que_canta].IA->CantaTrumfo();
	if (trumfo == TrumfoCantat::DELEGO)
	{
		trumfo = partida.players[partida.player_que_canta].IA->CantaTrumfoDelegat();
	}
	partida.trumfo = trumfo;

	// Contrades
	partida.contrades = 1;
	{
		if (partida.players[(partida.player_que_canta + 1) % 4].IA->DecidirContrar()
			|| partida.players[(partida.player_que_canta + 3) % 4].IA->DecidirContrar())
		{
			partida.contrades++; //Contro
			if (partida.players[(partida.player_que_canta + 0) % 4].IA->DecidirContrar()
				|| partida.players[(partida.player_que_canta + 2) % 4].IA->DecidirContrar())
			{
				partida.contrades++; //Recontro
				if (partida.players[(partida.player_que_canta + 1) % 4].IA->DecidirContrar()
					|| partida.players[(partida.player_que_canta + 3) % 4].IA->DecidirContrar())
				{
					partida.contrades++; //St. Vicenç
				}
			}
		}
	}

	// Les 12 bases
	for (int i = 0; i < 12; ++i)
	{
		InfoBasa basa;
		cout << "-- Basa " << i+1 << endl;
		cout << endl << "Trumfo: " << (char)partida.trumfo << endl;

		cout << endl;
		for (int p = 0; p < 4; ++p)
		{
			EscriuMa(partida.players[p].ma, partida.players[p].ma_size, p);
		}
		cout << endl;

		for (int nb = 0; nb < 4; ++nb)
		{
			basa.num_carta = nb;
			int player = (i + nb + partida.player_que_canta) % 4;
			Carta jugada = partida.players[partida.player_que_canta].IA->JugaCarta(partida, player, basa);

			cout << "#" << player << " Juga: ";
			EscriuCarta(jugada);
			cout << endl;
		}
		
		cout << "Guanya: ";
		EscriuCarta(basa.guanyadora);
		cout << endl;

		int punts = 1;
		for (int i = 0; i < 4; ++i)
		{
			if (basa.guanyadora.valor >= 9)
			{
				punts += basa.guanyadora.valor - 9;
			}
		}
		partida.punts[basa.va_guanyant] += punts;

		cout << endl;
	}

	cout << "Punts de partida:" << endl;
	cout << "A - " << partida.punts[0] << endl;
	cout << "B - " << partida.punts[1] << endl;
	cout << endl;
}


void JugaUnaButiSencera()
{
	int total_punts[2];
	total_punts[0] = 0;
	total_punts[1] = 0;

	InfoPartida partida;
	partida.players[0].equip = partida.players[2].equip = Equip::A;
	partida.players[1].equip = partida.players[3].equip = Equip::B;
	for (int i = 0; i < 4; ++i)
	{
		partida.players[i].IA = new FunPlayer();
	}

	partida.player_que_canta = 0;

	while (total_punts[0] < 101 && total_punts[1] < 101)
	{
		Baralla deck;

		DeckUtils::Remena(deck);
		DeckUtils::Talla(deck);
		EscriuDeck(deck);
		
		DeckUtils::Reparteix(partida.players, deck);

		JugaPartida(partida);
		partida.player_que_canta = (partida.player_que_canta + 1) % 4;

		cout << "-- Recompte" << endl;
		if (partida.punts[0] == partida.punts[1])
		{
			cout << "Putes!" << endl;
		}
		else if (partida.punts[0] > partida.punts[1])
		{
			int guany = (partida.punts[0] - 36)*partida.contrades;
			cout << "Guanya A: +" << guany << " [" << "x" << partida.contrades << "]" << endl;
			total_punts[0] += guany;
		}
		else
		{
			int guany = (partida.punts[1] - 36)*partida.contrades;
			cout << "Guanya B: +" << guany << " [" << "x" << partida.contrades << "]" << endl;
			total_punts[1] += guany;
		}

		cout << endl;

		cout << "Total:" << endl;
		cout << "A - " << total_punts[0] << endl;
		cout << "B - " << total_punts[1] << endl;
		cout << endl;
	}
}

int main()
{
	DeckUtils::init();

	JugaUnaButiSencera();
}
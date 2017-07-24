#include <vector>
using namespace std;

#include "VeryData.h"
#include "Logger.h"
#include "DeckUtils.h"
#include "Player.h"

//IAs
//---------------------------
#include "FunPlayer.h"

//#include "StubPlayer.h"

//---------------------------

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

		Logger::ComencaBasa(i, partida);

		for (int nb = 0; nb < 4; ++nb)
		{
			basa.num_carta = nb;
			int player = (i + nb + partida.player_que_canta) % 4;
			Carta jugada = partida.players[partida.player_que_canta].IA->JugaCarta(partida, player, basa);

			Logger::EscriuJugada(player, jugada);
		}
		
		Logger::EscriuWinningCard(basa.guanyadora);

		int punts = 1;
		for (int i = 0; i < 4; ++i)
		{
			if (basa.cartes[i].valor >= Valor::SOTA)
			{
				punts += basa.cartes[i].valor - (int)Valor::SOTA + 1;
			}
		}
		partida.punts[basa.va_guanyant-'A'] += punts;

	}

	Logger::PuntsDePartida(partida);
	
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
		Logger::EscriuDeck(deck);
		
		DeckUtils::Reparteix(partida.players, deck);

		JugaPartida(partida);
		partida.player_que_canta = (partida.player_que_canta + 1) % 4;

		if (partida.punts[0] == partida.punts[1])
		{
			Logger::ResultatPutes();
		}
		else if (partida.punts[0] > partida.punts[1])
		{
			int guany = (partida.punts[0] - 36)*partida.contrades;
			total_punts[0] += guany;
			Logger::ResultatGuanya(Equip::A, guany, partida.contrades);
		}
		else
		{
			int guany = (partida.punts[1] - 36)*partida.contrades;
			total_punts[1] += guany;
			Logger::ResultatGuanya(Equip::B, guany, partida.contrades);
		}

		Logger::MarcadorTotal(total_punts[0], total_punts[1]);
		
	}
}

int main()
{
	DeckUtils::init();
	JugaUnaButiSencera();
}
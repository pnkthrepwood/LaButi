#pragma once

#include "VeryData.h"
#include <iostream>
using namespace std;

namespace Logger
{


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
			if ((i + 1) % 12 == 0) cout << endl;
		}
		cout << endl;
	}

	void ComencaBasa(int num_basa, InfoPartida& partida)
	{
		cout << "-- Basa " << num_basa + 1 << endl;
		cout << endl << "Trumfo: " << (char)partida.trumfo << endl;
		cout << endl;
		for (int p = 0; p < 4; ++p)
		{
			Logger::EscriuMa(partida.players[p].ma, partida.players[p].ma_size, p);
		}
		cout << endl;
	}

	void EscriuJugada(int player, Carta jugada)
	{
		cout << "#" << player << " Juga: ";
		Logger::EscriuCarta(jugada);
		cout << endl;
	}

	void EscriuWinningCard(Carta crack)
	{
		cout << "Guanya: ";
		Logger::EscriuCarta(crack);
		cout << endl;
	}

	void PuntsDePartida(InfoPartida& partida)
	{
		cout << "Punts de partida:" << endl;
		cout << "A - " << partida.punts[0] << endl;
		cout << "B - " << partida.punts[1] << endl;
		cout << endl;
	}

	void AnunciaRecompte()
	{
		cout << "-- Recompte" << endl;
	}

	void ResultatPutes()
	{
		AnunciaRecompte();
		cout << "PUTES!" << endl;
	}

	void ResultatGuanya(Equip equip, int guany, int contrades)
	{
		cout << "Guanya Equip " << (char)equip << ": +" << guany << " [" << "x" << contrades << "]" << endl;
	}
	
	void MarcadorTotal(int punts_a, int punts_b)
	{
		cout << endl;
		cout << "Total:" << endl;
		cout << "A - " << punts_a << endl;
		cout << "B - " << punts_b << endl;
		cout << endl;
	}
}
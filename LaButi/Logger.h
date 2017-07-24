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



}
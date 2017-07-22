#include <iostream>
#include <ctime>
#include <vector>
using namespace std;

enum Pal
{
	BASTOS = 'B',
	COPES = 'C',
	ESPASES = 'E',
	OROS = 'O'
};
enum Valor
{
	DOS = 2,
	TRES = 3,
	QUATRE = 4,
	CINC = 5,
	SIS = 6,
	SET = 7,
	VUIT = 8,

	SOTA = 9,
	CAVALL = 10, //Sortida de cavall, sortida d'animal
	REI = 11,
	AS = 12,
	MANILLA = 13
};
struct Carta
{
	Valor valor;
	Pal pal;
};
typedef Carta Baralla[12 * 4];

//Deck magic platonic
Baralla deck_ordenat;

void OrdenaBaralla(Baralla& deck)
{
	char* pal_char = "BCEO";
	for (int i = 0; i < 12 * 4; ++i)
	{
		deck[i].valor = (Valor)(2+((i % 12)));
		deck[i].pal = (Pal)(pal_char[(i / 12)]);
	}
}

void Remena(Baralla& deck)
{
	bool comptats[48];
	for (int i = 0; i < 48; ++i) comptats[i] = false;

	int deck_next = 0;
	while (deck_next < 48)
	{
		int rnd = rand() % 48;
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
	for (int i = 0; i < 48-n; ++i)
	{
		deck[i].valor = deck[i + n].valor;
		deck[i].pal = deck[i + n].pal;
	}
	for (int i = 0; i < n; ++i)
	{
		deck[i+48-n].valor = aux[i].valor;
		deck[i+48-n].pal = aux[i].pal;
	}
}

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

enum Equip
{
	A, B
};

struct Jugador
{
	Carta ma[12];
	int ma_size = 0;
	Equip equip;
};

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
				players[j].ma[i*4 + k].valor = deck[deck_counter].valor;
				players[j].ma[i*4 + k].pal = deck[deck_counter].pal;
				deck_counter++;
			}
		}
	}
}

enum class TrumfoCantat
{
	BASTOS = 'B',
	COPES = 'C',
	ESPASES = 'E',
	OROS = 'O',
	BUTI = 'X',		//Amb dos manilles i un as, buti cantaras
	DELEGO = 'D'	//Buti delegada, buti contrada
};

TrumfoCantat CantaTrumfo()
{
	int r = rand() % 6;
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

struct InfoPartida
{
	TrumfoCantat trumfo;
	Jugador players[4];
	int contrades = 0;
	int punts[2];
};

bool JugadorTePal(Pal pal, Carta* ma, int ma_size)
{
	for (int i = 0; i < ma_size; ++i)
	{
		if (ma[i].pal == pal)
		{
			return true;
		}
	}

	return false;
}

struct InfoBasa
{
	Carta cartes[4];
	Carta guanyadora;
	Equip va_guanyant;
	int num_carta;
};

Carta JugaCartaSudantDelCompany(InfoPartida& partida, int player, InfoBasa& basa)
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
				&&	basa.guanyadora.pal == pal_basa
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
	int r = rand() % ma_size;
	while (!jugables[r])
	{
		r = rand() % ma_size;
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

void JugaPartida(InfoPartida& partida, int player_que_ha_cantat)
{

	partida.punts[0] = 0;
	partida.punts[1] = 0;

	// Canta trumfo
	TrumfoCantat trumfo = CantaTrumfo();
	if (trumfo == TrumfoCantat::DELEGO)
	{
		trumfo = CantaTrumfoDelegat();
	}
	partida.trumfo = trumfo;

	// Contrades
	partida.contrades = 1;
	if (rand() % 100 < 15)
	{
		partida.contrades++; //Contro

		if (rand() % 100 < 10)
		{
			partida.contrades++; //Recontro

			if (rand() % 100 < 5)
			{
				partida.contrades++; //St. Vicenç
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
			int player = (i + nb + player_que_ha_cantat) % 4;
			Carta jugada = JugaCartaSudantDelCompany(partida, player, basa);

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


int main()
{
	srand(time(NULL));
	OrdenaBaralla(deck_ordenat);

	int total_punts[2];
	total_punts[0] = 0;
	total_punts[1] = 0;

	int player_que_ha_cantat = 0;

	while (total_punts[0] < 101 && total_punts[1] < 101)
	{
		Baralla deck;

		Remena(deck);
		Talla(deck);
		EscriuDeck(deck);

		InfoPartida partida;

		partida.players[0].equip = partida.players[2].equip = Equip::A;
		partida.players[1].equip = partida.players[3].equip = Equip::B;

		Reparteix(partida.players, deck);

		player_que_ha_cantat++;

		JugaPartida(partida, player_que_ha_cantat);

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
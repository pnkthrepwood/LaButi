#pragma once


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

enum Equip
{
	A, B
};

enum class TrumfoCantat
{
	BASTOS = 'B',
	COPES = 'C',
	ESPASES = 'E',
	OROS = 'O',
	BUTI = 'X',		//Amb dos manilles i un as, buti cantaras
	DELEGO = 'D'	//Buti delegada, buti contrada
};

struct IAPlayer;
struct Jugador
{
	Carta ma[12];
	int ma_size = 0;

	Equip equip;
	IAPlayer* IA;
};

struct InfoPartida
{
	int player_que_canta = 0;
	TrumfoCantat trumfo;
	Jugador players[4];
	int contrades = 0;
	int punts[2];
};

struct InfoBasa
{
	Carta cartes[4];
	Carta guanyadora;
	Equip va_guanyant;
	int num_carta;
};

//Deck magic platonic
Baralla deck_ordenat;

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

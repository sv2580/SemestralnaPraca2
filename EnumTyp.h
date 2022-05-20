#pragma once
#include <cstdlib>

enum class TypUzemnejJednotky {
	Stat,
	Kraj,
	Okres,
	Obec
};

enum class TypVzdelania {
	BezDo15,
	BezNad15,
	Zakladne,
	StredneOdborne,
	UplneStredne,
	VyssieOdborne,
	Vysokoskolske,
	Nezistene,
	Null
};

enum class Pohlavie {
	Muz,
	Zena,
	Obe
};

enum class VekovaSkupina {
	Predproduktivni,
	Produktivni,
	Poproduktivni,

};
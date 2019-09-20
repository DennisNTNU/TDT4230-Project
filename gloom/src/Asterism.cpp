#include "Asterism.h"

Asterism::Asterism()
{
	inited = false;
}

Asterism::~Asterism()
{
}

void Asterism::init()
{
	if (!inited)
	{

		initOrion();
		initUrsaMajor();
		initCygnus();
		initUrsaMinor();
		initAuriga();
		initTaurus();
		initGemini();
		initCassiopeia();
		initPerseus();
		initCepheus();
		initLyra();
		initAndromeda();
		initPegasus();
		initLeo();
		initBooetes();
		initHercules();
		initDraco();

		inited = true;
	}
}

void Asterism::initOrion()
{
	// Upper left Arm
	edges.push_back(Edge(28716, 27913));
	edges.push_back(Edge(28716, 29426));
	edges.push_back(Edge(29038, 27913));
	edges.push_back(Edge(29426, 28614));
	//edges.push_back(Edge(28614, 29038));
	edges.push_back(Edge(29426, 29038));
	edges.push_back(Edge(28614, 27989));
	// Head and neck
	edges.push_back(Edge(27989, 26207));
	edges.push_back(Edge(26207, 25336));
	edges.push_back(Edge(27989, 25336));
	//Torso and legs
	edges.push_back(Edge(27989, 26727));
	edges.push_back(Edge(26727, 27366));
	edges.push_back(Edge(26727, 26311));
	edges.push_back(Edge(25930, 26311));
	edges.push_back(Edge(25930, 25281));
	edges.push_back(Edge(25281, 24436));
	edges.push_back(Edge(25930, 25336));
	//Left Arm
	edges.push_back(Edge(25336, 22449));
	edges.push_back(Edge(22449, 22549));
	edges.push_back(Edge(22549, 22797));
	edges.push_back(Edge(22797, 23123));
	edges.push_back(Edge(22449, 22509));
	edges.push_back(Edge(22509, 22845));
	edges.push_back(Edge(22845, 22957));
}

void Asterism::initUrsaMajor()
{
	edges.push_back(Edge(67301, 65378));
	edges.push_back(Edge(65378, 62956));
	edges.push_back(Edge(62956, 59774));
	edges.push_back(Edge(59774, 58001));
	edges.push_back(Edge(58001, 53910));
	edges.push_back(Edge(53910, 54061));
	edges.push_back(Edge(54061, 59774));

	edges.push_back(Edge(58001, 57399));
	edges.push_back(Edge(57399, 54539));
	edges.push_back(Edge(57399, 55219));
	edges.push_back(Edge(54539, 50801));
	edges.push_back(Edge(50801, 50372));
	edges.push_back(Edge(53910, 48319));
	edges.push_back(Edge(54061, 46733));
	edges.push_back(Edge(46733, 48319));
	edges.push_back(Edge(46733, 41704));
	edges.push_back(Edge(41704, 48319));
	edges.push_back(Edge(48319, 46853));
	edges.push_back(Edge(46853, 44471));
	edges.push_back(Edge(44471, 44127));
}

void Asterism::initCygnus()
{
	edges.push_back(Edge(104732, 102488));
	edges.push_back(Edge(102488, 100453));
	edges.push_back(Edge(100453, 102098));
	edges.push_back(Edge(100453, 97165));
	edges.push_back(Edge(100453, 98110));
	edges.push_back(Edge(97165, 95853));
	edges.push_back(Edge(98110, 95947));
}

void Asterism::initUrsaMinor()
{

	edges.push_back(Edge(11767, 85822));
	edges.push_back(Edge(85822, 82080));
	edges.push_back(Edge(82080, 77055));
	edges.push_back(Edge(77055, 79822));
	edges.push_back(Edge(79822, 75097));
	edges.push_back(Edge(75097, 72607));
	edges.push_back(Edge(72607, 77055));
}

void Asterism::initAuriga()
{

	edges.push_back(Edge(24608, 28360));
	edges.push_back(Edge(28360, 28380));
	edges.push_back(Edge(28380, 25428));
	edges.push_back(Edge(25428, 23015));
	edges.push_back(Edge(23015, 24608));
}

void Asterism::initTaurus()
{
	edges.push_back(Edge(25428, 20889));
	edges.push_back(Edge(20889, 20455));
	edges.push_back(Edge(20455, 20205));
	edges.push_back(Edge(20205, 20885));
	edges.push_back(Edge(20885, 21421));
	edges.push_back(Edge(21421, 26451));
	edges.push_back(Edge(20205, 18724));
	edges.push_back(Edge(18724, 16083));
}

void Asterism::initGemini()
{
	edges.push_back(Edge(32362, 35350));
	edges.push_back(Edge(35350, 35550));
	edges.push_back(Edge(35550, 34088));
	edges.push_back(Edge(34088, 31681));
	edges.push_back(Edge(35550, 36962));
	edges.push_back(Edge(36962, 37740));
	edges.push_back(Edge(36962, 37826));
	edges.push_back(Edge(36962, 36046));
	edges.push_back(Edge(36046, 34693));
	edges.push_back(Edge(34693, 36850));
	edges.push_back(Edge(34693, 33018));
	edges.push_back(Edge(34693, 32246));
	edges.push_back(Edge(32246, 30883));
	edges.push_back(Edge(32246, 30343));
	edges.push_back(Edge(30343, 29655));
}

void Asterism::initCassiopeia()
{
	edges.push_back(Edge(8886, 6686));
	edges.push_back(Edge(6686, 4427));
	edges.push_back(Edge(4427, 3179));
	edges.push_back(Edge(3179, 746));

}

void Asterism::initPerseus()
{
	edges.push_back(Edge(17448, 18246));
	edges.push_back(Edge(18246, 18614));
	edges.push_back(Edge(18614, 18532));
	edges.push_back(Edge(18532, 14576));
	edges.push_back(Edge(18532, 17358));
	edges.push_back(Edge(14576, 14354));
	edges.push_back(Edge(17358, 15863));
	edges.push_back(Edge(14576, 14668));
	edges.push_back(Edge(14668, 14632));
	edges.push_back(Edge(14632, 15863));
	edges.push_back(Edge(15863, 14328));
	edges.push_back(Edge(14328, 13268));
	edges.push_back(Edge(14328, 13531));
	edges.push_back(Edge(13531, 13268));
	edges.push_back(Edge(13531, 14632));
}

void Asterism::initCepheus()
{
	
	edges.push_back(Edge(116727, 112724));
	edges.push_back(Edge(112724, 106032));
	edges.push_back(Edge(112724, 110991));
	edges.push_back(Edge(110991, 109492));
	edges.push_back(Edge(109492, 105199));
	edges.push_back(Edge(105199, 102422));
	edges.push_back(Edge(105199, 106032));
	edges.push_back(Edge(106032, 116727));
}

void Asterism::initLyra()
{
	edges.push_back(Edge(93194, 92420));
	edges.push_back(Edge(92420, 91971));
	edges.push_back(Edge(91971, 92791));
	edges.push_back(Edge(92791, 93194));
	edges.push_back(Edge(91971, 91262));
	edges.push_back(Edge(91971, 91926));
	edges.push_back(Edge(91262, 91926));
}

void Asterism::initAndromeda()
{
	edges.push_back(Edge(677, 3092));
	edges.push_back(Edge(3092, 5447));
	edges.push_back(Edge(5447, 9640));
	edges.push_back(Edge(677, 2912));
	edges.push_back(Edge(2912, 4436));
	edges.push_back(Edge(4436, 6411));
	//edges.push_back(Edge(, ));
	//edges.push_back(Edge(, ));
}

void Asterism::initPegasus()
{
	edges.push_back(Edge(677, 1067));
	edges.push_back(Edge(677, 113881));
	edges.push_back(Edge(113881, 112158));
	edges.push_back(Edge(112158, 109410));
	edges.push_back(Edge(113881, 112440));
	edges.push_back(Edge(112440, 107354));
	edges.push_back(Edge(113881, 113963));
	edges.push_back(Edge(113963, 1067));
	edges.push_back(Edge(113963, 112029));
	edges.push_back(Edge(112029, 109427));
	edges.push_back(Edge(109427, 107315));
}

void Asterism::initLeo()
{
	edges.push_back(Edge(57632, 54879));
	edges.push_back(Edge(54879, 54872));
	edges.push_back(Edge(54872, 57632));
	edges.push_back(Edge(54872, 50583));
	edges.push_back(Edge(54879, 49583));
	edges.push_back(Edge(49583, 49669));
	edges.push_back(Edge(49583, 50583));
	edges.push_back(Edge(50583, 50335));
	edges.push_back(Edge(50335, 48455));
	edges.push_back(Edge(48455, 47908));
	edges.push_back(Edge(54879, 55642));
	edges.push_back(Edge(55642, 55434));
}

void Asterism::initBooetes()
{
	edges.push_back(Edge(71795, 69673));
	edges.push_back(Edge(69673, 67927));
	edges.push_back(Edge(69673, 72105));
	edges.push_back(Edge(72105, 74666));
	edges.push_back(Edge(74666, 73555));
	edges.push_back(Edge(73555, 71075));
	edges.push_back(Edge(71075, 69732));
	edges.push_back(Edge(69732, 69483));
	edges.push_back(Edge(69483, 70497));
	edges.push_back(Edge(70497, 69732));
	edges.push_back(Edge(71075, 71053));
	edges.push_back(Edge(71053, 69673));
}

void Asterism::initHercules()
{
	
	edges.push_back(Edge(86414, 87808));
	edges.push_back(Edge(87808, 84380));
	edges.push_back(Edge(84380, 81833));
	edges.push_back(Edge(81833, 81126));
	edges.push_back(Edge(81126, 79992));
	edges.push_back(Edge(79992, 79101));
	edges.push_back(Edge(81833, 81693));
	edges.push_back(Edge(81693, 80816));
	edges.push_back(Edge(81693, 83207));
	edges.push_back(Edge(80816, 80170));
	edges.push_back(Edge(83207, 84379));
	edges.push_back(Edge(84379, 85693));
	edges.push_back(Edge(84379, 84345));
	edges.push_back(Edge(85693, 86974));
	edges.push_back(Edge(83207, 84380));

}

void Asterism::initDraco()
{
	
	edges.push_back(Edge(56211, 61281));
	edges.push_back(Edge(61281, 68756));
	edges.push_back(Edge(68756, 75458));
	edges.push_back(Edge(75458, 78527));
	edges.push_back(Edge(78527, 80331));
	edges.push_back(Edge(80331, 83895));
	edges.push_back(Edge(83895, 89908));
	edges.push_back(Edge(89908, 89937));
	edges.push_back(Edge(89908, 94376));
	edges.push_back(Edge(94376, 97433));
	edges.push_back(Edge(94376, 87585));
	edges.push_back(Edge(87585, 85819));
	edges.push_back(Edge(87585, 87833));
	edges.push_back(Edge(87833, 85670));
	edges.push_back(Edge(85670, 85819));
}


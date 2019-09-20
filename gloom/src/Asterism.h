#pragma once

#include <vector>

class Edge {
public:
	Edge(int star1_, int star2_) {
		star1 = star1_;
		star2 = star2_;
	}
	~Edge() {}

	int star1;
	int star2;
};

class Asterism {
public:
	Asterism();
	~Asterism();
	std::vector<Edge> edges;
	void init();

private:
	bool inited;

	void initOrion();
	void initUrsaMajor();
	void initCygnus();
	void initUrsaMinor();
	void initAuriga();
	void initTaurus();
	void initGemini();
	void initCassiopeia();
	void initPerseus();
	void initCepheus();
	void initLyra();
	void initAndromeda();
	void initPegasus();
	void initLeo();
	void initBooetes();
	void initHercules();
	void initDraco();

	void initLeoMinor();

};

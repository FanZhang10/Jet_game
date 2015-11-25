#ifndef _JET_H
#define _JET_H


class Jet{

public:
	Jet();
	float normal[3];

	void rendering();
	void ReduceToUnit(float vector[3]);
	void calcNormal(float v[3][3], float out[3]);

};

#endif
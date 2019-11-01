#pragma once
#include <ctime>
class Probability
{
public:
	double eventA;
	double eventB;
	double intersectionAB;
	double conditionalProbability;

	Probability();
	Probability(double A, double B, double AB);
	Probability(double A, double B, double AB, double conditional);
	~Probability()=default;


};


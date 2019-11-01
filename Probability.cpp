#include "Probability.h"

Probability::Probability(double A, double B, double AB) {
	eventA = A;
	eventB = B;
	intersectionAB = AB;
	conditionalProbability = AB / B;
}

Probability::Probability(double A, double B, double AB, double conditional) {
	eventA = A;
	eventB = B;
	intersectionAB = AB;
	conditionalProbability = conditional;
}

Probability::Probability() {
	eventA = 0;
	eventB = 0;
	intersectionAB = 0;
	conditionalProbability = 0;
}



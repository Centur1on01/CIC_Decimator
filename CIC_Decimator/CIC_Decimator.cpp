
#define _USE_MATH_DEFINES


#include <iostream>
#include <vector>
#include <deque>
#include <cmath>
#include <fstream>
#include <complex>
#include "BigInt.hpp"

using namespace std;



BigInt sum_2(const BigInt& a, const BigInt& b, bool& overflow, const int& type, const BigInt& sizePos, const BigInt& sizeNeg) {
	overflow = 0;
	BigInt result = a + b;
	if (result > sizePos) {
		overflow = 1;
		cout << "overflowPos" << endl;
	}
	if (result < sizeNeg) {
		overflow = 1;
		cout << "overflowNeg" << endl;
	}
	return result;
}



class CICFilter {

	int inputType, sectionType, decimationCoeff, sectionNum, diffDelay, it;
	bool overflow;
	deque<BigInt> delayIntegratorBuff, delayCombBuff, sumIntegratorBuff, sumCombBuff;
	BigInt sizeSectionPos, sizeSectionNeg;

public:

	CICFilter(int sectionNum = 1, int diffDelay = 2, int decimationCoeff = 1, int inputType = 8, int sectionType = 32) :
		sectionNum(sectionNum), diffDelay(diffDelay), decimationCoeff(decimationCoeff), inputType(inputType), sectionType(sectionType) {

		if (sectionType == 0) {
			this->sectionType = ceil((double)sectionNum * log2(decimationCoeff * diffDelay) + inputType);
			cout << "Auto sectionType is: " << this->sectionType << endl;
		}

		sizeSectionPos = pow(2LL, this->sectionType - 1) - 1LL;
		sizeSectionNeg = -pow(2LL, this->sectionType - 1);

		sumIntegratorBuff.resize(sectionNum + 1, 0);
		sumCombBuff.resize(sectionNum + 1, 0);
		delayIntegratorBuff.resize(sectionNum, 0);
		delayCombBuff.resize(diffDelay * sectionNum, 0);

		overflow = 0;
		it = 0;
	}


	BigInt next(const BigInt& signal) {

		sumIntegratorBuff[0] = signal;
		// Integrator
		for (int i = 0; i < sectionNum; ++i) {
			sumIntegratorBuff[i + 1] = sum_2(delayIntegratorBuff[i], sumIntegratorBuff[i], overflow, sectionType, sizeSectionPos, sizeSectionNeg);
			delayIntegratorBuff[i] = sumIntegratorBuff[i + 1];
		}
		sumCombBuff[0] = delayIntegratorBuff[sectionNum - 1];
		
		// Decimation
		if (it % decimationCoeff == 0) {
			// Comb
			for (int i = 0; i < sectionNum; ++i) {
				BigInt temp = delayCombBuff[diffDelay - 1 + i * diffDelay];
				delayCombBuff.erase(delayCombBuff.begin() + diffDelay - 1 + i * diffDelay);
				delayCombBuff.insert(delayCombBuff.begin() + i * diffDelay, sumCombBuff[i]);
				sumCombBuff[i + 1] = sum_2(sumCombBuff[i], -temp, overflow, sectionType, sizeSectionPos, sizeSectionNeg);
			}
		}

		++it;
		return sumCombBuff[sectionNum];
	}


};



int main()
{
	ofstream out("out.txt");

	int N = 64;
	int sectionNum = 4, diffDelay = 20, decimationCoeff = 1, inputType = 64, sectionType = 0;
	double gainCIC = pow(decimationCoeff * diffDelay, sectionNum);
	BigInt signal;
	BigInt result;
	CICFilter CIC(sectionNum, diffDelay, decimationCoeff, inputType, sectionType);

	for (int i = 0; i < N; ++i) {
		//signal = (long long)(round(127. * cos(2 * M_PI * 500. / 8000. * i))); // 2011111147483646. // 2147483648.
		signal = 9223372036854775807;
		result = CIC.next(signal);
		out << signal << "\t";
		if (i % decimationCoeff == 0) {
			//out << (double)result.to_long_long() / gainCIC << endl;
			out << result.to_string() << endl;
		}
		else {
			out << "0" << endl;
		}
	}



	out.close();
	system("pause");
}

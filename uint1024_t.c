#include <stdio.h>

typedef struct uint1024_t {
	long long digits[35];
} uint1024_t;

uint1024_t subtr_op(uint1024_t x, uint1024_t y) {
	uint1024_t result;
	uint1024_t x2;
	for (int i = 0; i < 35; i++) {
		x2.digits[i] = 0;
	}

	for (int i = 0; i < 34; i++) {
		if (x.digits[i] - y.digits[i] + x2.digits[i] < 0) {
			result.digits[i] = x.digits[i] - y.digits[i] + x2.digits[i] + 1000000000;
			x2.digits[i + 1] -= 1;
		}
		else {
			result.digits[i] = x2.digits[34] + x.digits[i] - y.digits[i];
		}
	}

	if (x2.digits[34] + x.digits[34] - y.digits[34] < 0) {
		result.digits[0] = -1;
		for (int i = 1; i < 35; i++) {
			result.digits[i] = 0;
		}
		return result;
	}
	else {
		result.digits[34] = x2.digits[34] + x.digits[34] - y.digits[34];
		return result;
	}

}

int behavior(uint1024_t y) {
	uint1024_t x = { 224137215, 356329624, 716304835, 245938479, 298239947, 510684586, 237163350, 110540827, 881473913, 682342462, 768838150, 952085005, 119453082, 601246094, 276302219, 424865485, 377767893, 639474124, 492847430, 814416622, 658789768, 871393357, 120113879, 407536021, 708477322, 500963132, 732675805, 430081157, 230657273, 797697894, 902473361, 930519078, 231590772, 769313486, 179 };

	uint1024_t result;
	uint1024_t x2;

	for (int i = 0; i < 35; i++) {
		x2.digits[i] = 0;
	}

	for (int i = 0; i < 34; i++) {
		if (x.digits[i] - y.digits[i] + x2.digits[i] < 0) {
			result.digits[i] = x.digits[i] - y.digits[i] + x2.digits[i] + 1000000000;
			x2.digits[i + 1] -= 1;
		}
		else {
			result.digits[i] = x2.digits[34] + x.digits[i] - y.digits[i];
		}
	}

	if (x2.digits[34] + x.digits[34] - y.digits[34] < 0) {
		return 0;
	}
	else {
		return 1;
	}
	
} 

//{ 224137215, 356329624, 716304835, 245938479, 298239947, 510684586, 237163350, 110540827, 881473913, 682342462, 768838150, 952085005, 119453082, 601246094, 276302219, 424865485, 377767893, 639474124, 492847430, 814416622, 658789768, 871393357, 120113879, 407536021, 708477322, 500963132, 732675805, 430081157, 230657273, 797697894, 902473361, 930519078, 231590772, 769313486, 179 }
uint1024_t from_unit(unsigned int x) {
	uint1024_t bigint;
	for (int i = 0; i < 35; i++) {
		bigint.digits[i] = x % 1000000000;
		x /= 1000000000;
	}

	return bigint;
}

uint1024_t add_op(uint1024_t x, uint1024_t y) {
	uint1024_t result;
	long long carry = 0;
	for (int i = 0; i < 35; i++) {
		result.digits[i] = (x.digits[i] + y.digits[i] + carry) % 1000000000;
		carry = (x.digits[i] + y.digits[i] + carry) / 1000000000;
	}
	if (carry > 1) {
		result.digits[0] = -1;
		for (int i = 1; i < 34; i++) {
			result.digits[i] = 0;
		}
		return result;
	}
	else {
		if (behavior(result) == 0) {
			return result;
		}
		else {
			result.digits[0] = -1;
			for (int i = 1; i < 34; i++) {
				result.digits[i] = 0;
			}
			return result;
		}
	}

}

uint1024_t mult_op(uint1024_t x, uint1024_t y) {
	uint1024_t result;

	for (int i = 0; i < 35; i++) {
		result.digits[i] = 0;
	}

	for(int i = 0; i < 35; i++){
		for (int j = 0; j < 35 - i; j++) {
			result.digits[i + j] += x.digits[i] * y.digits[j];
		}
	}

	for (int i = 0; i < 34; i++) {
		result.digits[i + 1] += result.digits[i] / 1000000000;
		result.digits[i] %= 1000000000;
	}

	if (behavior(result) == 0) {
		result.digits[0] = -1;
		for (int i = 1; i < 34; i++) {
			result.digits[i] = 0;
		}
		return result;
	}

	return result;
}

void printf_value(uint1024_t x) {
	int flag = 1;
	for (int i = 34; i >= 0; i--) {

		if (x.digits[i] != 0) {
			flag = 0;
		}
		if (flag == 0)
			printf("%lld",x.digits[i]);
	}
	if (flag == 1)
		printf("0");
}
	

int main() {
	uint1024_t biggestNumber = { 224137215, 356329624, 716304835, 245938479, 298239947, 510684586, 237163350, 110540827, 881473913, 682342462, 768838150, 952085005, 119453082, 601246094, 276302219, 424865485, 377767893, 639474124, 492847430, 814416622, 658789768, 871393357, 120113879, 407536021, 708477322, 500963132, 732675805, 430081157, 230657273, 797697894, 902473361, 930519078, 231590772, 769313486, 179 };
	uint1024_t a = { 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	uint1024_t x = from_unit(500);
	uint1024_t y = from_unit(-1);

	uint1024_t res = mult_op(x, y);
	printf_value(res);

	return 0;
}
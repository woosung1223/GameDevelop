#include "BouncingBall.h"
#include <vector>
#include <thread>
int main(void)
{
	std::cout << std::thread::hardware_concurrency();
	using namespace jm;
	BouncingBall().run();
	return 0;
}

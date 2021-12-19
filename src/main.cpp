#include <iostream>

#include "interval_map.h"

int main()
{
	static constexpr auto INIT = 'a';
	interval_map<int, char> im(INIT);
	std::cout << "Console representation of interval_map: int -> char\n";
	std::cout << "Initial value: " << INIT << "\n";
	std::cout << "Write \'=\' and two numbers p q and char v. Value v will be assigned to [p, q)\n";
	std::cout << "Write \'?\' and one number v. Program will print value assigned to v\n";
	std::cout << "Write 0 to exit\n";
	char op = '0';
	while (!std::cin.eof() && std::cin >> op) {
		try {
			switch (op)
			{
			case '=': {
				int p, q;
				char v;
				std::cin >> p >> q >> v;
				
				im.assign(p, q, v);
				break;
			}
			case '?': {
				int v;
				std::cin >> v;

				std::cout << im[v] << "\n";
				break;
			}
			case '0': return 0;
			default:
				throw "Undefined operation";
				break;
			}
		}
		catch (...) {
			std::cout << "Input is invalid. Please try again\n";
		}
	}
}

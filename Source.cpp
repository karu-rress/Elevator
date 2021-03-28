#include <iostream>
#include <cstdlib>
#include "elevator.h"


int main() {
	system("chcp 949");
	system("cls");
	elevator ev{ -2, 10, 1 };

	ev.print();
	ev.push_button(6, elevator::button::DOWN);
}
#include "elevator.h"

void elevator::reset(int min, int max) {
	if (min == 0)
		throw std::out_of_range("Floor can't be set to 0.");
	if (max <= 0)
		throw std::out_of_range("Max floor should be n <= N.");
	min_floor = min;
	max_floor = max;

	floors.resize(floors_count());
	for (auto& f : floors) {
		f = elevator::button::NONE;
	}
	set_floor();

	LOG("Set floor to %d to %d\n", min, max);
	LOG("Current floor is %d\n", index_to_floor(posi - std::begin(floors)));
}

size_t elevator::floor_to_index(int floor)
{
	if (min_floor > 0)
		return size_t(floor - min_floor);

	// -1 1 [2] 3
	//  0  1 [2] 3
	// floor + min_floor = 1
	// floor + min_floor -1 = 0
	// -2 -1 1 [2] 3 4 5
	// 0    1 2 [3] 4 5 6
	// 	   -3 -2 -1 1 2 3
	// 	   0    1    2 3 4 5
	// floor + min_floor = 0
	// floor + min_floor -1 = -1?

	/*
	* 	if (idx < size_t(abs(min_floor)))
		return idx + min_floor;
		return idx + min_floor + 1;
	*/
	if (floor < 0)
		return floor - min_floor; // -2, -1 => -1 - (-2) = -1 + 2 = idx 1
	return floor + abs(min_floor) - 1;
}

int elevator::index_to_floor(size_t idx) {
	// 1 2 3 4 5
	// 0 1 2 3 4
	// min == 1
	if (min_floor > 0) {
		return idx + min_floor;
	}

	// -2 -1 1 2 3 4
	// 	0    1  2 3 4 5
	// min_floor < 0
	// return idx + min_floor + ((idx < abs(min_floor)) ? 0 : 1);
	if (idx < size_t(abs(min_floor)))
		return idx + min_floor;
	return idx + min_floor + 1;
}

void elevator::move() {
	// 이거 좀 이상함
	if (dir == direction::UP) {
		for (auto iter = std::ranges::begin(floors); iter != std::ranges::end(floors); ++iter) {
			if (*iter == button::UP || *iter == button::BOTH) {
				if (std::distance(posi, std::begin(floors)) > std::distance(iter, std::begin(floors)))
					continue; // 현재 층보다 낮은 층에 있는 버튼은 무시하기

				posi = iter;
				print_floor();
				if (*iter == button::UP) {
					*iter = button::NONE;
				}
				else { // *iter == BOTH
					*iter = button::DOWN;
				}
				std::cout << "Floor " << current_floor() << ". Going up." << std::endl;
			}
		}
		// UP이거나 BOTH이거나
		// 우선순위는 낮은 층 ((iter +++++++
		// 시작은 현재 층
	}
	//else 
	{
		for (auto iter = std::ranges::begin(floors); iter != std::ranges::end(floors); ++iter) {
			if (*iter == button::DOWN || *iter == button::BOTH) {
				if (std::distance(posi, std::begin(floors)) < std::distance(iter, std::begin(floors)))
					continue; // 현재 층보다 높은 층에 있는 버튼은 무시하기

				posi = iter;
				print_floor();
				if (*iter == button::DOWN) {
					*iter = button::NONE;
				}
				else { // *iter == BOTH
					*iter = button::UP;
				}
				std::cout << "Floor " << current_floor() << ". Going down." << std::endl;
			}
		}
		// DOWN이거나 BOTH이거나
		// 우선순위는 높은 층부터 (iter --------
		// 시작은 현재 층에서 부터
	}

	/*/
	// WHAT THE FU** HOW DO I SOLVE THIS
	if (auto down = std::find(std::begin(floors), std::end(floors), button::DOWN);
		posi > down) {
		posi = down;
		*posi = button::NONE;
	}

	std::sort(std::begin(floors), std::end(floors), []() {})

	//qsort(); ((우선순위부터 정렬해서 버튼 끄기.
	*/
}

void elevator::print() {
	if constexpr (is_debug) {
		for (auto criter = std::crbegin(floors); criter != std::crend(floors); ++criter) {
			if (auto floor = index_to_floor(std::crend(floors) - criter - 1); floor > 0)
				std::cout << floor;
			else
				std::cout << "B" << -floor;
			std::cout.put('\t');

			switch (*criter) {
			case button::UP: std::cout << "UP"; break;
			case button::DOWN: std::cout << "DOWN"; break;
			case button::BOTH: std::cout << "BOTH"; break;
			case button::NONE: std::cout << "NONE"; break;
			}
			std::cout << std::endl;
		}
	}
}

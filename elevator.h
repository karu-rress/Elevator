#pragma once

#include <algorithm>
#include <concepts>
#include <vector>
#include <exception>
#include <iostream>
#include <ranges>


#define LOG std::printf

class elevator {
public:
	elevator() : elevator(-2, 10) {   }
	elevator(int min, int max) : elevator(min, max, 1) {   }
	elevator(int min, int max, int pos) : pos{ pos } { 
		reset(min, max); 
		// posi = std::begin(floors) + floor_to_index(pos);
	}

	elevator(const elevator& rhs)
		: min_floor{ rhs.min_floor }, max_floor{ rhs.max_floor }
		, pos{ rhs.pos }, floors{ rhs.floors }, posi{ rhs.posi } {  }
	elevator(elevator&& rr) noexcept : min_floor{ rr.min_floor }, max_floor{ rr.max_floor },
		pos{ rr.pos }, floors{ std::move(rr.floors) }, posi{ std::move(rr.posi) } {   }

	enum class button
	{
		UP,
		DOWN,
		NONE,
		BOTH,
	};

	/// <summary>
	/// Reset the whole floor of the elevator.
	/// </summary>
	/// <param name="min">= Min floor. min != 0</param>
	/// <param name="max">= Max floor. max > 0</param>
	void reset(int min, int max);

	// Set the floor of the elevator
	inline void set_floor(int floor = 1) {
		pos = floor;
		posi = std::begin(floors) + floor_to_index(floor);
	}

	inline void push_button(int floor, button b) {
		floors.at(floor_to_index(floor)) = b;

		move();
	}

	inline void print_floor() {
		std::cout << "Current floor is floor " << index_to_floor(posi - std::begin(floors)) << "." << std::endl;
	}
	 
	inline size_t current_floor() {
		return index_to_floor(posi - std::begin(floors));
	}

	void print();
protected:
	virtual size_t floor_to_index(int floor);

	virtual int index_to_floor(size_t idx);

	inline virtual size_t floors_count() {
		return max_floor - min_floor + ((min_floor >= 1) ? 1 : 0);
	}

	virtual void move();

private:
	
	int min_floor;
	int max_floor;

	 int pos = 1; // 이걸 pos로 할까 iterator로 할까.. 나중에 상속해서 iterator로 할까
	std::vector<button>::iterator posi;

	std::vector<button> floors;

	enum class direction { UP, DOWN } dir = direction::UP;

#ifdef _DEBUG
	constexpr static bool is_debug = true;
#else
	constexpr static bool is_debug = false;
#endif
};


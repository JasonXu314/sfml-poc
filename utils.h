#ifndef UTILS_H
#define UTILS_H

#include <SFML/Graphics.hpp>
#include <iostream>

template <typename T>
std::ostream& operator<<(std::ostream& out, sf::Vector2<T> vec) {
	return out << "<" << vec.x << ", " << vec.y << ">";
}

#endif
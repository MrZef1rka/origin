#pragma once
#include <stdexcept>

class CreationError : public std::domain_error {
public:
	CreationError(const std::string& message);

};


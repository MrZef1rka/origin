#include "CreationError.h"

CreationError::CreationError(const std::string& message) : std::domain_error(message) {};


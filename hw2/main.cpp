#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "fleet.hpp"
#include "inventory.hpp"

std::string readWord(std::istringstream& input, const std::string& field) {
  std::string value;
  if (!(input >> value)) {
    throw CommandError("missing " + field);
  }
  return value;
}

template <typename T>
T readNumber(std::istringstream& input, const std::string& field) {
  std::istringstream token(readWord(input, field));
  T value;
  if (!(token >> value) || !token.eof()) {
    throw CommandError("invalid " + field);
  }
  return value;
}

void requireEnd(std::istringstream& input) {
  std::string extra;
  if (input >> extra) {
    throw CommandError("too many arguments");
  }
}

int readThreshold(std::istringstream& input) {
  input >> std::ws;
  if (input.eof()) {
    return 6;
  }
  const int threshold = readNumber<int>(input, "threshold");
  requireEnd(input);
  if (threshold < 1 || threshold > 10) {
    throw CommandError("threshold must be between 1 and 10");
  }
  return threshold;
}

std::unique_ptr<InventoryItem> readItem(std::istringstream& input) {
  const std::string type = readWord(input, "item type");
  const std::string id = readWord(input, "id");
  const std::string name = readWord(input, "name");

  if (type == "scooter" || type == "ebike") {
    const double energy = readNumber<double>(input, "energy");
    const int score = readNumber<int>(input, "score");
    requireEnd(input);
    if (type == "scooter") {
      return std::make_unique<Scooter>(id, name, energy, score);
    }
    return std::make_unique<ElectricBike>(id, name, energy, score);
  }
  if (type == "bike") {
    const int score = readNumber<int>(input, "score");
    requireEnd(input);
    return std::make_unique<Transport>(id, name, score);
  }
  if (type == "helmet") {
    requireEnd(input);
    return std::make_unique<Item>(id, name);
  }
  if (type == "dock") {
    const double energy = readNumber<double>(input, "energy");
    requireEnd(input);
    return std::make_unique<Dock>(id, name, energy);
  }
  if (type == "light") {
    const double watts = readNumber<double>(input, "power");
    const double hours = readNumber<double>(input, "hours");
    requireEnd(input);
    return std::make_unique<BikeLight>(id, name, watts, hours);
  }
  throw CommandError("unknown item type");
}

bool runCommand(Fleet& fleet, const std::string& line, std::ostream& out) {
  std::istringstream input(line);
  std::string command;
  if (!(input >> command)) {
    return true;
  }

  if (command == "add") {
    fleet.add(readItem(input));
  } else if (command == "remove" || command == "focus") {
    const std::string id = readWord(input, "id");
    requireEnd(input);
    if (command == "remove") {
      fleet.remove(id);
    } else {
      fleet.focus(id);
    }
  } else if (command == "show_focus") {
    requireEnd(input);
    fleet.showFocus(out);
  } else if (command == "list") {
    requireEnd(input);
    fleet.list(out);
  } else if (command == "novice") {
    fleet.listNovice(out, readThreshold(input));
  } else if (command == "count_novice") {
    out << fleet.countNovice(readThreshold(input)) << '\n';
  } else if (command == "energy" || command == "report") {
    requireEnd(input);
    const double energy = fleet.dailyEnergy();
    if (command == "report") {
      out << fleet.size() << ' ';
    }
    out << std::fixed << std::setprecision(1) << energy << '\n';
  } else if (command == "quit") {
    requireEnd(input);
    return false;
  } else {
    throw CommandError("unknown command");
  }
  return true;
}

int main() {
  Fleet fleet;
  std::string line;
  while (std::getline(std::cin, line)) {
    try {
      if (!runCommand(fleet, line, std::cout)) {
        break;
      }
    } catch (const FleetError& error) {
      std::cout << "error: " << error.what() << '\n';
    }
  }
}

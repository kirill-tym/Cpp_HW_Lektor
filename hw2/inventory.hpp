#pragma once

#include <cctype>
#include <cmath>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

class FleetError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

class InspectionError : public FleetError {
 public:
  using FleetError::FleetError;
};

class CommandError : public FleetError {
 public:
  using FleetError::FleetError;
};

class InventoryItem {
 public:
  virtual ~InventoryItem() = default;
  virtual const std::string& inventoryId() const = 0;
  virtual const std::string& name() const = 0;
  virtual void describe(std::ostream& out) const = 0;
  virtual double dailyEnergy() const { return 0.0; }
  virtual bool isForNovice(int) const { return false; }
  virtual void inspect() const {}

  friend std::ostream& operator<<(std::ostream& out,
                                  const InventoryItem& item) {
    item.describe(out);
    return out;
  }

  friend bool operator<(const InventoryItem& left, const InventoryItem& right) {
    return left.inventoryId() < right.inventoryId();
  }

  friend bool operator==(const InventoryItem& left,
                         const InventoryItem& right) {
    return left.inventoryId() == right.inventoryId();
  }
};

class EnergyConsumer {
 public:
  virtual ~EnergyConsumer() = default;
  virtual double dailyEnergy() const = 0;

 protected:
  EnergyConsumer(double energy) : energy_(energy) {
    if (!std::isfinite(energy) || energy < 0.0) {
      throw CommandError("energy must be finite and nonnegative");
    }
  }

  double energy() const { return energy_; }

 private:
  double energy_;
};

class Item : public InventoryItem {
 public:
  Item(std::string id, std::string name)
      : id_(std::move(id)), name_(std::move(name)) {
    if (id_.empty() || name_.empty()) {
      throw CommandError("id and name must not be empty");
    }
  }

  const std::string& inventoryId() const override { return id_; }
  const std::string& name() const override { return name_; }
  void describe(std::ostream& out) const override {
    out << id_ << ' ' << name_;
  }

 private:
  std::string id_;
  std::string name_;
};

class Transport : public Item {
 public:
  Transport(const std::string& id, const std::string& name, int score)
      : Item(id, name), score_(score) {
    if (score < 1 || score > 10) {
      throw CommandError("score must be between 1 and 10");
    }
  }

  bool isForNovice(int threshold) const override { return score_ >= threshold; }

  void inspect() const override {
    std::string lowerName = name();
    for (char& letter : lowerName) {
      letter =
          static_cast<char>(std::tolower(static_cast<unsigned char>(letter)));
    }
    if (lowerName.find("broken") != std::string::npos) {
      throw InspectionError("inspection rejected");
    }
  }

 private:
  int score_;
};

class Scooter : public Transport, public EnergyConsumer {
 public:
  Scooter(const std::string& id, const std::string& name, double energy,
          int score)
      : Transport(id, name, score), EnergyConsumer(energy) {}

  double dailyEnergy() const override { return energy(); }
};

class ElectricBike : public Transport, public EnergyConsumer {
 public:
  ElectricBike(const std::string& id, const std::string& name, double energy,
               int score)
      : Transport(id, name, score), EnergyConsumer(energy) {}

  double dailyEnergy() const override { return energy(); }
};

class Dock : public Item, public EnergyConsumer {
 public:
  Dock(const std::string& id, const std::string& name, double energy)
      : Item(id, name), EnergyConsumer(energy) {}

  double dailyEnergy() const override { return energy(); }
};

class BikeLight : public Item, public EnergyConsumer {
 public:
  BikeLight(const std::string& id, const std::string& name, double watts,
            double hours)
      : Item(id, name), EnergyConsumer(calculateEnergy(watts, hours)) {}

  double dailyEnergy() const override { return energy(); }

 private:
  static double calculateEnergy(double watts, double hours) {
    if (!std::isfinite(watts) || watts < 0.0) {
      throw CommandError("power must be finite and nonnegative");
    }
    if (!std::isfinite(hours) || hours < 0.0 || hours > 24.0) {
      throw CommandError("hours must be between 0 and 24");
    }
    return watts / 1000.0 * hours;
  }
};

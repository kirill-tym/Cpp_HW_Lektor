#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "inventory.hpp"

template <typename T>
class Registry {
 public:
  void add(std::unique_ptr<T> item) {
    for (const auto& existing : items_) {
      if (*existing == *item) {
        throw CommandError("duplicate inventory id");
      }
    }
    items_.push_back(std::shared_ptr<T>(std::move(item)));
  }

  std::shared_ptr<T> find(const std::string& id) const { return *findById(id); }

  void remove(const std::string& id) { items_.erase(findById(id)); }

  std::size_t size() const { return items_.size(); }

  const std::vector<std::shared_ptr<T>>& items() const { return items_; }

  std::vector<const T*> sortedItems() const {
    std::vector<const T*> sorted;
    sorted.reserve(items_.size());
    for (const auto& item : items_) {
      sorted.push_back(item.get());
    }
    std::sort(sorted.begin(), sorted.end(),
              [](const T* left, const T* right) { return *left < *right; });
    return sorted;
  }

 private:
  auto findById(const std::string& id) const {
    const auto position = std::find_if(
        items_.begin(), items_.end(),
        [&id](const auto& item) { return item->inventoryId() == id; });
    if (position == items_.end()) {
      throw CommandError("unknown inventory id");
    }
    return position;
  }

  std::vector<std::shared_ptr<T>> items_;
};

class Fleet {
 public:
  void add(std::unique_ptr<InventoryItem> item) {
    item->inspect();
    inventory_.add(std::move(item));
  }

  void remove(const std::string& id) { inventory_.remove(id); }

  void focus(const std::string& id) { showcase_ = inventory_.find(id); }

  void showFocus(std::ostream& out) const {
    if (const auto item = showcase_.lock()) {
      out << *item << '\n';
    } else {
      out << "expired\n";
    }
  }

  std::size_t size() const { return inventory_.size(); }

  double dailyEnergy() const {
    double total = 0.0;
    for (const auto& item : inventory_.items()) {
      total += item->dailyEnergy();
    }
    if (!std::isfinite(total)) {
      throw CommandError("total energy is too large");
    }
    return total;
  }

  void list(std::ostream& out) const {
    for (const auto* item : inventory_.sortedItems()) {
      out << *item << '\n';
    }
  }

  void listNovice(std::ostream& out, int threshold) const {
    for (const auto* item : inventory_.sortedItems()) {
      if (item->isForNovice(threshold)) {
        out << *item << '\n';
      }
    }
  }

  std::size_t countNovice(int threshold) const {
    return std::count_if(
        inventory_.items().begin(), inventory_.items().end(),
        [threshold](const auto& item) { return item->isForNovice(threshold); });
  }

 private:
  Registry<InventoryItem> inventory_;
  std::weak_ptr<InventoryItem> showcase_;
};

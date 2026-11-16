# ДЗ №2. «ФизКат»

Консольная программа для учёта транспорта и вещей на балансе. [Условие в Telegram](https://t.me/miptcpp2026/106/164).

## Сборка и запуск

Из корня репозитория:

```sh
mkdir -p build
c++ -std=c++17 -Wall -Wextra -Wpedantic hw2/main.cpp -o build/hw2
./build/hw2
```

Одна команда на строку. `id` и `name` вводятся без пробелов. У транспорта `score` от 1 до 10. `energy` в командах добавления указывается в кВт·ч/сутки.

| Команда | Что делает |
| --- | --- |
| `add scooter id name energy score` | Добавляет электросамокат после техосмотра |
| `add bike id name score` | Добавляет обычный велосипед после техосмотра |
| `add ebike id name energy score` | Добавляет электровелосипед после техосмотра |
| `add helmet id name` | Добавляет шлем |
| `add dock id name energy` | Добавляет док-станцию |
| `add light id name watts hours` | Добавляет фонарь; расход равен `watts / 1000 * hours` |
| `list` | Печатает все позиции в формате `id name`, по возрастанию `id` |
| `novice [threshold]` | Печатает подходящий новичкам транспорт; порог по умолчанию 6 |
| `count_novice [threshold]` | Печатает количество подходящих новичкам единиц транспорта |
| `energy` | Печатает суммарный суточный расход с одним знаком после точки |
| `report` | Печатает число позиций и расход в одной строке |
| `focus id`, `show_focus` | Выбирает позицию для витрины и показывает её |
| `remove id`, `quit` | Удаляет позицию или завершает программу |

## Наследование

Классы находятся в [inventory.hpp](inventory.hpp). Стрелка направлена от производного класса к базовому.

```text
Item         -> InventoryItem
Transport    -> Item
Scooter      -> Transport + EnergyConsumer
ElectricBike -> Transport + EnergyConsumer
Dock         -> Item + EnergyConsumer
BikeLight    -> Item + EnergyConsumer
```

`Item` используется для шлема, `Transport` — для обычного велосипеда. Знак `+` обозначает множественное наследование. У `InventoryItem` и `EnergyConsumer` нет общего базового класса, поэтому виртуальное наследование не требуется. `list`, `energy` и `novice` обращаются к позициям через базовый `InventoryItem`. `operator<<` печатает позицию, `operator<` сортирует по ID, `operator==` проверяет совпадение ID при добавлении.

## Владение, ошибки и шаблоны

- По уточнению от 4 ноября [в теме курса](https://t.me/miptcpp2026/106) новый объект до осмотра принадлежит `unique_ptr`. После успешного осмотра `Registry<InventoryItem>` в [fleet.hpp](fleet.hpp) хранит его в `shared_ptr`. Витрина хранит `weak_ptr`, поэтому при удалении позиции `show_focus` выводит `expired`. Повторное добавление того же ID витрину не восстанавливает.
- `InspectionError` бросается, когда имя транспорта содержит `broken` без учёта регистра. `CommandError` используется для ошибок ввода, неверных значений, повторяющихся и отсутствующих ID. Оба наследуются от `FleetError`. Сообщение печатается в `main`, неудачная команда не меняет парк.
- Шаблон `Registry<T>` хранит, ищет, удаляет и сортирует позиции. В программе используется `Registry<InventoryItem>`. `readNumber<T>` в [main.cpp](main.cpp) одинаково разбирает целые и вещественные аргументы.
- Креативная часть: фонарь `BikeLight` считает суточный расход по мощности и времени работы (от 0 до 24 часов), а `count_novice` считает транспорт для новичков с выбранным порогом.

## Пример

Ввод:

```text
add scooter S-1 ModelX 2.5 8
add scooter S-2 BROKEN 1.0 9
add light L-1 Beam 100 8
focus S-1
show_focus
report
remove S-1
show_focus
energy
quit
```

Вывод:

```text
error: inspection rejected
S-1 ModelX
2 3.3
expired
0.8
```

## Использование ИИ

ИИ помог подготовить код, этот README и проверки. Самостоятельные правки автора к этой версии пока не внесены.

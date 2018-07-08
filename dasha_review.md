# Осмотр.
`Врач: Владислав Богданов. Больной: некомпилирующийся код.`

## 1. Постоянство стиля:

Скобочки:
- )\n{			3
- ){			2
- ) {			10

Наличие скобок при if с одним действием:
- Наличие		4
- Отсутствие	10

Табуляция пробелами: 18 в caves.c, 1 в caves.h, 2 в common.h.

Закомментированный код: строчки 12-14, +15 отжиманий.

Дополнительно:
- Cъехала табуляция в caves.c:33. +8 за 8 строчек.
- В двух местах присутствует таб в конце строки. +2 жима.
- Совершенно неуместная пустая строка в caves.c:50. +1 жим.
- В строчках 46 и 51 творится что-то непотребное с табуляцией. +2 жима.

Итого за стиль - +68: +9 за скобочки, +21 за табуляцию пробелами, +15 за комментарии, +13 дополнительно и +10 за две капли крови, которая скатились по моим щечкам от вида *этого ужаса*.

## А также...
- Было сказано: используйте `#pragma once`. В одном хедере он есть, в другом - ...
	Короче, плохо. +10 отжиманий.
- В caves.c дублируются #define из common.h. Плохо. +5 отжиманий.
- Строки 65-80 в файле caves.c - очень много одинакового кода.
	Кода, легко вытаскиваемого как минимум в макрос.
	Третья капля крови - +5 отжиманий.
- Комментариев - вообще нет. Не то, чтобы хорошо, но и не плохо.
	По крайней мере, код выглядит весьма понятно.
- Отсутствует Makefile. Отсутствует `int main()`. Проект со 100% вероятностью не собирается. Хм?
- Однако, ошибки памяти предупреждены. Неплохо.
- Комментарии к коммитам в меру адекватные. Неплохо.

Всего отжиманий: 88.
Рекордсвумен.
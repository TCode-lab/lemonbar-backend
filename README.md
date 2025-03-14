![Демонстрация бара](https://raw.githubusercontent.com/TCode-lab/lemonbar-backend/cab88c070fbbbf91ceeaab5b855eb593f2ab79cc/bar-demo.png)
# lemonbar-backend
Моя реализация бекенда для lemonbar на C++.

Данная программа поддерживает использование модулей, через описание их в файле "./modules/modules.json". (см. каталог "example")

На данный момент поддерживаются следующие модули:
- CPU
- RAM
- Battery
- Time
- Script
- Shortcuts

# Модули
У каждого модуля имеются базовые параметры, которые обязательно необходимо указать, для его работы:
- type
- position
- interval

Параметр "type" указывает тип модуля (См. выше).

Параметр "position" может иметь значения: "Left", "Center", "Right".

Параметр "interval" указывает интервал обновления модуля в секундах.

У каждого из модулей так же есть уникальные параметры, которые будут описаны ниже. Данные параметры не обязательны к описыванию в файле "modules.json", если их не окажется, то будут использованы стандартные значения для модуля.

# CPU
"icon_cpu"          :   // Строка

"show_temp"         :   // Булиевая

"icon_temp_cold"    :   // Строка

"icon_temp_cool"    :   // Строка

"icon_temp_medium"  :   // Строка

"icon_temp_warm"    :   // Строка

"icon_temp_hot"     :   // Строка

# RAM
"icon"  :  // Строка

# Battery
"icon_full"         :   // Строка

"icon_four_fifths"  :   // Строка

"icon_one_half"     :   // Строка

"icon_two_fifths"   :   // Строка

"icon_one_fifths"   :   // Строка

"icon_status_charging"      :   // Строка

"icon_status_discharging"   :   // Строка

"icon_status_full"          :   // Строка

# Time
"short_out" : // Булиевая

# Script
"command"   : // Строка (проверял только: "bash", "python3" и "lua")

"on_start"      : // Строка (путь к исполняемому файлу)

"on_update"     : // Строка (путь к исполняемому файлу)

"on_click_left" : // Строка (путь к исполняемому файлу)

"on_click_middle" : // Строка (путь к исполняемому файлу)

"on_click_right" : // Строка (путь к исполняемому файлу)

"on_wheel_up" : // Строка (путь к исполняемому файлу)

"on_wheel_down" : // Строка (путь к исполняемому файлу)

# Shortcuts
"separator" : // Строка (разделяющий символ между ярлыками)

"shortcuts" : // Массив объектов с элементами "label" и "command" (см. "example/modules.json")
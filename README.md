# ClearPomidor
Фокус таймер, похожий на Clearfocus и Pomodoro, написан на C++ и QT5

1. При работе висит в трее.
  * Если незапущено приложение: серого цвета значок.
  * Если запущен: красного цвета (и при наведении всплыв. текст, где указано время сколько осталось).
  * Если перерыв: зеленого цвета (+при наведении оставшееся время).

2. Когда работает: таймер на 25 мин. Перерыв: 5 мин.
3. Есть управление Start/Pause, Stop.

4. Как заканчивается таймер, то иконка программы мигает: серо-красным (чередуясь) и срабатывает "будильник":
   * 3 раза полностью повторялся,
   *  15 сек перерыв, 3 раза повторился,
   *  20 сек перерыв, 4 раза повторился,
   *  минута перерыв, 4 раз повторился и
   *  минута перерыв, 4 раз повторился,
   *  и т.д..
  
5. Есть настройки времени и текстов.

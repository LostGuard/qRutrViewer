## Конвертер для дампа https://rutracker.org/forum/viewtopic.php?t=5290461 (версия 0.1.20170916) и просмотрщик получившийся базы.

## Сборка на Linux (Ubuntu)

Установка зависимостей:

    apt install libqt5gui5 libqt5core5a qttools5-dev qttools5-dev-tools libqt5webkit5-dev qt5-qmake

Сборка:

    git clone https://github.com/LostGuard/qRutrViewer && cd qRutrViewer

    # Если в системе стоит одновременно Qt 4 и Qt 5
    /usr/lib/x86_64-linux-gnu/qt5/bin/qmake && make

    # Если в системе только Qt 5
    qmake && make

Готовые бинарные файлы:
    
- qRutrXMLConv/qRutrXMLConv -- конвертер базы данных из XML в sqlite
- qRutrGui/qRutrGui         -- сама программа

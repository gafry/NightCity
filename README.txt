Tento soubor popisuje program Bakalaris, soubory,
které obsahuje a jak jej používat
---------------------------------------------------

1. Obsah
----------
Jednotlivé soubory:
 - Bakalaris.sln : Řešení programu ve Visual Studiu
 - Bakalaris : složka obsahuje projekt ve Visual Studiu,
 			   všechny potřebné knihovny pro kompilaci programu
 			   a zdrojové soubory 
 			   - Include & Libs : soubory knihoven
 			   - Source : zdrojové soubory
 			   - System : obsahuje překladač pro assembler YASM  
 			   			  a exe packer kkrunchy
 			   - Output : výstup kompilace
 - NightCity.exe : výsledný spustitelný soubor
 - LICENCE.txt : GNU licence

2. Kompilace
--------------
Projekt byl vyvíjen ve Visual Studiu 2017, byla použita verze OpenGL 4.5.
Před kompilací je nutné nastavit Release x86 build.
Výstupní program by se měl nacházet ve složce Output
s názvem NightCity.exe. Aby měl soubor menší velikost než
64kB, je potřeba ještě použít exe packer, který je ve složce System.
Stačí jednodušše přetáhnout NightCity.exe na kkrunchy_k7 a exe packer
komprimuje spustitelný soubor. Druhá možnost je použít příkaz ze
složky Bakalaris:
System\kkrunchy_k7.exe --best Output\NightCity.exe --out Output\NightCity.exe

3. Poznámky
-------------
Po domluvě s vedoucím nebyl plakát tisknut, pouze přiložen v archívu.

#Wstęp
Projekt ten to w pełni funkcjonalny, kompaktowy moduł radiowy AM/FM. Sercem układu jest wysokiej jakości tuner samochodowy TEF6686, sterowany przez osobny mikrokontroler STM32. Moduł dostarcza na wyjściu czysty, cyfrowy sygnał audio w standardzie I2S, gotowy do dalszej obróbki przez zewnętrzne procesory DSP lub wzmacniacze.
<ul>
<li>Odbiór pasm AM/FM: Wykorzystanie zaawansowanego tunera TEF6686.
<li>Cyfrowe wyjście audio: Strumień danych I2S zapewnia odpowiednią jakość dźwięku i odporność na zakłócenia.
<li>Sterowanie cyfrowe: Komunikacja z tunerem poprzez interfejs I2C.
<li>Niskopoziomowe programowanie: Oprogramowanie firmware dla mikrokontrolera STM32 napisane w C/C++ z wykorzystaniem bibliotek HAL.
<li>Projekt PCB: Kompaktowa, dwuwarstwowa płytka drukowana.
</ul>

#PCB
Projekt PCB został zrealizowany w technologii dwuwarstwowej, która jest wystarczająca dla optymalnego poprowadznia ścieżek. Wszystkie komponenty umieszczono na warstwie górnej, co upraszcza i przyspiesza proces montażu, zarówno ręcznego, jak i zautomatyzowanego. Warstwa dolna została niemal w całości pokryta wylewką miedzi połączoną z masą, tworząc niską impedancję powrotu prądu i skutecznie ekranując układ przed zakłóceniami elektromagnetycznymi (EMI/EMC). Na warstwie górnej przewidziano dedykowane, odsłonięte pola miedzi umożliwiające przylutowanie metalowego ekranu, co pozwala na dodatkowe odizolowanie wrażliwych obwodów wejściowych. Tory sygnałowe dla pasm AM i FM oraz linia zasilania anteny zostały starannie odseparowane i poprowadzone w sposób minimalizujący przesłuchy (crosstalk). Wyprowadzenia modułu rozmieszczono w standardowym rastrze 2,54 mm (0.1 cala), co zapewnia pełną kompatybilność z płytkami stykowymi i złączami goldpin, ułatwiając prototypowanie i integrację.

#Oprogramowanie
Oprogramowanie firmware zostało stworzone w języku C z wykorzystaniem bibliotek STM32 HAL. Do sterowania układem służą biblioteki (tef6686.c, tef6686.h), w których znajdują się funkcje odpowiedzialne za komunikację tunera radiowego poprzez magistralę I2C.
Pierwszym etapem pracy programu jest sekwencja inicjalizacyjna, podczas której do pamięci wewnętrznej układu TEF6686 ładowany jest oficjalny firmware patch dostarczony przez producenta (NXP). Ten proces, zrealizowany zgodnie z notą aplikacyjną, jest niezbędny do odblokowania pełnej funkcjonalności i zapewnienia optymalnych parametrów pracy tunera. Funkcja TEF6686_Tune_To(), pozwala na intuicyjne wstrojenie odbiornika na zadaną częstotliwość za pomocą jednego wywołania. Program służy jedynie do sprawdzenia poprawnego działania układu. Zostanie on rozbudowany w wersji drugiej projektu.

#Błędy
Dla prawidłowej wymiany danych zabrakło rezystorów podciągających.

#Kompromisy projektowe
Podczas fazy koncepcyjnej założono dwa cele: osiągnięcie wysokiej miniaturyzacji oraz umożliwienie montażu modułu bezpośrednio na większej płycie w technologii SMT, dzięki zastosowaniu półotworów (ang. castellated holes). Jednakże, konieczne okazało się zrezygnowanie z tych celów. Zrezygnowano z komponentów w obudowach mniejszych niż 0603. Mniejszy rozmiar powodowałby trudność w ręcznym montażu, bez profesjonalnych narzędzi. Co więcej, implementacja półotworów wiązałaby się z blisko dziesięciokrotnym wzrostem kosztu produkcji jednostkowej PCB, co jest niepotrzebne na pierwszym etapie projektu. W rezultacie, obecna wersja płytki jest celowo powiększona i wykorzystuje standardowe złącza goldpin. Te kompromisy, podyktowane względami praktycznymi i ekonomicznymi, zostaną poprawione w planowanej wersji v2, która będzie zoptymalizowana pod kątem rozmiarów.

#Dalszy rozwój projektu:
Ten moduł stanowi doskonałą bazę do rozbudowy o dodatkowe funkcjonalności:
<ul>
<li>Dodanie wyświetlacza LCD/OLED do pokazywania częstotliwości i informacji RDS.
<li>Dodanie enkodera obrotowego i przycisków do interakcji z użytkownikiem (zmiana częstotliwości, głośności, zapis stacji).
<li>Integracja z systemem audio opartym o mikrokontrolery z serii ESP32 w celu dodania łączności Wi-Fi/Bluetooth (radio internetowe).
</ul>
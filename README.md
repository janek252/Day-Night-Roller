# Day-Night-Roller

Układ sterowania roletą Dzień-Noc (D-N) projekt na przedmiot **Metodyki projektowania i modelowania systemów**
# Day-Night Roller Project

Całość projektu znajduje się w repozytorium pod linkiem: [Day-Night Roller GitHub](https://github.com/janek252/Day-Night-Roller)

Główne foldery w projekcie to:

- **src** – folder źródłowy, w nim znajdują się biblioteki, wykorzystywane w projekcie, sam kod źródłowy.
- **utilities** – folder zawierający pliki pomocnicze, w przypadku tego projektu program do szukania urządzeń podłączonych do danej magistrali i2c.
- **documentation** – folder zawierający plik Word z dokumentacją, oraz plik video prezentujący działanie projektu.


# Projekt sterowania roletą okienną typu dzień-noc

## Skrócony Opis projektu, dokadny opis znajduje się w pliku Dokumentacja_projektu.pdf

Celem projektu było zaprojektowanie i wykonanie układu sterowania roletą okienną typu dzień-noc, z siecią sensorów informującą użytkownika o warunkach środowiskowych. Umożliwiono zdalną kontrolę stopnia rozwinięcia rolety. Przeprowadzono testy aplikacji webowej, sensorów i temperatury silnika, potwierdzając skuteczność interfejsu i działania radiatora z wentylatorem.



### Biblioteki
```cpp
#include <WiFi.h> // Obsługa komunikacji WiFi
#include <BH1750.h> // Obsługa czujnika natężenia światła
#include <Adafruit_BMP085.h> // Obsługa czujnika temperatury
#include <Adafruit_BMP280.h> // Obsługa czujnika temperatury
#include <Wire.h> // Serial wire monitor
```



## 1. Wprowadzenie
Projekt ma na celu zaprojektowanie i wdrożenie systemu automatycznego sterowania roletą okienną typu dzień-noc. System wykorzystuje szereg sensorów do monitorowania warunków środowiskowych i umożliwia zdalne sterowanie roletą przez interfejs webowy.

## 2. Wymagania i komponenty
### 2.1. Wymagania sprzętowe
- Mikrokontroler ESP32
- Silnik krokowy wraz ze sterownikiem
- Czujniki natężenia światła (BH1750)
- Czujniki temperatury i ciśnienia (BMP180, BMP280)
- Transoptory do wykrywania położenia rolety
- Moduł WiFi do komunikacji zdalnej

### 2.2. Wymagania programowe
- Arduino IDE lub inna kompatybilna platforma do programowania mikrokontrolerów
- Biblioteki do obsługi sensorów i modułu WiFi:
  - WiFi.h
  - BH1750.h
  - Adafruit_BMP085.h
  - Adafruit_BMP280.h
  - Wire.h

## 3. Opis działania systemu
System składa się z dwóch głównych części: układu mechanicznego i elektronicznego sterowania roletą oraz interfejsu webowego do zdalnego monitorowania i sterowania.

### 3.1. Układ mechaniczny
Silnik krokowy steruje ruchem rolety w górę i w dół. Kierunek obrotu silnika oraz ilość kroków są kontrolowane przez mikrokontroler. Transoptory wykrywają krańcowe położenia rolety, co zapobiega jej uszkodzeniu poprzez przejście poza zadane pozycje.

### 3.2. Układ elektroniczny
Mikrokontroler ESP32 zarządza całą logiką sterowania roletą oraz komunikacją z czujnikami. Czujniki BH1750 mierzą natężenie światła wewnątrz i na zewnątrz pomieszczenia, a czujniki BMP180 i BMP280 monitorują temperaturę i ciśnienie. Dane z tych czujników są wykorzystywane do automatycznego dostosowywania położenia rolety w zależności od warunków środowiskowych.

### 3.3. Interfejs webowy
ESP32 pełni również rolę serwera webowego, umożliwiając użytkownikowi zdalny dostęp do systemu przez przeglądarkę internetową. Strona webowa prezentuje dane z czujników oraz oferuje przyciski do manualnego sterowania roletą.

## 4. Implementacja
### 4.1. Konfiguracja sprzętowa
Kod źródłowy zawiera definicje pinów mikrokontrolera, do których są podłączone poszczególne komponenty. Piny do sterowania silnikiem, przyciski manualne oraz transoptory są skonfigurowane na odpowiednich portach ESP32.

### 4.2. Inicjalizacja czujników
Czujniki są inicjalizowane w funkcji setup(). W przypadku niepowodzenia inicjalizacji program sygnalizuje błąd i zatrzymuje dalsze działanie, co pozwala na łatwe zdiagnozowanie problemów z okablowaniem lub uszkodzonymi czujnikami.

### 4.3. Konfiguracja WiFi
Mikrokontroler łączy się z lokalną siecią WiFi przy użyciu zadanych SSID i hasła. Po pomyślnym połączeniu, ESP32 uruchamia serwer webowy na porcie 80, umożliwiając dostęp do interfejsu sterowania przez przeglądarkę.

### 4.4. Sterowanie roletą
Funkcje sterujące ruchem rolety realizują odpowiednie sekwencje kroków silnika, zależnie od wybranego poziomu (pełne, pół, ćwierć, trzy czwarte). Kierunek obrotów jest kontrolowany przez zmianę stanu pinu DIR, a liczba kroków przez odpowiednie opóźnienia w pętli.

### 4.5. Obsługa transoptorów
Transoptory monitorują pozycje krańcowe rolety, zapobiegając jej przekroczeniu dolnej lub górnej granicy. Funkcja checkTransoptors() jest wywoływana w pętli głównej programu, aby na bieżąco sprawdzać stan transoptorów i w razie potrzeby zatrzymać ruch rolety.

### 4.6. Obsługa klienta webowego
Mikrokontroler obsługuje połączenia klientów webowych, przetwarzając zapytania HTTP i zwracając odpowiednie dane w formacie HTML. Na stronie webowej użytkownik może odczytać aktualne dane z czujników oraz sterować pozycją rolety za pomocą przycisków.

## 5. Testowanie i wnioski
System został przetestowany pod kątem poprawności działania czujników, silnika oraz interfejsu webowego. Testy potwierdziły, że system działa zgodnie z założeniami, a użytkownik może w łatwy sposób monitorować warunki środowiskowe oraz sterować roletą zdalnie. Dodatkowo, zastosowanie radiatora z wentylatorem okazało się skuteczne w utrzymaniu temperatury silnika na bezpiecznym poziomie.

## 6. Wnioski końcowe
Projekt osiągnął swoje cele, dostarczając funkcjonalny system automatycznego sterowania roletą okienną typu dzień-noc. Integracja czujników środowiskowych z systemem sterowania umożliwia automatyczne dostosowywanie położenia rolety, co zwiększa komfort użytkowania i efektywność energetyczną. System zdalnego sterowania przez interfejs webowy zapewnia dodatkową wygodę użytkownikom.
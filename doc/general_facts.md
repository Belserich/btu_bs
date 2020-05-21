# Definitionen

**Funktionssignatur** sind Name und Parameter einer Funktion (**Funktionskopf** sind Rueckgabewert plus Signatur).

**Deklaration** ist die Festlegung entscheidender Aspekte einer Variable/Funktion (Beispiel Aspekte: Name, Parameter, Rueckgabewert, Datentyp, Dimension \[bei Arrays\])).

_Robin: Deklaration ist die Vorgabe des "Aussehens" einer Variable/Funktion (bitte "Aussehen" konkretisieren!)._  
_Florian: Deklaration gibt die Existenz einer Variable/Funktion und ihre Parameter (im Falle einer Funktion) an (um es exakt zu haben "Parameter" zu "Aspekte" oder Synonyme verallgemeinern)._

- Deklaration gleicher Variablen/Funktionen kann mehr als einmal statt finden (im Gegensatz zur Definition)
- primitive Datentypen (z.B.: `int`, `float`, `char`) oder Klassen mit Default-Konstruktor (also leerer Parameterliste) werden bei einer Deklaration ohne Initialisierung wie in `int a;` automatisch initialisiert, dies wird mit dem Schluesselwort `extern` verhindert. 
    
**Definition** gibt einer Variable/Funktion Bedeutung indem sie ihren Wert/Funktionskoerper konkretisiert.

- Definition kann genau einmal statt finden (festgelegt durch die [One Definition Rule \[ODR\]](https://en.cppreference.com/w/cpp/language/definition))

**Initialisierung** reserviert Speicherplatz fuer eine Variable und setzt einen Startwert (der Startwert kann auch undefiniert sein).

- Ein **undefinierter Wert** ist jener Wert, der im Speicher liegt, aber nicht durch das Programm konkretisiert wird und daher beliebig sein kann.

**[Initialisierer](https://en.cppreference.com/w/cpp/language/initialization) (C++):**
```
(<Liste von Ausdruecken>)       (1)
= <Ausdruck>                    (2)
{<Liste von Initialisierern>}   (3)
```
_Ausnahme: (2) wird in (3) ohne "=" genutzt._

Beispiel:
```
int i {42};     // 1 Initialisierungen: (3)
int i(42);      // 1 Initialisierungen: (1)
int i = 42;     // 1 Initialisierungen: (2)
int i = {42};   // 2 Initialisierungen: i wird ueber (2) initialisiert, und der '<Ausdruck>' wird durch (3) initialisiert

// 4 Initialisierungen: ein char mit 'O', ein char mit 'K', ein char-Array mit 'O' und 'K' alle ueber (3), dann ueber (2)
char c[2] = {{'O'}, {'K'}};
```

_Frage_: Was ist der Unterschied zwischen Definition und Initialisierung?  
_Antwort_: Initialisierung bezieht sich auf die Aktion, welche vom Programm zur Laufzeit durchgefuehrt wird, waehrend die Definition sich auf die Darstellung der Funktion oder des initialen Wertes im Quellcode bezieht (man kann eine Funktion in C++ nach dieser Definition nicht instanziieren).

# Sammelfakten   
_Nicht validiert und daher nur sinnbildlich zu verstehen._
- in Assembler gehen saemtliche Schleifen oder Funktionsaufrufe auf jump-Befehl (`jmp` in nasm-Syntax) zurueck
- Header Dateien enthalten Funktions- und Variablendeklarationen und werden inkludiert damit der Linker Funktionsaufrufe ihrer Deklaration zuordnen kann
- Deklarationen werdenen der Definition verlinkt
- \#-Bezeichner stellen immer sog. **Praeprozessorbefehle** dar, jede davon hat individuelle Funktionalitaet
- Der Praeprozessor wird vor dem Compiler gestartet und verarbeitet die Compileranweisungen (jene Anweisungen, mit einer vorangestellten Raute)
- nichtfluechtige Register muessen von der aufgerufenden Prozedur am Ende wieder auf ihren Ursprungswert zurueckgesetzt werden
- fluechtige Register duerfen in einer Prozedur beliebig ueberschrieben werden
- Der Doppeltpunkt in der Klassendefinition steht fuer eine Vererbungsbeziehung (analog zu "extends" in Java)

TODO wird Programmstack bei Programmende mit Nullen gefuellt?

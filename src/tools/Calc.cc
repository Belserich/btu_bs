#include "tools/Calc.h"

#include "device/CgaChannel.h" // ermöglicht Kontrolle über Cursor
#include "device/Keyboard.h" // ermöglicht Eingaben durch Tastatur
#include "io/PrintStream.h" // ermöglicht Linefeeds und Ausgabe von Zahlen

extern CgaChannel cga;
extern PrintStream out;

Calculator::Calculator()
{
    init();
}

Calculator::Calculator(void* sp)
    : Activity(sp)
{
    init();
    wakeup();
}

void Calculator::init()
{
    clearBuffer();
    cga.clear();
}

void Calculator::body()
{
	Key next;
	for (next = keyboard.read(); (ch = next.getValue()) != '\e'; next = keyboard.read())
	{
		if (next.isAscii())
		{
			switch (ch)
			{
				case '\n':
					enter();
					break;

				case '\t':
					for (int i = 0; i < 4 && numChars + i < EXPR_SIZE_MAX; i++)
					{
						buffer[numChars++] = ' ';
					}
					break;

				case '\b':
					if (numChars > 0)
					{
						numChars--;
						for (int i = numChars; i <= EXPR_SIZE_MAX; i++)
						{
							buffer[i] = buffer[i+1];
						}
					}
					break;

				default:
					if (numChars < EXPR_SIZE_MAX)
					{
						insert(ch);
					}
					break;
			}
		}
		else
		{
			switch(ch)
			{
				case CodeTable::LEFT:
					moveLeft();
					break;
				case CodeTable::RIGHT:
					moveRight();
					break;
				case CodeTable::POS1:
					for (numChars = 0; buffer[numChars] == '\0'; numChars++);
					break;
				case CodeTable::END:
					for (numChars = EXPR_SIZE_MAX; numChars > 0 && buffer[numChars-1] == '\0'; numChars--);
					break;
				case CodeTable::DEL:
					for (int i = numChars; buffer[i] != '\0' && i < EXPR_SIZE_MAX; i++)
					{
						buffer[i] = buffer[i+1];
					}
					break;
			}
		}

		// verschiebt den Cursor
		int row, col;
		cga.getCursor(col, row);
		cga.setCursor(numChars, row);

		renderBuffer();
	}
}

void Calculator::insert(char c)
{
	for (int i = EXPR_SIZE_MAX - 1; i > numChars && i > 1; i--)
	{
		buffer[i] = buffer[i-1];
	}
	buffer[numChars++] = ch;
}

void Calculator::enter()
{
	int result;
	int code = interp.eval(buffer, result);
	if (code == 0)
	{
		// leert die Ausgabezeile
		out.print("\r\n");
		for (int i = 0; i < 80 - 1; i++)
		{
			out.print(' ');
		}

		// gibt das Ergebnis in Ausgabezeile aus
		out.print("\rErgebnis: ");
		out.print(result);
		out.print("\r\n");
		clearBuffer();
	}
	else
	{
		printErrorMsg(code);
	}
}

void Calculator::moveLeft()
{
	if (numChars > 0)
	{
		numChars--;
		while (numChars > 0 && buffer[numChars] == '\0') // ueberspringt alle Nullzeichen
		{
			numChars--;
		}
	}
}

void Calculator::moveRight()
{
	if (buffer[numChars] != '\0' && numChars <= EXPR_SIZE_MAX)
	{
		numChars++;
	}
}

void Calculator::renderBuffer()
{
    // Cursor sichern
    int column, row;
    cga.getCursor(column, row);

    // Zeile schreiben
    cga.setCursor(0, row);
    cga.write(buffer, EXPR_SIZE_MAX);

    // Cursor wiederherstellen
    cga.setCursor(column, row);
}

void Calculator::clearBuffer()
{
    // Alle Zeichen nullen und Null-Byte hinter der höchsten Stelle setzen
    for (unsigned i=0; i<=EXPR_SIZE_MAX; ++i)
        buffer[i] = 0;
    numChars = 0;
}

void Calculator::printErrorMsg(unsigned code)
{
	int col, row;
	cga.getCursor(col, row);

	out.print("\r\n");
	switch (code) {
    case Interpreter::UNEXP_EOT:
        out.print("Error: Unexpected end of expression!");
        break;

    case Interpreter::UNEXP_SYMBOL:
        out.print("Error: Unexpected symbol!");
        break;

    case Interpreter::ARITHM_ERROR:
        out.print("Error: Arithmetic error!");
        break;

    case Interpreter::BAD_BUFFER:
        out.print("Error: Invalid buffer!");
        break;

    default:
    	out.println("Unknown error.");
        break;
    }

    cga.setCursor(col, row);
}

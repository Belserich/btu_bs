#include <device/CgaChannel.h>

CgaChannel::CgaChannel()
	: CgaScreen()
{}

CgaChannel::CgaChannel(const CgaAttr &attr)
	: CgaScreen(attr)
{}

int CgaChannel::write(const char* data, int size)
{
	int col, row;
	getCursor(col, row);

	for (int i = 0; i < size; ++i)
	{
		if (data[i] == '\n') // neue Zeile
		{
			row += 1;
		}
		else if (data[i] == '\r') // Cursorruecklauf
		{
			col = 0;
		}
		else if (data[i] == '\b') // Zeichen loeschen
		{
			col -= 1;
			if (col < 0) // Cursor ist am linken Rand
			{
				row -= 1;
				col = Columns - 1;

				if (row < 0) // Cursor am oberen linken Rand
				{
					// verwerfe Zeichen
					row = 0;
					col = 0;
				}
			}

			setCursor(col, row);
			show(' ');
		}
		else
		{
			show(data[i]);

			col += 1;
			if (col >= Columns)
			{
				col = 0;
				row += 1; // TODO fragil, was wenn row out of bounds?
			}
		}
		setCursor(col, row);
	}

	return 0;
}

void CgaChannel::blueScreen(const char *error)
{
	CgaAttr attr(CgaAttr::Color::WHITE, CgaAttr::Color::BLUE);
	for (int col = 0; col < Columns; ++col)
	{
		for (int row = 0; row < Rows; ++row)
		{
			setCursor(col, row);
			show(' ', attr);
			// TODO Nachricht
		}
	}
}

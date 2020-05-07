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
		if (data[i] < 32) // Steuerzeichen
		{
			if (data[i] == '\n') // neue Zeile
			{
				row += 1;
				col = 0;
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
				show('\0');
			}
		}
		else
		{
			show(data[i]);

			col += 1;
			if (col >= Columns)
			{
				col = 0;
				row += 1;
			}
		}

		while (row >= Rows)
		{
			scroll();
			row -= 1;
			col = 0;
		}

		setCursor(col, row);
	}

	return 0;
}

void CgaChannel::blueScreen(const char *error)
{
	CgaAttr attr(CgaAttr::Color::WHITE, CgaAttr::Color::BLUE);
	CgaAttr oldAttr;

	getAttr(oldAttr);
	setAttr(attr);

	for (int col = 0; col < Columns; ++col)
	{
		for (int row = 0; row < Rows; ++row)
		{
			setCursor(col, row);
			show(' ');
		}
	}

	const char* bsText1 = "(>_<)> FATALITY! <(>_<)";
	const char* bsText2 = "^(>_<)^";
	int size1, size2, size3;

	for (size1 = 0; bsText1[size1] != '\0'; ++size1);
	for (size2 = 0; bsText2[size2] != '\0'; ++size2);
	for (size3 = 0; error[size3] != '\0'; ++size3);

	setCursor(Columns / 2 - (size1 / 2), Rows / 2 - 1);
	write(bsText1, size1);

	setCursor(Columns / 2 - (size3 / 2), Rows / 2 + 1);
	write(error, size3);

	attr.setBlinkState(true);
	setAttr(attr);
	setCursor(Columns / 2 - (size2 / 2), Rows / 2 - 2);
	write(bsText2, size2);

	setCursor(0, 0);
	setAttr(oldAttr);
}

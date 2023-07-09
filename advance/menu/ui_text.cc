/*********************************************************************

    ui_text.c

    Functions used to retrieve text used by MAME, to aid in
    translation.

    Copyright (c) 1996-2006, Nicola Salmoria and the MAME Team.
    Visit http://mamedev.org for licensing and usage restrictions.

*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ui_text.h"




lang_struct lang;

/* All entries in this table must match the enum ordering in "ui_text.h" */
static const char *mame_default_text[] =
{
	/* Exit */
	"Select command",
	"Continue",
	"Exit",
	"Shutdown",	

	/* Sort */
	"Select Sort Mode",
	"Parent",
	"Emulator",
	"Name",
	"Time played",
	"Smart time",
	"Play times",
	"Time per play",
	"Group",
	"Type",
	"Manufacturer",
	"Year",
	"Size",
	"Resolution",
	"Info",

	"Select List Mode",
	"Select Preview Mode",
	"Include Groups",
	"Include Emulators",
	"Include Types",
	
	/* Help */
	"In the game menu:",
	"Main menu",
	"Run the current game/On menu accept the choice",
	"Next preview mode/On menu change the option",
	"Next menu mode", 
	"Next game group",
	"Next game type",
	"Include/Exclude games by attribute",
	"Select the game sort method",
	"Next emulator",
	"Commands",
	"Change the current game group",
	"Change the current game type",
	"Run a clone",
	"Rotate the screen",
	"In the submenus:",
	"Accept",
	"Unselect all",
	"Select all",
	"Toggle (+ include, - exclude, * required)",
	"Cancel",
	
	/* submenu */
	"Listing...",
	"Settings...",
	"Emulators...",
	"Volume...",
	"Difficulty...",
	"Clone...",
	"Help",
	"Statistics",
	"Runcommand...",
	"Poweroff",
	"Menu",

	/* extra command */
	"Delete game snapshot",
	"Delete game clip",
	"Delete game flyer",
	"Delete game cabinet",
	"Delete game icon",
	"Delete shown image (from parent)",
	"Delete game sound",
	"Delete played sound (from parent)",
	"(shown)",
	"(played)",
	"No commands available",
	"Error",
	
	/* subthismenu */
	"Listing Emulator",
	"Listing Multiple",
	"Sort...",
	"Mode...",
	"Preview...",
	"Groups...",
	"Types...",
	"Filters...",
	
	/* suballmenu: */
	"Game Group...",
	"Game Type...",
	"Calibration...",
	"Save all settings",
	"Restore all settings",
	"Clear all stats",
	"Lock settings",
	"Settings",
	
	/* calib */
	"Joystick Configuration",
	"No bluetooth daemon",
	"Joystick",
	"<none>",
	"Connect one joystick to continue",
	"Press any button to continue",

	/* stat */
	"Listed",
	"Total",
	"Perc",
	"Games",
	"Play",
	"Time",
	"Most time",
	"Most play",
	"Most time per play",

	"Capture the current screen",
	
	"No game was found",
	"No game was found for the emulator",
	"No game matches the group selection for",
	"No game matches the type selection for",
	"No game matches the filter selection for",
	"<empty>",
	NULL
};



static const char **default_text[] =
{
	mame_default_text,

	NULL
};



static const char **trans_text;


int uistring_init (FILE *langfile)
{
	/*
        TODO: This routine needs to do several things:
            - load an external font if needed
            - determine the number of characters in the font
            - deal with multibyte languages

    */

	int i, j, str;
	char curline[255];
	char section[255] = "\0";
	char *ptr;
	int string_count;

	/* count the total amount of strings */
	string_count = 0;
	for (i = 0; default_text[i]; i++)
	{
		for (j = 0; default_text[i][j]; j++)
			string_count++;
	}

	/* allocate the translated text array, and set defaults */
	trans_text = (const char **)malloc(sizeof(const char *) * string_count);

	/* copy in references to all of the strings */
	str = 0;
	for (i = 0; default_text[i]; i++)
	{
		for (j = 0; default_text[i][j]; j++)
			trans_text[str++] = default_text[i][j];
	}

	memset(&lang, 0, sizeof(lang));

	/* if no language file, exit */
	if (!langfile)
		return 0;

	while (fgets (curline, sizeof(curline) / sizeof(curline[0]), langfile) != NULL)
	{
		/* Ignore commented and blank lines */
		if (curline[0] == ';') continue;
		if (curline[0] == '\n') continue;
		if (curline[0] == '\r') continue;

		if (curline[0] == '[')
		{
			ptr = strtok (&curline[1], "]");
			/* Found a section, indicate as such */
			strcpy (section, ptr);

			/* Skip to the next line */
			continue;
		}

		/* Parse the LangInfo section */
		if (strcmp (section, "LangInfo") == 0)
		{
			ptr = strtok (curline, "=");
			if (strcmp (ptr, "Version") == 0)
			{
				ptr = strtok (NULL, "\n\r");
				sscanf (ptr, "%d", &lang.version);
			}
			else if (strcmp (ptr, "Language") == 0)
			{
				ptr = strtok (NULL, "\n\r");
				strcpy (lang.langname, ptr);
			}
			else if (strcmp (ptr, "Author") == 0)
			{
				ptr = strtok (NULL, "\n\r");
				strcpy (lang.author, ptr);
			}
			else if (strcmp (ptr, "Font") == 0)
			{
				ptr = strtok (NULL, "\n\r");
				strcpy (lang.fontname, ptr);
			}
		}

		/* Parse the Strings section */
		if (strcmp (section, "Strings") == 0)
		{
			/* Get all text up to the first line ending */
			ptr = strtok (curline, "\n\r");

			/* Find a matching default string */
			str = 0;
			for (i = 0; default_text[i]; i++)
			{
				for (j = 0; default_text[i][j]; j++)
				{
					if (strcmp (curline, default_text[i][j]) == 0)
				{
					char transline[255];

					/* Found a match, read next line as the translation */
					fgets (transline, 255, langfile);

					/* Get all text up to the first line ending */
					ptr = strtok (transline, "\n\r");

					/* Allocate storage and copy the string */
						trans_text[str] = strdup(transline);
						if (!trans_text[str])
							return 1;
					}
					str++;
				}
			}
		}
	}

	/* indicate success */
	return 0;
}



const char * ui_getstring (int string_num)
{
		return trans_text[string_num];
}

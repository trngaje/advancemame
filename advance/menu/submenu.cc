/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 1999, 2000, 2001, 2002, 2003 Andrea Mazzoleni
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "portable.h"

#include "submenu.h"
#include "text.h"
#include "play.h"

#include "advance.h"

#include <sstream>
#include <iomanip>
#include <algorithm>

#include "ui_text.h"

using namespace std;

#define MSG_CHOICE_DX 30 * int_font_dx_get(menu)
#define MSG_CHOICE_X (int_dx_get() - MSG_CHOICE_DX) / 2
#define MSG_CHOICE_Y int_dy_get() / 2

string menu_name(config_state& rs, const string& s, unsigned event)
{
	if (!rs.menu_key)
		return s;

	string name = event_name(event);
	if (!name.length())
		return s;

	return s + "^" + name;
}

// ------------------------------------------------------------------------
// Sort menu

#define SORT_CHOICE_DX 15 * int_font_dx_get(menu)

int run_sort(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Parent", sort_by_root_name));
	ch.insert(ch.end(), choice("Emulator", sort_by_emulator));
	ch.insert(ch.end(), choice("Name", sort_by_name));
	ch.insert(ch.end(), choice("Time played", sort_by_time));
	ch.insert(ch.end(), choice("Smart time", sort_by_smart_time));
	ch.insert(ch.end(), choice("Play times", sort_by_session));
	ch.insert(ch.end(), choice("Time per play", sort_by_timepersession));
	ch.insert(ch.end(), choice("Group", sort_by_group));
	ch.insert(ch.end(), choice("Type", sort_by_type));
	ch.insert(ch.end(), choice("Manufacturer", sort_by_manufacturer));
	ch.insert(ch.end(), choice("Year", sort_by_year));
	ch.insert(ch.end(), choice("Size", sort_by_size));
	ch.insert(ch.end(), choice("Resolution", sort_by_res));
	ch.insert(ch.end(), choice("Info", sort_by_info));

	choice_bag::iterator i = ch.find_by_value(rs.sort_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(string(" ") + ui_getstring(UI_select_sort_mode), THIRD_CHOICE_X, THIRD_CHOICE_Y, SORT_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.sort_set((listsort_t)i->value_get());
	}

	return key;
}

// ------------------------------------------------------------------------
// Command menu

#define COMMAND_CHOICE_DX 33 * int_font_dx_get(menu)

#define SECOND_CHOICE_DX (int_dx_get() - SECOND_CHOICE_X * 2)

int run_command(config_state& rs)
{
	choice_bag ch;

	if (event_is_visible(EVENT_RUNCOMMAND))
		ch.insert(ch.end(), choice(ui_getstring(UI_Runcommand_), 10));

	if (!rs.console_mode && rs.current_game) {
		bool used_backdrop = false;
		bool used_sound = false;
		
		if (rs.current_game->preview_snap_get().is_deletable()) {
			string s = ui_getstring(UI_Delete_game_snapshot);
			if (rs.current_game->preview_snap_get() == rs.current_backdrop) {
				s += " ";
				s += ui_getstring(UI_shown);
				used_backdrop = true;
			}
			ch.insert(ch.end(), choice(s, 0));
		}
		if (rs.current_game->preview_clip_get().is_deletable()) {
			string s = ui_getstring(UI_Delete_game_clip);
			if (rs.current_game->preview_clip_get() == rs.current_backdrop) {
				s += " " ;
				s += ui_getstring(UI_shown);
				used_backdrop = true;
			}
			ch.insert(ch.end(), choice(s, 1));
		}
		if (rs.current_game->preview_flyer_get().is_deletable()) {
			string s = ui_getstring(UI_Delete_game_flyer);
			if (rs.current_game->preview_flyer_get() == rs.current_backdrop) {
				s += " ";
				s += ui_getstring(UI_shown);
				used_backdrop = true;
			}
			ch.insert(ch.end(), choice(s, 2));
		}
		if (rs.current_game->preview_cabinet_get().is_deletable()) {
			string s = ui_getstring(UI_Delete_game_cabinet);
			if (rs.current_game->preview_cabinet_get() == rs.current_backdrop) {
				s += " ";
				s += ui_getstring(UI_shown);
				used_backdrop = true;
			}
			ch.insert(ch.end(), choice(s, 3));
		}
		if (rs.current_game->preview_icon_get().is_deletable()) {
			string s = ui_getstring(UI_Delete_game_icon);
			if (rs.current_game->preview_icon_get() == rs.current_backdrop) {
				s += " ";
				s += ui_getstring(UI_shown);
				used_backdrop = true;
			}
			ch.insert(ch.end(), choice(s, 4));
		}
		if (!used_backdrop && rs.current_backdrop.is_deletable()) {
			string s = ui_getstring(UI_Delete_shown_image_from_parent);
			ch.insert(ch.end(), choice(s, 5));
		}
		if (rs.current_game->preview_sound_get().is_deletable()) {
			string s = ui_getstring(UI_Delete_game_sound);
			if (rs.current_game->preview_sound_get() == rs.current_sound) {
				s += " ";
				s += ui_getstring(UI_played);
				used_sound = true;
			}
			ch.insert(ch.end(), choice(s, 6));
		}
		if (!used_sound && rs.current_sound.is_deletable()) {
			ch.insert(ch.end(), choice(ui_getstring(UI_Delete_played_sound_from_parent), 7));
		}
	}

	for (script_container::iterator i = rs.script_bag.begin(); i != rs.script_bag.end(); ++i) {
		if (i->text.find("%s") != string::npos) {
			if (!rs.current_game)
				continue;
		}
		if (i->text.find("%p") != string::npos || i->text.find("%f") != string::npos) {
			if (!rs.current_game)
				continue;
			if (rs.current_game->rom_zip_set_get().size() == 0)
				continue;
		}
		ch.insert(ch.end(), choice(i->name, &*i));
	}

	if (ch.begin() == ch.end())
		ch.insert(ch.end(), choice(ui_getstring(UI_No_commands_available), -1));

	choice_bag::iterator i = ch.begin();

	int key = ch.run(string(" ") + rs.script_menu, SECOND_CHOICE_X, SECOND_CHOICE_Y, SECOND_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		int r;
		if (i->value_get() >= -1 && i->value_get() <= 256) {
			switch (i->value_get()) {
			case 0:
				r = remove(cpath_export(rs.current_game->preview_snap_get().archive_get()));
				break;
			case 1:
				r = remove(cpath_export(rs.current_game->preview_clip_get().archive_get()));
				break;
			case 2:
				r = remove(cpath_export(rs.current_game->preview_flyer_get().archive_get()));
				break;
			case 3:
				r = remove(cpath_export(rs.current_game->preview_cabinet_get().archive_get()));
				break;
			case 4:
				r = remove(cpath_export(rs.current_game->preview_icon_get().archive_get()));
				break;
			case 5:
				r = remove(cpath_export(rs.current_backdrop.archive_get()));
				break;
			case 6:
				r = remove(cpath_export(rs.current_game->preview_sound_get().archive_get()));
				break;
			case 7:
				r = remove(cpath_export(rs.current_sound.archive_get()));
				break;
			case 10:
				key = run_runcommand(rs);
				r=0;
				break;	
			case -1:
				r = 0;
				break;
			default:
				r = -1;
				break;
			}
		} else {
			script* s = static_cast<script*>(i->ptr_get());

			string text = s->text;

			if (rs.current_game) {
				text = subs(text, "%s", rs.current_game->name_without_emulator_get());
				if (rs.current_game->rom_zip_set_get().size()) {
					string path = *rs.current_game->rom_zip_set_get().begin();
					text = subs(text, "%p", path_export(path));
					text = subs(text, "%f", path_export(file_file(path)));
				}
			}

			int_unplug();

			r = target_script(text.c_str());

			int_plug();
		}

		if (r != 0) {
			choice_bag ch;
			ch.insert(ch.end(), choice(rs.script_error, 0));
			choice_bag::iterator i = ch.begin();
			ch.run(string(" ") + ui_getstring(UI_Error), MSG_CHOICE_X, MSG_CHOICE_Y, MSG_CHOICE_DX, i);
		}
	}

	return key;
}

// ------------------------------------------------------------------------
// Mode menu

#define MODE_CHOICE_DX 15 * int_font_dx_get(menu)

int run_mode(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Full", mode_full));
	ch.insert(ch.end(), choice("Full Mixed", mode_full_mixed));
	ch.insert(ch.end(), choice("Text", mode_text));
	ch.insert(ch.end(), choice("List", mode_list));
	ch.insert(ch.end(), choice("List Mixed", mode_list_mixed));
	ch.insert(ch.end(), choice("Tile 2x2", mode_tile_2x2));
	ch.insert(ch.end(), choice("Tile Tiny", mode_tile_tiny));
	ch.insert(ch.end(), choice("Tile Small", mode_tile_small));
	ch.insert(ch.end(), choice("Tile Normal", mode_tile_normal));
	ch.insert(ch.end(), choice("Tile Big", mode_tile_big));
	ch.insert(ch.end(), choice("Tile Enormous", mode_tile_enormous));
	ch.insert(ch.end(), choice("Tile Giant", mode_tile_giant));
	ch.insert(ch.end(), choice("Tile Icon", mode_tile_icon));
	ch.insert(ch.end(), choice("Tile Marquee", mode_tile_marquee));

	choice_bag::iterator i = ch.find_by_value(rs.mode_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(string(" ") + ui_getstring(UI_select_list_mode), THIRD_CHOICE_X, THIRD_CHOICE_Y, MODE_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.mode_set((listmode_t)i->value_get());
	}

	return key;
}

// ------------------------------------------------------------------------
// Preview menu

#define PREVIEW_CHOICE_DX 15 * int_font_dx_get(menu)

int run_preview(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Snap", preview_snap));
	ch.insert(ch.end(), choice("Title", preview_title));
	ch.insert(ch.end(), choice("Flyer", preview_flyer));
	ch.insert(ch.end(), choice("Cabinet", preview_cabinet));
	ch.insert(ch.end(), choice("Icon", preview_icon));
	ch.insert(ch.end(), choice("Marquee", preview_marquee));

	choice_bag::iterator i = ch.find_by_value(rs.preview_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(string(" ") + ui_getstring(UI_select_preview_mode), THIRD_CHOICE_X, THIRD_CHOICE_Y, PREVIEW_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.preview_set((listpreview_t)i->value_get());
	}

	return key;
}

// ------------------------------------------------------------------------
// Group menu

#define GROUP_CHOICE_DX 20 * int_font_dx_get(menu)

int run_group(config_state& rs)
{
	choice_bag ch;

	for (pcategory_container::const_iterator j = rs.group.begin(); j != rs.group.end(); ++j) {
		bool tag = rs.include_group_get().find((*j)->name_get()) != rs.include_group_get().end();
		ch.insert(ch.end(), choice((*j)->name_get(), tag, 0));
	}

	choice_bag::iterator i = ch.begin();

	int key = ch.run(string(" ") + ui_getstring(UI_include_groups), THIRD_CHOICE_X, THIRD_CHOICE_Y, GROUP_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		category_container c;
		for (choice_bag::const_iterator j = ch.begin(); j != ch.end(); ++j) {
			if (j->bistate_get())
				c.insert(j->desc_get());
		}
		rs.include_group_set(c);
	}

	return key;
}

void run_group_next(config_state& rs)
{
	category* next_select = 0;
	bool all_select = true;

	bool pred_in = false;
	for (pcategory_container::const_iterator j = rs.group.begin(); j != rs.group.end(); ++j) {
		if (pred_in)
			next_select = *j;
		pred_in = false;
		for (category_container::const_iterator k = rs.include_group_get().begin(); k != rs.include_group_get().end(); ++k) {
			if ((*j)->name_get() == *k) {
				pred_in = true;
				break;
			}
		}
		if (!pred_in)
			all_select = false;
	}

	category_container c;

	if (!all_select && next_select == 0) {
		// insert all
		for (pcategory_container::const_iterator j = rs.group.begin(); j != rs.group.end(); ++j) {
			c.insert((*j)->name_get());
		}
	} else {
		if ((all_select || next_select == 0) && rs.group.begin() != rs.group.end())
			next_select = *rs.group.begin();
		if (next_select != 0) {
			// insert the next
			c.insert(next_select->name_get());
		}
	}

	rs.include_group_set(c);
}

// ------------------------------------------------------------------------
// Emu menu

#define EMU_CHOICE_DX 20 * int_font_dx_get(menu)

int run_emu(config_state& rs)
{
	choice_bag ch;

	for (pemulator_container::const_iterator j = rs.emu_active.begin(); j != rs.emu_active.end(); ++j) {
		bool tag = false;
		for (emulator_container::const_iterator k = rs.include_emu_get().begin(); k != rs.include_emu_get().end(); ++k) {
			if ((*j)->user_name_get() == *k) {
				tag = true;
				break;
			}
		}
		ch.insert(ch.end(), choice((*j)->user_name_get(), tag, 0));
	}

	choice_bag::iterator i = ch.begin();

	int key = ch.run(string(" ") + ui_getstring(UI_include_emulators), SECOND_CHOICE_X, SECOND_CHOICE_Y, EMU_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		emulator_container c;
		for (choice_bag::const_iterator j = ch.begin(); j != ch.end(); ++j) {
			if (j->bistate_get())
				c.insert(c.end(), j->desc_get());
		}
		rs.include_emu_set(c);
	}

	return key;
}

emulator* run_emu_select(config_state& rs)
{
	choice_bag ch;

	for (emulator_container::const_iterator j = rs.include_emu_get().begin(); j != rs.include_emu_get().end(); ++j) {
		ch.insert(ch.end(), choice(*j, 0));
	}

	string emu;

	if (ch.size() > 1) {
		choice_bag::iterator i = ch.begin();
		int key = ch.run(" Select Emulator", THIRD_CHOICE_X, THIRD_CHOICE_Y, EMU_CHOICE_DX, i);

		if (key != EVENT_ENTER)
			return 0;

		emu = i->desc_get();
	} else {
		emu = ch.begin()->desc_get();
	}

	for (pemulator_container::const_iterator j = rs.emu_active.begin(); j != rs.emu_active.end(); ++j) {
		if ((*j)->user_name_get() == emu) {
			return *j;
		}
	}

	return 0;
}

void run_emu_next(config_state& rs)
{
	string next_select = "";
	bool pred_in = false;

	for (pemulator_container::const_iterator j = rs.emu_active.begin(); j != rs.emu_active.end(); ++j) {
		if (pred_in)
			next_select = (*j)->user_name_get();
		pred_in = false;
		for (emulator_container::const_iterator k = rs.include_emu_get().begin(); k != rs.include_emu_get().end(); ++k) {
			if ((*j)->user_name_get() == *k) {
				pred_in = true;
				break;
			}
		}
	}
	if (next_select.length() == 0 && rs.emu_active.begin() != rs.emu_active.end())
		next_select = (*rs.emu_active.begin())->user_name_get();

	emulator_container c;

	if (next_select.length() != 0)
		c.insert(c.end(), next_select);

	rs.include_emu_set(c);
}

// ------------------------------------------------------------------------
// Type menu

#define TYPE_CHOICE_DX 30 * int_font_dx_get(menu)

int run_type(config_state& rs)
{
	choice_bag ch;

	for (pcategory_container::const_iterator j = rs.type.begin(); j != rs.type.end(); ++j) {
		bool tag = rs.include_type_get().find((*j)->name_get()) != rs.include_type_get().end();
		ch.insert(ch.end(), choice((*j)->name_get(), tag, 0));
	}

	choice_bag::iterator i = ch.begin();

	int key = ch.run(string(" ") + ui_getstring(UI_include_types), THIRD_CHOICE_X, THIRD_CHOICE_Y, TYPE_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		category_container c;
		for (choice_bag::const_iterator j = ch.begin(); j != ch.end(); ++j) {
			if (j->bistate_get())
				c.insert(j->desc_get());
		}
		rs.include_type_set(c);
	}

	return key;
}

void run_type_next(config_state& rs)
{
	category* next_select = 0;
	bool all_select = true;

	bool pred_in = false;
	for (pcategory_container::const_iterator j = rs.type.begin(); j != rs.type.end(); ++j) {
		if (pred_in)
			next_select = *j;
		pred_in = false;
		for (category_container::const_iterator k = rs.include_type_get().begin(); k != rs.include_type_get().end(); ++k) {
			if ((*j)->name_get() == *k) {
				pred_in = true;
				break;
			}
		}
		if (!pred_in)
			all_select = false;
	}

	category_container c;
	if (!all_select && next_select == 0) {
		// insert all
		for (pcategory_container::const_iterator j = rs.type.begin(); j != rs.type.end(); ++j) {
			c.insert((*j)->name_get());
		}
	} else {
		if ((all_select || next_select == 0) && rs.type.begin() != rs.type.end())
			next_select = *rs.type.begin();
		if (next_select != 0) {
			// insert the next
			c.insert(next_select->name_get());
		}
	}
	rs.include_type_set(c);
}


// ------------------------------------------------------------------------
// Move menu

int run_group_move(config_state& rs)
{
	choice_bag ch;

	if (!rs.current_game)
		return EVENT_ENTER;

	for (pcategory_container::const_iterator j = rs.group.begin(); j != rs.group.end(); ++j) {
		ch.insert(ch.end(), choice((*j)->name_get(), 0));
	}

	choice_bag::iterator i = ch.find_by_desc(rs.current_game->group_get()->name_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Select Game Group", THIRD_CHOICE_X, THIRD_CHOICE_Y, GROUP_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.current_game->user_group_set(rs.group.insert(i->desc_get()));
	}

	return key;
}

int run_type_move(config_state& rs)
{
	choice_bag ch;

	if (!rs.current_game)
		return EVENT_ENTER;

	for (pcategory_container::const_iterator j = rs.type.begin(); j != rs.type.end(); ++j) {
		ch.insert(ch.end(), choice((*j)->name_get(), 0));
	}

	choice_bag::iterator i = ch.find_by_desc(rs.current_game->type_get()->name_get());
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Select Game Type", THIRD_CHOICE_X, THIRD_CHOICE_Y, TYPE_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.current_game->user_type_set(rs.type.insert(i->desc_get()));
	}

	return key;
}

// ------------------------------------------------------------------------
// Clone menu

#define CLONE_CHOICE_X int_dx_get() / 10
#define CLONE_CHOICE_Y int_dy_get() / 8
#define CLONE_CHOICE_DX int_dx_get() * 4 / 5

void run_clone(config_state& rs)
{
	choice_bag ch;
	choice_bag::iterator i;
	const game* base;

	rs.current_clone = 0;
	if (!rs.current_game)
		return;

	if (rs.current_game->software_get()) {
		base = &rs.current_game->root_get();
	} else {
		base = rs.current_game;
	}

	ostringstream s;
	s << base->description_get() << ", " << base->manufacturer_get() << ", " << base->year_get();
	ch.insert(ch.end(), choice(s.str(), (void*)base));

	for (pgame_container::const_iterator j = base->clone_bag_get().begin(); j != base->clone_bag_get().end(); ++j) {
		if (!(*j)->software_get()) {
			ostringstream s;

			s << (*j)->description_get() << ", " << (*j)->manufacturer_get() << ", " << (*j)->year_get();

			switch ((*j)->play_get()) {
			case play_imperfect: s << " [IMPERFECT]"; break;
			case play_preliminary: s << " [PRELIMINARY]"; break;
			default: break;
			}

			emulator* emu = base->emulator_get();
			if (!emu || emu->filter_working(**j))
				ch.insert(ch.end(), choice(s.str(), (void*)&**j));
		}
	}

	i = ch.begin();
	int key = ch.run(" Select clone", CLONE_CHOICE_X, CLONE_CHOICE_Y, CLONE_CHOICE_DX, i);
	if (key == EVENT_ENTER) {
		rs.current_clone = (game*)i->ptr_get();
	}
}

// ------------------------------------------------------------------------
// Calib menu

#define CALIB_CHOICE_DX (44 * int_font_dx_get(menu))
#define CALIB_CHOICE_DY (20 * int_font_dy_get(menu) + int_font_dy_get(bar))
#define CALIB_CHOICE_X (int_dx_get() - CALIB_CHOICE_DX) / 2
#define CALIB_CHOICE_Y (int_dy_get() - CALIB_CHOICE_DY) / 2

void run_calib(config_state& rs)
{
	int border = int_font_dx_get(bar) / 2;
	target_clock_t start;
	bool was_processing = false;
	bool now_waiting = false;

	int_idle_2_enable(true, 1);
	event_unassigned(true);

	start = target_clock();
	bool done = false;
	while (!done) {
		int x = CALIB_CHOICE_X;
		int y = CALIB_CHOICE_Y;
		int dy = CALIB_CHOICE_DY;
		int dx = CALIB_CHOICE_DX;
		int xc = x + dx / 2;
		int y_last = y + dy - int_font_dy_get(text);

		// forced replug at every iteration, needed in case autocalib is disabled
		int_joystick_replug();

		int_box(x - 2 * border, y - border, dx + 4 * border, dy + border * 2, 1, COLOR_CHOICE_NORMAL.foreground);
		int_clear(x - 2 * border + 1, y - border + 1, dx + 4 * border - 2, dy + border * 2 - 2, COLOR_CHOICE_NORMAL.background);

		const char* d_title = "Gamepad Configuration";
		int w_title = int_font_dx_get(bar, d_title);

		int_put(bar, xc - w_title / 2, y, dx, d_title, COLOR_CHOICE_TITLE);
		y += int_font_dy_get(bar);

		y += int_font_dy_get(text);

		int bt_line = 0;
		int f = open("/tmp/blue.msg", O_RDONLY);
		if (f >= 0) {
			char msg[4096];
			char c;
			int p;
			char* show;
			const char* state;
			int ret = read(f, msg, sizeof(msg) - 1);
			if (ret > 0) {
				msg[ret] = 0;
			} else {
				msg[0] = 0;
			}
			close(f);

			p = 0;
			state = stoken(&c, &p, msg, "\n", "");
			sskip(&p, msg, "\n");
			show = &msg[p];

			if (strcmp(state, "WORKING") == 0)
				was_processing = true;
			now_waiting = strcmp(state, "WAITING") == 0;

			p = 0;
			const char* token = stoken(&c, &p, show, "\n", "");
			while (*token) {
				int_put_filled_center(menu, x, y, dx, token, COLOR_CHOICE_NORMAL);
				y += int_font_dy_get(menu);
				++bt_line;
				token = stoken(&c, &p, show, "\n", "");
			}
		}

		if (bt_line == 0) {
			int_put_filled_center(text, x, y, dx, ui_getstring(UI_No_bluetooth_daemon), COLOR_CHOICE_NORMAL);
			y += int_font_dy_get(text);
		}

		y += int_font_dy_get(text);

		int j;
		for (j = 0; j < joystickb_count_get(); ++j) {
			ostringstream os;

			os << ui_getstring(UI_Joystick) << " " << j + 1;

			char name[DEVICE_NAME_MAX];
			if (joystickb_device_desc_get(j, name) == 0) {
				os << " - " << name;
			}
			if (joystickb_device_name_get(j, name) == 0) {
				os << " [" << name << "]";
			}

			int_put(text, x, y, dx, os.str().c_str(), COLOR_CHOICE_TITLE);
			y += int_font_dy_get(text);

			if (joystickb_stick_count_get(j) != 0) {
				ostringstream s_os;
				s_os << "  Sticks " << joystickb_stick_count_get(j) << ":";
				for (int s = 0; s < joystickb_stick_count_get(j); ++s) {
					s_os << " " << joystickb_stick_name_get(j, s);
				}
				int_put(text, x, y, dx, s_os.str().c_str(), COLOR_CHOICE_NORMAL);
				y += int_font_dy_get(text);
			}

			if (joystickb_rel_count_get(j) != 0) {
				ostringstream r_os;
				r_os << "  Rels " << joystickb_rel_count_get(j) << ":";
				for (int r = 0; r < joystickb_rel_count_get(j); ++r) {
					r_os << " " << joystickb_rel_name_get(j, r);
				}
				int_put(text, x, y, dx, r_os.str().c_str(), COLOR_CHOICE_NORMAL);
				y += int_font_dy_get(text);
			}

			if (joystickb_button_count_get(j) != 0) {
				ostringstream b_os;
				b_os << "  Buttons " << joystickb_button_count_get(j) << ":";
				for (int b = 0; b < joystickb_button_count_get(j); ++b) {
					b_os << " " << joystickb_button_name_get(j, b);
					if (int_put_width(text, b_os.str()) > (dx - (4 * border + 6 * int_font_dx_get(text) ))) { // limit to display text by trngaje, 계산식 다시 고민해야 함
						int_put(text, x, y, dx, b_os.str().c_str(), COLOR_CHOICE_NORMAL);
						y += int_font_dy_get(text);	
						b_os.str("");
						b_os.clear();
					}
				}
				int_put(text, x, y, dx, b_os.str().c_str(), COLOR_CHOICE_NORMAL);
				y += int_font_dy_get(text);
			}

			y += int_font_dy_get(text);
		}

		for (; j < 4; ++j) {
			ostringstream os;

			os << "Player " << j + 1 << " - <none>";

			int_put(text, x, y, dx, os.str().c_str(), COLOR_CHOICE_TITLE);
			y += int_font_dy_get(text);
			if (y >= y_last)
				break;

			int_put(text, x, y, dx, string("  Sticks ") + ui_getstring(UI_none_), COLOR_CHOICE_NORMAL);
			y += int_font_dy_get(text);
			if (y >= y_last)
				break;

			int_put(text, x, y, dx, string("  Buttons ") + ui_getstring(UI_none_), COLOR_CHOICE_NORMAL);
			y += int_font_dy_get(text);
			if (y >= y_last)
				break;

			y += int_font_dy_get(text);
			if (y >= y_last)
				break;
		}

		target_clock_t now = target_clock();

		if (joystickb_count_get() == 0) {
			ostringstream os;

			string command = rs.ui_autocalib_reset;
			if (command.length() != 0 && now_waiting && !was_processing) {
				target_clock_t elapsed = (now - start) / 1000000;

				if (elapsed < 60) {
					os << "Connect a gamepad in " << 120 - elapsed << " seconds" ;
				} else if (elapsed < 120) {
					os << "Going to unpair all gamepads in " << 120 - elapsed << " seconds" ;
				} else {
					system(command.c_str());
					start = target_clock();
				}
			} else {
				os << "Connect a gamepad to continue" ;
			}

			int w_exit = int_font_dx_get(menu, os.str().c_str());
			int_put(menu, xc - w_exit / 2, y, w_exit, os.str().c_str(), COLOR_CHOICE_NORMAL);
			y += int_font_dy_get(menu);
		} else {
			const char* d_exit = "Connect another gamepad or press any button to continue";
			int w_exit = int_font_dx_get(menu, d_exit);

			int_put(text, xc - w_exit / 2, y, w_exit, d_exit, COLOR_CHOICE_NORMAL);
			y += int_font_dy_get(text);
		}

		int_update();

		int key = int_event_get(false);
		switch (key) {
		case EVENT_ESC:
		case EVENT_ENTER:
		case EVENT_UNASSIGNED:
		case EVENT_CANCEL:
			done = true;
			break;
		}
	}

	event_unassigned(false);
	int_idle_2_enable(false, 0);
}

// ------------------------------------------------------------------------
// Volume

#define VOLUME_CHOICE_DX 10 * int_font_dx_get(menu)

int run_volume(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Full", 0));
	ch.insert(ch.end(), choice("-2 db", -2));
	ch.insert(ch.end(), choice("-4 db", -4));
	ch.insert(ch.end(), choice("-6 db", -6));
	ch.insert(ch.end(), choice("-8 db", -8));
	ch.insert(ch.end(), choice("-10 db", -10));
	ch.insert(ch.end(), choice("-12 db", -12));
	ch.insert(ch.end(), choice("-14 db", -14));
	ch.insert(ch.end(), choice("-16 db", -16));
	ch.insert(ch.end(), choice("-18 db", -18));
	ch.insert(ch.end(), choice("-20 db", -20));
	ch.insert(ch.end(), choice("Silence", -40));

	choice_bag::iterator i = ch.find_by_value(play_attenuation_get());
	if (i == ch.end())
		i = ch.find_by_value(play_attenuation_get() - 1); // if the value is odd
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Volume", SECOND_CHOICE_X, SECOND_CHOICE_Y, VOLUME_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		play_attenuation_set(i->value_get());
		play_mute_set(false);
	}

	return key;
}

// ------------------------------------------------------------------------
// Exit

#define EXIT_CHOICE_DX 18 * int_font_dx_get(menu)
#define EXIT_CHOICE_DY 4 * int_font_dy_get(menu)
#define EXIT_CHOICE_X (int_dx_get() - EXIT_CHOICE_DX) / 2
#define EXIT_CHOICE_Y (int_dy_get() - EXIT_CHOICE_DY) / 2

bool run_exit(config_state& rs, int key)
{
	choice_bag ch;

	ch.insert(ch.end(), choice(ui_getstring(UI_continue), 0));
	switch (key) {
	case EVENT_ESC:
	case EVENT_EXIT:
		ch.insert(ch.end(), choice(ui_getstring(UI_Exit), EVENT_EXIT));
		break;
	case EVENT_OFF:
		ch.insert(ch.end(), choice(ui_getstring(UI_Shutdown), EVENT_OFF));
		break;
	}

	choice_bag::iterator i = ch.begin();

	string menutitle = " ";
	menutitle += ui_getstring(UI_select_command);
	key = ch.run(string(" ") + ui_getstring(UI_select_command), EXIT_CHOICE_X, EXIT_CHOICE_Y, EXIT_CHOICE_DX, i, true);

	if (key == EVENT_ENTER && i->value_get())
		return true;

	return false;
}


// ------------------------------------------------------------------------
// Difficulty

#define DIFFICULTY_CHOICE_DX 10 * int_font_dx_get(menu)

int run_difficulty(config_state& rs)
{
	choice_bag ch;

	ch.insert(ch.end(), choice("Default", difficulty_none));
	ch.insert(ch.end(), choice("Easiest", difficulty_easiest));
	ch.insert(ch.end(), choice("Easy", difficulty_easy));
	ch.insert(ch.end(), choice("Normal", difficulty_medium));
	ch.insert(ch.end(), choice("Hard", difficulty_hard));
	ch.insert(ch.end(), choice("Hardest", difficulty_hardest));

	choice_bag::iterator i = ch.find_by_value(rs.difficulty_effective);
	if (i == ch.end())
		i = ch.begin();

	int key = ch.run(" Difficulty", SECOND_CHOICE_X, SECOND_CHOICE_Y, DIFFICULTY_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		rs.difficulty_effective = static_cast<difficulty_t>(i->value_get());
	}

	return key;
}

// ------------------------------------------------------------------------
// Sub Menu

#define MENU_CHOICE_DX 20 * int_font_dx_get(menu)

void clear_stat(config_state& rs)
{
	for (game_set::const_iterator i = rs.gar.begin(); i != rs.gar.end(); ++i) {
		if (i->is_time_set() || i->is_session_set()) {
			i->time_set(0);
			i->session_set(0);
		}
	}
}

int run_suballmenu(config_state& rs)
{
	choice_bag ch;

	rs.sub_disable(); // force the use of the default config
	if (rs.group.size() > 1 && event_is_visible(EVENT_SETGROUP))
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Game_Group_), EVENT_SETGROUP), 7, rs.current_game != 0));
	if (rs.type.size() > 1 && event_is_visible(EVENT_SETTYPE))
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Game_Type_), EVENT_SETTYPE), 8, rs.current_game != 0));
	if (event_is_visible(EVENT_CALIBRATION))
		ch.insert(ch.end(), choice(ui_getstring(UI_Calibration_), 9));
	ch.insert(ch.end(), choice(ui_getstring(UI_Save_all_settings), 6));
	
	ch.insert(ch.end(), choice(ui_getstring(UI_Restore_all_settings), 20));
	
	ch.insert(ch.end(), choice(ui_getstring(UI_Clear_all_stats), 21));
	if (event_is_visible(EVENT_LOCK))
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Lock_settings), EVENT_LOCK), 11));

	choice_bag::iterator i = ch.begin();

	int key;
	bool done = false;

	do {
		void* save = int_save();

		key = ch.run(string(" ") + ui_getstring(UI_Settings), SECOND_CHOICE_X, SECOND_CHOICE_Y, MENU_CHOICE_DX, i);

		if (key == EVENT_ENTER) {
			switch (i->value_get()) {
			case 0:
				key = run_sort(rs);
				break;
			case 1:
				key = run_mode(rs);
				break;
			case 2:
				key = run_preview(rs);
				break;
			case 3:
				key = run_type(rs);
				break;
			case 4:
				key = run_group(rs);
				break;
			case 6:
				rs.restore_save();
				break;
			case 20:
				rs.restore_load();
				break;
			case 21:
				clear_stat(rs);
				break;
			case 7:
				key = run_group_move(rs);
				break;
			case 8:
				key = run_type_move(rs);
				break;
			case 9:
				run_calib(rs);
				break;
			case 11:
				rs.lock_effective = !rs.lock_effective;
				break;
			}
		} else if (key == EVENT_ESC || key == EVENT_CANCEL ) {
			done = true;
		}

		int_restore(save);

		if (key == EVENT_ENTER || key == EVENT_MENU)
			done = true;

	} while (!done);

	rs.sub_enable(); // restore the use of the normal config

	return key;
}

int run_subthismenu(config_state& rs)
{
	choice_bag ch;

	if (event_is_visible(EVENT_SORT))
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Sort_), EVENT_SORT), 0));
	if (event_is_visible(EVENT_MODE))
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Mode_), EVENT_MODE), 1));
	if (event_is_visible(EVENT_PREVIEW))
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Preview_), EVENT_PREVIEW), 2));
	if (rs.group.size() > 1 && event_is_visible(EVENT_GROUP))
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Groups_), EVENT_GROUP), 4));
	if (rs.type.size() > 1 && event_is_visible(EVENT_TYPE))
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Types_), EVENT_TYPE), 3));
	if (event_is_visible(EVENT_ATTRIB))
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Filters_), EVENT_ATTRIB), 11, rs.include_emu_get().size() != 0));

	string title = " ";
	if (rs.sub_has()) {
		title += ui_getstring(UI_Listing_Emulator);
	} else {
		title += ui_getstring(UI_Listing_Multiple);
	}

	choice_bag::iterator i = ch.begin();

	int key;
	bool done = false;

	do {
		void* save = int_save();

		key = ch.run(title, SECOND_CHOICE_X, SECOND_CHOICE_Y, MENU_CHOICE_DX, i);

		if (key == EVENT_ENTER) {
			emulator* emu;
			switch (i->value_get()) {
			case 0:
				key = run_sort(rs);
				break;
			case 1:
				key = run_mode(rs);
				break;
			case 2:
				key = run_preview(rs);
				break;
			case 3:
				key = run_type(rs);
				break;
			case 4:
				key = run_group(rs);
				break;
			case 5:
				rs.sub_get().restore_save();
				break;
			case 6:
				rs.sub_get().sort_unset();
				break;
			case 7:
				rs.sub_get().mode_unset();
				break;
			case 8:
				rs.sub_get().preview_unset();
				break;
			case 9:
				rs.sub_get().include_type_unset();
				break;
			case 10:
				rs.sub_get().include_group_unset();
				break;
			case 11:
				emu = run_emu_select(rs);
				if (emu) {
					key = emu->attrib_run(FOURTH_CHOICE_X, FOURTH_CHOICE_Y);
				} else {
					key = EVENT_ESC;
				}
				break;
			}
		} else if (key == EVENT_ESC || key == EVENT_CANCEL ) {
			done = true;
		}

		int_restore(save);

		if (key == EVENT_ENTER || key == EVENT_MENU)
			done = true;

	} while (!done);

	return key;
}

#define SUBMENU_CHOICE_X (int_dx_get() / 20)
#define SUBMENU_CHOICE_Y (int_dy_get() / 15)
#define SUBMENU_CHOICE_DX (int_dy_get() - SUBMENU_CHOICE_X * 2)

int run_submenu(config_state& rs)
{
	choice_bag ch;
	target_nfo("log: run_submenu:++\n");
	
	if (!rs.console_mode) {
		ch.insert(ch.end(), choice(ui_getstring(UI_Listing_), 1));
		ch.insert(ch.end(), choice(ui_getstring(UI_Settings_), 0));

		if (rs.emu.size() > 1 && event_is_visible(EVENT_EMU)) {
			ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Emulators_), EVENT_EMU), 7));
		}
		if (event_is_visible(EVENT_VOLUME)) {
			ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Volume_), EVENT_VOLUME), 16));
		}
		if (event_is_visible(EVENT_DIFFICULTY)) {
			ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Difficulty_), EVENT_DIFFICULTY), 17));
		}
		
		if (event_is_visible(EVENT_COMMAND))
			ch.insert(ch.end(), choice(menu_name(rs, rs.script_menu, EVENT_COMMAND), 8));
		if (event_is_visible(EVENT_CLONE)) {
			ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Clone_), EVENT_CLONE), 15));
		}
		if (event_is_visible(EVENT_HELP)) {
			ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Help), EVENT_HELP), 10));
		}
		ch.insert(ch.end(), choice(ui_getstring(UI_Statistics), 18));

	} else {
		if (event_is_visible(EVENT_HELP)) {
			ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Help), EVENT_HELP), 10));
		}
		if (rs.emu.size() > 1 && event_is_visible(EVENT_EMU)) {
			ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Emulators_), EVENT_EMU), 7));
		}
		if (event_is_visible(EVENT_VOLUME)) {
			ch.insert(ch.end(), choice(ui_getstring(UI_Volume_), 16));
		}
		if (event_is_visible(EVENT_DIFFICULTY)) {
			ch.insert(ch.end(), choice(ui_getstring(UI_Difficulty_), 17));
		}
		if (event_is_visible(EVENT_COMMAND))
			ch.insert(ch.end(), choice(menu_name(rs, rs.script_menu, EVENT_COMMAND), 8));
	}
/*
	ch.insert(ch.end(), choice(ui_getstring(UI_Runcommand_), 30));
*/
	if ((rs.exit_mode == exit_esc || rs.exit_mode == exit_all || rs.exit_mode == exit_menu) && event_is_visible(EVENT_ESC)) {
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Exit), EVENT_ESC), 19));
	} else if ((rs.exit_mode == exit_exit || rs.exit_mode == exit_all || rs.exit_mode == exit_menu) && event_is_visible(EVENT_EXIT)) {
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Exit), EVENT_ESC), 19));
	}
	if ((rs.exit_mode == exit_shutdown || rs.exit_mode == exit_all || rs.exit_mode == exit_menu) && event_is_visible(EVENT_OFF)) {
		ch.insert(ch.end(), choice(menu_name(rs, ui_getstring(UI_Poweroff), EVENT_OFF), 21));
	}

	choice_bag::iterator i = ch.begin();

	int key;
	bool done = false;
	int ret = EVENT_NONE;

	do {
		void* save = int_save();

		key = ch.run(string(" ") + ui_getstring(UI_Menu), SUBMENU_CHOICE_X, SUBMENU_CHOICE_Y, SUBMENU_CHOICE_DX, i);
	
		if (key == EVENT_ENTER) {
			switch (i->value_get()) {
			case 0:
				key = run_suballmenu(rs);
				break;
			case 1:
				key = run_subthismenu(rs);
				break;
			case 7:
				key = run_emu(rs);
				break;
			case 8:
				key = run_command(rs);
				break;
			case 10:
				run_help(rs);
				break;
			case 15:
				done = true;
				ret = EVENT_CLONE;
				break;
			case 16:
				key = run_volume(rs);
				break;
			case 17:
				key = run_difficulty(rs);
				break;
			case 18:
				run_stat(rs);
				break;
			case 19:
				done = true;
				ret = EVENT_ESC;
				break;
			case 20:
				done = true;
				ret = EVENT_EXIT;
				break;
			case 21:
				done = true;
				ret = EVENT_OFF;
				break;
/*			case 30:
				key = run_runcommand(rs);
				break;		*/	
			}
		} else if (key == EVENT_ESC || key == EVENT_CANCEL ) {
			done = true;
		}

		int_restore(save);

		if (key == EVENT_MENU || key == EVENT_ENTER)
			done = true;

	} while (!done);

	target_nfo("log: run_submenu:--\n");
	
	return ret;
}

// ------------------------------------------------------------------------
// Help menu

void run_help(config_state& rs)
{
	bool wait = true;

	int_clear(COLOR_HELP_NORMAL.background, COLOR_MENU_OVERSCAN.background);

	if (rs.ui_help != "none") {
		wait = int_clip(rs.ui_help, true);
	} else {
		if (rs.ui_back != "none") {
			unsigned x, y;
			int_image(rs.ui_back, x, y);
		}

		int_clear_alpha(rs.ui_left, rs.ui_top, int_dx_get() - rs.ui_left - rs.ui_right, int_dy_get() - rs.ui_top - rs.ui_bottom, COLOR_HELP_NORMAL.background);

		int y = rs.ui_top;
		int xt = rs.ui_left + 2 * int_font_dx_get(text);
		int xd = rs.ui_left + (2 + 12) * int_font_dx_get(text);

		y += int_font_dy_get(text);
		int_put_alpha(text, xt, y, ui_getstring(UI_In_the_game_menu_), COLOR_HELP_NORMAL);
		y += int_font_dy_get(text);
		if (rs.console_mode) {
			int_put_alpha(text, xt, y, event_name(EVENT_ESC), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Main_menu), COLOR_HELP_NORMAL);
		} else {
			int_put_alpha(text, xt, y, event_name(EVENT_MENU), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Main_menu), COLOR_HELP_NORMAL);
		}
		y += int_font_dy_get(text);
		int_put_alpha(text, xt, y, event_name(EVENT_ENTER), COLOR_HELP_TAG);
		int_put_alpha(text, xd, y, ui_getstring(UI_Run_the_current_game_On_menu_accept_the_choice), COLOR_HELP_NORMAL);
		y += int_font_dy_get(text);
		if (event_name(EVENT_CANCEL) != "NONE") {
			int_put_alpha(text, xt, y, event_name(EVENT_CANCEL), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Cancel), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);			
		}
		
		if (event_is_visible(EVENT_SPACE) && (event_name(EVENT_SPACE) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_SPACE), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Next_preview_mode_On_menu_change_the_option), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (event_is_visible(EVENT_MODE) && (event_name(EVENT_MODE) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_MODE), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Next_menu_mode), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if ((rs.exit_mode == exit_esc || rs.exit_mode == exit_all || rs.exit_mode == exit_menu) && event_is_visible(EVENT_ESC) && (event_name(EVENT_ESC) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_ESC), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Exit), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		} else if ((rs.exit_mode == exit_exit || rs.exit_mode == exit_all || rs.exit_mode == exit_menu) && event_is_visible(EVENT_EXIT) && (event_name(EVENT_EXIT) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_EXIT), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Exit), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if ((rs.exit_mode == exit_shutdown || rs.exit_mode == exit_all || rs.exit_mode == exit_menu) && event_is_visible(EVENT_OFF) && (event_name(EVENT_OFF) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_OFF), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Shutdown), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (rs.group.size() > 1 && event_is_visible(EVENT_GROUP) && (event_name(EVENT_GROUP) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_GROUP), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Next_game_group), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (rs.type.size() > 1 && event_is_visible(EVENT_TYPE) && (event_name(EVENT_TYPE) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_TYPE), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Next_game_type), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (event_is_visible(EVENT_ATTRIB) && (event_name(EVENT_ATTRIB) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_ATTRIB), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Include_Exclude_games_by_attribute), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (event_is_visible(EVENT_SORT) && (event_name(EVENT_SORT) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_SORT), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Select_the_game_sort_method), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (rs.emu.size() > 1 && event_is_visible(EVENT_EMU) && (event_name(EVENT_EMU) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_EMU), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Next_emulator), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (event_is_visible(EVENT_COMMAND) && (event_name(EVENT_COMMAND) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_COMMAND), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Commands), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (rs.group.size() > 1 && event_is_visible(EVENT_SETGROUP) && (event_name(EVENT_SETGROUP) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_SETGROUP), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Change_the_current_game_group), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (rs.type.size() > 1 && event_is_visible(EVENT_SETTYPE) && (event_name(EVENT_SETTYPE) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_SETTYPE), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Change_the_current_game_type), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (event_is_visible(EVENT_CLONE) && (event_name(EVENT_CLONE) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_CLONE), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Run_a_clone), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (event_is_visible(EVENT_ROTATE) && (event_name(EVENT_ROTATE) != "NONE")) {
			int_put_alpha(text, xt, y, event_name(EVENT_ROTATE), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Rotate_the_screen), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (event_name(EVENT_RUNCOMMAND) != "NONE") {
			int_put_alpha(text, xt, y, event_name(EVENT_RUNCOMMAND), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Runcommand_), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (event_name(EVENT_CAPTURE) != "NONE") {
			int_put_alpha(text, xt, y, event_name(EVENT_CAPTURE), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Capture_the_current_screen), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}		
		
		y += int_font_dy_get(text);
		int_put_alpha(text, xt, y, ui_getstring(UI_In_the_submenus_), COLOR_HELP_NORMAL);
		y += int_font_dy_get(text);
		int_put_alpha(text, xt, y, event_name(EVENT_ENTER), COLOR_HELP_TAG);
		int_put_alpha(text, xd, y, ui_getstring(UI_Accept), COLOR_HELP_NORMAL);
		y += int_font_dy_get(text);
		if (event_name(EVENT_CANCEL) != "NONE") {
			int_put_alpha(text, xt, y, event_name(EVENT_CANCEL), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Cancel), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);			
		}
		if (event_name(EVENT_DEL) != "NONE") {
			int_put_alpha(text, xt, y, event_name(EVENT_DEL), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Unselect_all), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (event_name(EVENT_INS) != "NONE") {
			int_put_alpha(text, xt, y, event_name(EVENT_INS), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Select_all), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
		if (event_name(EVENT_SPACE) != "NONE") {
			int_put_alpha(text, xt, y, event_name(EVENT_SPACE), COLOR_HELP_TAG);
			int_put_alpha(text, xd, y, ui_getstring(UI_Toggle_include_exclude_required), COLOR_HELP_NORMAL);
			y += int_font_dy_get(text);
		}
	}


	
	int_update();	
/*	
	if (wait) {
		//int_event_get();
		int_event_get(false);
	}
*/
	
	bool done = false;
	while (!done) {

		int key = int_event_get(false);
		switch (key) {
		case EVENT_ESC:
		case EVENT_ENTER:
		case EVENT_UNASSIGNED:
		case EVENT_CANCEL:
			done = true;
			break;
		}
	}

}

// ------------------------------------------------------------------------
// Stat

#define STAT_MAX 10

void stat_insert(const game* (&map)[STAT_MAX], unsigned (&val)[STAT_MAX], const game* g, unsigned v)
{
	for (unsigned i = 0; i < STAT_MAX; ++i) {
		if (!map[i] || val[i] < v) {
			for (unsigned j = STAT_MAX - 1; j > i; --j) {
				map[j] = map[j - 1];
				val[j] = val[j - 1];
			}
			map[i] = g;
			val[i] = v;
			return;
		}
	}
}

string stat_time(unsigned v)
{
	ostringstream os;
	os << (v / 3600) << ":" << setw(2) << setfill('0') << ((v / 60) % 60);
	return os.str();
}

string stat_int(unsigned v)
{
	ostringstream os;
	os << v;
	return os.str();
}

string stat_perc(unsigned v, unsigned t)
{
	ostringstream os;
	if (t)
		os << stat_int(v * 100 / t) << "%";
	else
		os << stat_int(0) << "%";
	return os.str();
}

void run_stat(config_state& rs)
{
	unsigned total_count = 0;
	unsigned total_session = 0;
	unsigned total_time = 0;
	unsigned select_count = 0;
	unsigned select_session = 0;
	unsigned select_time = 0;
	const game* most_session_map[STAT_MAX] = { 0, 0, 0 };
	const game* most_time_map[STAT_MAX] = { 0, 0, 0 };
	const game* most_timepersession_map[STAT_MAX] = { 0, 0, 0 };
	unsigned most_session_val[STAT_MAX] = { 0, 0, 0 };
	unsigned most_time_val[STAT_MAX] = { 0, 0, 0 };
	unsigned most_timepersession_val[STAT_MAX] = { 0, 0, 0 };
	unsigned n;

	int y = rs.ui_top;
	int xn = rs.ui_left + 2 * int_font_dx_get(text);
	int xs = rs.ui_left + (2 + 1 * 8) * int_font_dx_get(text);
	int xt = rs.ui_left + (2 + 2 * 8) * int_font_dx_get(text);
	int xp = rs.ui_left + (2 + 3 * 8) * int_font_dx_get(text);
	int xe = rs.ui_left + (2 + 4 * 8) * int_font_dx_get(text);

	n = ((int_dy_get() - rs.ui_top - rs.ui_bottom) / int_font_dy_get(text) - 12) / 3;
	if (n > STAT_MAX)
		n = STAT_MAX;

	int_clear(COLOR_HELP_NORMAL.background, COLOR_MENU_OVERSCAN.background);

	if (rs.ui_back != "none") {
		unsigned x, y;
		int_image(rs.ui_back, x, y);
	}

	int_clear_alpha(rs.ui_left, rs.ui_top, int_dx_get() - rs.ui_left - rs.ui_right, int_dy_get() - rs.ui_top - rs.ui_bottom, COLOR_HELP_NORMAL.background);

	// select and sort
	for (game_set::const_iterator i = rs.gar.begin(); i != rs.gar.end(); ++i) {
		unsigned session;
		unsigned time;
		unsigned timepersession;

		if (i->emulator_get()->tree_get())
			session = i->session_tree_get();
		else
			session = i->session_get();
		if (i->emulator_get()->tree_get())
			time = i->time_tree_get();
		else
			time = i->time_get();
		if (session)
			timepersession = time / session;
		else
			timepersession = 0;

		total_count += 1;
		total_session += session;
		total_time += time;

		// emulator
		if (!i->emulator_get()->state_get())
			continue;

		// group
		if (!i->group_derived_get()->state_get())
			continue;

		// type
		if (!i->type_derived_get()->state_get())
			continue;

		// filter
		if (!i->emulator_get()->filter(*i))
			continue;

		select_count += 1;
		select_session += session;
		select_time += time;

		stat_insert(most_session_map, most_session_val, &*i, session);
		stat_insert(most_time_map, most_time_val, &*i, time);
		stat_insert(most_timepersession_map, most_timepersession_val, &*i, timepersession);
	}

	y += int_font_dy_get(text);
	int_put_right_alpha(text, xs, y, xt - xs, ui_getstring(UI_Listed), COLOR_HELP_TAG);
	int_put_right_alpha(text, xt, y, xp - xt, ui_getstring(UI_Total), COLOR_HELP_TAG);
	int_put_right_alpha(text, xp, y, xe - xp, ui_getstring(UI_Perc), COLOR_HELP_TAG);

	{

		y += int_font_dy_get(text);
		int_put_alpha(text, xn, y, ui_getstring(UI_Games), COLOR_HELP_TAG);
		int_put_right_alpha(text, xs, y, xt - xs, stat_int(select_count), COLOR_HELP_NORMAL);
		int_put_right_alpha(text, xt, y, xp - xt, stat_int(total_count), COLOR_HELP_NORMAL);
		int_put_right_alpha(text, xp, y, xe - xp, stat_perc(select_count, total_count), COLOR_HELP_NORMAL);
	}

	{
		y += int_font_dy_get(text);
		int_put_alpha(text, xn, y, ui_getstring(UI_Play), COLOR_HELP_TAG);
		int_put_right_alpha(text, xs, y, xt - xs, stat_int(select_session), COLOR_HELP_NORMAL);
		int_put_right_alpha(text, xt, y, xp - xt, stat_int(total_session), COLOR_HELP_NORMAL);
		int_put_right_alpha(text, xp, y, xe - xp, stat_perc(select_session, total_session), COLOR_HELP_NORMAL);
	}

	{
		y += int_font_dy_get(text);
		int_put_alpha(text, xn, y, ui_getstring(UI_Time), COLOR_HELP_TAG);
		int_put_right_alpha(text, xs, y, xt - xs, stat_time(select_time), COLOR_HELP_NORMAL);
		int_put_right_alpha(text, xt, y, xp - xt, stat_time(total_time), COLOR_HELP_NORMAL);
		int_put_right_alpha(text, xp, y, xe - xp, stat_perc(select_time, total_time), COLOR_HELP_NORMAL);
	}

	xs = (1 + 7) * int_font_dx_get(text);
	xe = (1 + 7 + 5) * int_font_dx_get(text);
	xt = (1 + 7 + 5 + 2) * int_font_dx_get(text);

	if (n > 0 && most_time_map[0]) {
		y += int_font_dy_get(text);
		y += int_font_dy_get(text);
		int_put_alpha(text, xn, y, ui_getstring(UI_Most_time), COLOR_HELP_TAG);
		for (unsigned i = 0; i < n && most_time_map[i]; ++i) {
			y += int_font_dy_get(text);
			int_put_right_alpha(text, xn, y, xs - xn, stat_time(most_time_val[i]), COLOR_HELP_NORMAL);
			int_put_right_alpha(text, xs, y, xe - xs, stat_perc(most_time_val[i], select_time), COLOR_HELP_NORMAL);
			int_put_alpha(text, xt, y, most_time_map[i]->description_get(), COLOR_HELP_NORMAL);
		}
	}

	if (n > 0 && most_session_map[0]) {
		ostringstream os;
		y += int_font_dy_get(text);
		y += int_font_dy_get(text);
		int_put_alpha(text, xn, y, ui_getstring(UI_Most_play), COLOR_HELP_TAG);
		for (unsigned i = 0; i < n && most_session_map[i]; ++i) {
			y += int_font_dy_get(text);
			int_put_right_alpha(text, xn, y, xs - xn, stat_int(most_session_val[i]), COLOR_HELP_NORMAL);
			int_put_right_alpha(text, xs, y, xe - xs, stat_perc(most_session_val[i], select_session), COLOR_HELP_NORMAL);
			int_put_alpha(text, xt, y, most_session_map[i]->description_get(), COLOR_HELP_NORMAL);
		}
	}

	if (n > 0 && most_timepersession_map[0]) {
		ostringstream os;
		y += int_font_dy_get(text);
		y += int_font_dy_get(text);
		int_put_alpha(text, xn, y, ui_getstring(UI_Most_time_per_play), COLOR_HELP_TAG);
		for (unsigned i = 0; i < n && most_timepersession_map[i]; ++i) {
			y += int_font_dy_get(text);
			int_put_right_alpha(text, xn, y, xs - xn, stat_time(most_timepersession_val[i]), COLOR_HELP_NORMAL);
			int_put_alpha(text, xt, y, most_timepersession_map[i]->description_get(), COLOR_HELP_NORMAL);
		}
	}


	int_update();	
//	int_event_get();
//	int_event_get(false);
	
	bool done = false;
	while (!done) {

		int key = int_event_get(false);
		switch (key) {
		case EVENT_ESC:
		case EVENT_ENTER:
		case EVENT_UNASSIGNED:
		case EVENT_CANCEL:
			done = true;
			break;
		}
	}

}

#define RUNCOMMAND_CHOICE_X 2 * int_font_dx_get(menu)
#define RUNCOMMAND_CHOICE_Y 4 * int_font_dy_get(menu)
#define RUNCOMMAND_CHOICE_DX (int_dx_get() - 2 * (RUNCOMMAND_CHOICE_X))

int run_runcommand(config_state& rs)
{
	choice_bag ch;
	choice_bag::iterator i;
	
	// emulator/game rom : rs.current_game->name_get().c_str()
	// to extract name of emulator from rs.current_game->name_get()
	string emulator = rs.current_game->name_get().substr(0, rs.current_game->name_get().find("/")); 
	// to get name of rom : rs.current_game->name_without_emulator_get()

    char buf[200];
	int cnt=1;
	
    FILE *fp;
	string path_runcommand = rs.ui_runcommand_cfg; //"/mnt/SDCARD/runcommand";
	

	string path_runcommand_config = path_runcommand + "/cfg/" + emulator + ".cfg";
	string emulatorconfig = "cat " + path_runcommand_config + " | grep 'CORES' | awk -F= '{print $2}'";
	
	if (rs.ui_runcommand_cfg != "none") {
		fp = popen(emulatorconfig.c_str(), "r");
		if (fp != NULL) {
			while (fgets(buf, sizeof(buf), fp) != NULL) {
				char* pch = NULL;

				pch = strtok(buf, "\r\n");

				while (pch != NULL)
				{
					ch.insert(ch.end(), choice(pch, cnt++));			
					pch = strtok(NULL, "\r\n");
				}		
			} 
			
			pclose(fp);
		}
	}
	
	string defaultconfig = "cat " + path_runcommand_config + " | grep 'DEFAULT' | awk -F= '{print $2}'";
	string defaultemulator;
	
	if (rs.ui_runcommand_cfg != "none") {
		fp = popen(defaultconfig.c_str(), "r");
		if (fp != NULL) {
			while (fgets(buf, sizeof(buf), fp) != NULL) {
				// 한줄이기 때문에 정리 필요함
				char* pch = NULL;

				pch = strtok(buf, "\r\n");

				while (pch != NULL)
				{
					defaultemulator = pch;
					pch = strtok(NULL, "\r\n");
				}		
			} 
			
			pclose(fp);
		}		
	}
	

	
	
	if (ch.begin() == ch.end())
		ch.insert(ch.end(), choice("No list", -1));
	
//	if (i == ch.end())
		i = ch.begin();

// 아래는 에러 발생함, 유사한 함수가 없을까?
//	i = ch.find_by_value(defaultemulator);
	
	int key = ch.run(emulator + "(" + defaultemulator + ")", RUNCOMMAND_CHOICE_X, RUNCOMMAND_CHOICE_Y, RUNCOMMAND_CHOICE_DX, i);

	if (key == EVENT_ENTER) {
		string selected_emulator = i->desc_get();
		choice_bag ch2;
		// 선택된 아이템이름 : i->desc_get()
		// 선택된 아이템 값 : i->value_get() 
	
		//DEFAULT=$1
		//CORE_CONF_FILE="/home/odroid/runcommand/cfg/"$2".cfg"

		//sed -i '/DEFAULT=/c\DEFAULT=\"'"${DEFAULT}"'\"' ${CORE_CONF_FILE}
		string defaultsetcommand = "sed -i '/DEFAULT=/c\DEFAULT=" + i->desc_get() + "' " + path_runcommand_config;
		
		//string runcommand_setdefault = path_runcommand + "/setdefault.sh " +  selected_emulator + " " + emulator;
		
		fp = popen(defaultsetcommand.c_str(), "r");
		if (fp != NULL) {
			pclose(fp);
		}

#if 0
		ch2.insert(ch2.end(), choice(defaultsetcommand, 0));
		choice_bag::iterator ii = ch2.begin();
		ch2.run(" 선택함", MSG_CHOICE_X, MSG_CHOICE_Y, MSG_CHOICE_DX, ii);	
#endif
	}
	
	return 0;



}

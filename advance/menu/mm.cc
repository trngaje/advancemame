/*
 * This file is part of the Advance project.
 *
 * Copyright (C) 1999-2002 Andrea Mazzoleni
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

#include "mconfig.h"
#include "menu.h"
#include "submenu.h"
#include "text.h"
#include "play.h"
#include "log.h"
#include "target.h"
#include "os.h"
#include "videoall.h"
#include "soundall.h"
#include "keyall.h"
#include "joyall.h"
#include "mouseall.h"

#include <iostream>

#include <stdio.h>
#include <assert.h>
#include <unistd.h>

using namespace std;

// --------------------------------------------------------------------------
// Run

int run_sub(config_state& rs, bool silent)
{

	log_std(("menu: int_init4 call\n"));

	if (!int_enable(rs.video_font_path, rs.video_orientation_effective)) {
		return INT_KEY_ESC;
	}

	bool done = false;
	bool is_run = false;
	int key = 0;

	log_std(("menu: menu start\n"));

	while (!done) {
		emulator* emu;

		key = run_menu(rs, (rs.video_orientation_effective & INT_ORIENTATION_SWAP_XY) != 0, silent);

		// don't replay the sound and clip
		silent = true;

		if (!rs.lock_effective)
		switch (key) {
			case INT_KEY_HELP :
				run_help(rs);
				break;
			case INT_KEY_GROUP :
				// replay the sound and clip
				silent = false;
				run_group_next(rs);
				break;
			case INT_KEY_EMU :
				// replay the sound and clip
				silent = false;
				run_emu_next(rs);
				break;
			case INT_KEY_TYPE :
				// replay the sound and clip
				silent = false;
				run_type_next(rs);
				break;
			case INT_KEY_EXCLUDE :
				// replay the sound and clip
				silent = false;
				emu = run_emu_select(rs);
				if (emu)
					emu->attrib_run();
				break;
			case INT_KEY_COMMAND :
				run_command(rs);
				break;
			case INT_KEY_SORT :
				// replay the sound and clip
				silent = false;
				run_sort(rs);
				break;
			case INT_KEY_MENU :
				// replay the sound and clip
				silent = false;
				if (run_submenu(rs)) {
					if (rs.current_game) {
						done = true;
						is_run = true;
						key = INT_KEY_ENTER;
					}
				}
				break;
			case INT_KEY_SETGROUP :
				// replay the sound and clip
				silent = false;
				run_group_move(rs);
				break;
			case INT_KEY_SETTYPE :
				// replay the sound and clip
				silent = false;
				run_type_move(rs);
				break;
			case INT_KEY_ROTATE :
			case INT_KEY_ESC :
			case INT_KEY_OFF :
				done = true;
				break;
		}
		switch (key) {
			case INT_KEY_LOCK :
				rs.lock_effective = !rs.lock_effective;
				break;
			case INT_KEY_IDLE_0 :
				if (rs.current_game) {
					rs.current_clone = &rs.current_game->clone_best_get();
					done = true;
					is_run = true;
				}
				break;
			case INT_KEY_RUN_CLONE :
				// replay the sound and clip
				silent = false;
				run_clone(rs);
				if (rs.current_clone) {
					done = true;
					is_run = true;
				}
				break;
			case INT_KEY_ENTER :
				// replay the sound and clip
				silent = false;
				if (rs.current_game) {
					done = true;
					is_run = true;
				}
				break;
		}
	}

	if (is_run) {
		assert( rs.current_game );
		if (!rs.current_clone) {
			if (rs.current_game->emulator_get()->tree_get())
				rs.current_clone = &rs.current_game->clone_best_get();
			else
				rs.current_clone = rs.current_game;
		}
		if (!rs.video_reset_mode)
			run_runinfo(rs);
	}

	log_std(("menu: menu stop\n"));

	log_std(("menu: int_disable call\n"));
	int_disable();

	return key;
}

int run_main(config_state& rs, bool is_first, bool silent)
{
	log_std(("menu: int_set call\n"));

	if (!int_set(rs.video_gamma, rs.video_brightness,
		rs.idle_start_first, rs.idle_start_rep, rs.idle_saver_first, rs.idle_saver_rep, rs.repeat, rs.repeat_rep,
		rs.preview_fast, rs.alpha_mode)) {
		return INT_KEY_ESC;
	}

	log_std(("menu: play_init call\n"));
	if (!play_init()) {
		int_unset(true);
		target_err("Error initializing the sound mixer.\n");
		target_err("Try with the option '-device_sound none'.\n");
		return INT_KEY_ESC;
	}

	// play start background sounds
	if (is_first) {
		play_background_effect(rs.sound_background_begin, PLAY_PRIORITY_EVENT, false);
	} else {
		play_background_effect(rs.sound_background_stop, PLAY_PRIORITY_EVENT, false);
	}

	// play start foreground sounds
	if (is_first) {
		play_foreground_effect_begin(rs.sound_foreground_begin);
	} else {
		play_foreground_effect_stop(rs.sound_foreground_stop);
	}

	// fill the player buffer
	log_std(("menu: play_fill call\n"));
	play_fill();

	bool done = false;
	bool is_terminate = false;
	bool is_run = false;
	int key = 0;

	log_std(("menu: menu start\n"));

	while (!done) {
		key = run_sub(rs, silent);

		// don't replay the sound and clip
		silent = true;

		if (!rs.lock_effective)
		switch (key) {
			case INT_KEY_ROTATE : {
					unsigned mirror = rs.video_orientation_effective & (INT_ORIENTATION_FLIP_X | INT_ORIENTATION_FLIP_Y);
					unsigned flip = rs.video_orientation_effective & INT_ORIENTATION_SWAP_XY;
					if (mirror == 0) {
						mirror = INT_ORIENTATION_FLIP_Y;
					} else if (mirror == INT_ORIENTATION_FLIP_Y) {
						mirror = INT_ORIENTATION_FLIP_X | INT_ORIENTATION_FLIP_Y;
					} else if (mirror == (INT_ORIENTATION_FLIP_X | INT_ORIENTATION_FLIP_Y)) {
						mirror = INT_ORIENTATION_FLIP_X;
					} else {
						mirror = 0;
					}
					flip ^= INT_ORIENTATION_SWAP_XY;
					rs.video_orientation_effective = flip | mirror;
				}
				break;
			case INT_KEY_ESC :
			case INT_KEY_OFF :
				done = true;
				is_terminate = true;
				break;
		}
		switch (key) {
			case INT_KEY_IDLE_0 :
			case INT_KEY_ENTER :
			case INT_KEY_RUN_CLONE :
				if (rs.current_game && rs.current_clone) {
					done = true;
					is_run = true;
				}
				break;
		}
	}

	log_std(("menu: menu stop\n"));

	if (is_terminate) {
		play_foreground_effect_end(rs.sound_foreground_end);
		play_background_effect(rs.sound_background_end, PLAY_PRIORITY_END, false);
	}
	if (is_run) {
		play_foreground_effect_start(rs.sound_foreground_start);
		play_background_effect(rs.sound_background_start, PLAY_PRIORITY_END, false);
	}

	// wait for the sound end
	log_std(("menu: wait foreground stop\n"));
	play_foreground_wait();
	log_std(("menu: background stop\n"));
	play_background_stop(PLAY_PRIORITY_EVENT);
	log_std(("menu: wait background stop\n"));
	play_background_wait();

	log_std(("menu: play_done call\n"));
	play_done();

	log_std(("menu: int_unset call\n"));
	int_unset(is_terminate || rs.video_reset_mode);

	return key;
}

//---------------------------------------------------------------------------
// run_all

int run_all(adv_conf* config_context, config_state& rs)
{
	bool done = false;
	bool is_first = true;
	bool silent = false;
	int key = 0;

	rs.current_game = 0;
	rs.current_clone = 0;
	rs.fast = "";

	while (!done) {
		key = run_main(rs, is_first, silent);

		// the next
		is_first = false;

		// replay the sound and clip
		silent = false;

		switch (key) {
			case INT_KEY_ESC :
			case INT_KEY_OFF :
				done = true;
				break;
			case INT_KEY_ENTER :
			case INT_KEY_IDLE_0 :
			case INT_KEY_RUN_CLONE :
				if (key == INT_KEY_IDLE_0) {
					// don't replay the sound and clip
					silent = true;
				}

				if (!rs.current_clone)
					rs.current_clone = rs.current_game;

				if (rs.current_clone) {
					// save before
					rs.save(config_context);

					// run the game
					rs.current_clone->emulator_get()->run(*rs.current_clone, rs.video_orientation_effective, rs.difficulty_effective, play_attenuation_get(), key == INT_KEY_IDLE_0);

					// update the game info
					rs.current_clone->emulator_get()->update(*rs.current_clone);

					// save after
					rs.save(config_context);
					
					// print the messages
					target_flush();
				}
				break;
		}
	}

	return key;
}

//---------------------------------------------------------------------------
// Version

static void version(void)
{
	char report_buffer[128];
	target_out("AdvanceMENU %s\n\n", VERSION);
	video_report_driver_all(report_buffer, sizeof(report_buffer));
	target_out("Video:%s\n", report_buffer);
	sound_report_driver_all(report_buffer, sizeof(report_buffer));
	target_out("Sound:%s\n", report_buffer);
	keyb_report_driver_all(report_buffer, sizeof(report_buffer));
	target_out("Keyboard:%s\n", report_buffer);
	joystickb_report_driver_all(report_buffer, sizeof(report_buffer));
	target_out("Joystick:%s\n", report_buffer);
	mouseb_report_driver_all(report_buffer, sizeof(report_buffer));
	target_out("Mouse:%s\n", report_buffer);
}

//---------------------------------------------------------------------------
// main

extern "C" void adv_svgalib_log_va(const char *text, va_list arg)
{
	log_va(text, arg);
}

static void error_callback(void* context, enum conf_callback_error error, const char* file, const char* tag, const char* valid, const char* desc, ...)
{
	va_list arg;
	va_start(arg, desc);
	target_err_va(desc, arg);
	target_err("\n");
	if (valid)
		target_err("%s\n", valid);
	va_end(arg);
}

static adv_conf_conv STANDARD[] = {
#ifdef __MSDOS__
{ "", "allegro_*", "*", "%s", "%s", "%s", 1 }, /* auto registration of the Allegro options */
#endif
{ "*", "group_inport", "*", "%s", "group_import", "%s", 0 }, /* 1.16.0 */
{ "*", "type_inport", "*", "%s", "type_import", "%s", 0 }, /* 1.16.0 */
{ "*", "preview_aspect", "fit", "%s", "preview_expand", "3.0", 0 }, /* 1.17.4 */
{ "*", "preview_aspect", "correct", "%s", "preview_expand", "1.15", 0 }, /* 1.17.4 */
/* 2.1.0 */
{ "*", "msg_run", "*", "%s", "run_msg", "%s", 0 }, /* rename */
{ "*", "select_neogeo", "*", "", "", "", 0 }, /* remove */
{ "*", "select_neogeo", "*", "", "", "", 0 }, /* remove */
{ "*", "select_deco", "*", "", "", "", 0 }, /* remove */
{ "*", "select_playchoice", "*", "", "", "", 0 }, /* remove */
{ "*", "select_clone", "*", "", "", "", 0 }, /* remove */
{ "*", "select_bad", "*", "", "", "", 0 }, /* remove */
{ "*", "select_missing", "*", "", "", "", 0 }, /* remove */
{ "*", "select_vector", "*", "", "", "", 0 }, /* remove */
{ "*", "select_vertical", "*", "", "", "", 0 }, /* remove */
{ "*", "type_import", "none", "", "", "", 0 }, /* remove */
{ "*", "group_import", "none", "", "", "", 0 }, /* remove */
{ "*", "desc_import", "none", "", "", "", 0 }, /* remove */
{ "*", "device_joystick", "standard", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "dual", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "4button", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "6button", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "8button", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "fspro", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "wingex", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "sidewinder", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "sidewinderag", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "gamepadpro", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "grip", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "grip4", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "sneslpt1", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "sneslpt2", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "sneslpt3", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "psxlpt1", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "psxlpt2", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "psxlpt3", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "n64lpt1", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "n64lpt2", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "n64lpt3", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "db9lpt1", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "db9lp2", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "db9lp3", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "tgxlpt1", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "tgxlpt2", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "tgxlpt3", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "segaisa", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "segapci", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "segapcifast", "%s", "%s", "allegro/%s", 0 }, /* rename */
{ "*", "device_joystick", "wingwarrior", "%s", "%s", "allegro/%s", 0 }, /* rename */
/* 2.2.0 */
{ "*", "device_svgaline_divide_clock", "*", "%s", "device_svgaline_divideclock", "%s", 0 }, /* rename */
/* 2.2.2 */
{ "*", "video_depth", "*", "", "", "", 0 }, /* remove */
{ "*", "device_sdl_fullscreen", "yes", "%s", "device_video_output", "fullscreen", 0 }, /* rename */
{ "*", "device_sdl_fullscreen", "no", "%s", "device_video_output", "window", 0 }, /* rename */
{ "*", "device_video_8bit", "*", "%s", "device_color_palette8", "%s", 0 }, /* rename */
{ "*", "device_video_15bit", "*", "%s", "device_color_bgr15", "%s", 0 }, /* rename */
{ "*", "device_video_16bit", "*", "%s", "device_color_bgr16", "%s", 0 }, /* rename */
{ "*", "device_video_24bit", "*", "%s", "device_color_bgr24", "%s", 0 }, /* rename */
{ "*", "device_video_32bit", "*", "%s", "device_color_bgr32", "%s", 0 } /* rename */
};

void os_signal(int signum)
{
	os_default_signal(signum);
}

int os_main(int argc, char* argv[])
{
	config_state rs;
	adv_conf* config_context;
	bool opt_verbose;
	bool opt_log;
	bool opt_default;
	bool opt_remove;
	bool opt_logsync;
	bool opt_version;
	int key = 0;
	const char* section_map[1];
	const char* include;

	srand(time(0));

	config_context = conf_init();

	if (os_init(config_context)!=0) {
		target_err("Error initializing the OS support.\n");
		goto err_conf;
	}

	/* include file */
	conf_string_register_default(config_context, "include", "");

	config_state::conf_register(config_context);
	int_reg(config_context);
	play_reg(config_context);

	if (conf_input_args_load(config_context, 2, "", &argc, argv, error_callback, 0) != 0)
		goto err_init;

	opt_verbose = false;
	opt_log = false;
	opt_logsync = false;
	opt_remove = false;
	opt_default = false;
	opt_version = false;
	for(int i=1;i<argc;++i) {
		if (target_option(argv[i], "verbose")) {
			opt_verbose = true;
		} else if (target_option(argv[i], "version")) {
			opt_version = true;
		} else if (target_option(argv[i], "remove")) {
			opt_remove = true;
		} else if (target_option(argv[i], "default")) {
			opt_default = true;
		} else if (target_option(argv[i], "log")) {
			opt_log = true;
		} else if (target_option(argv[i], "logsync")) {
			opt_logsync = true;
		} else {
			target_err("Unknown option '%s'.\n", argv[i]);
			goto err_init;
		}
	}

	if (opt_version) {
		version();
		goto done_init;
	}

	if (opt_log || opt_logsync) {
		remove("advmenu.log");
		if (log_init("advmenu.log", opt_logsync) != 0) {
			target_err("Error opening the log file 'advmenu.log'.\n");
			goto err_init;
		}
	}

	log_std(("menu: %s %s\n", __DATE__, __TIME__));

	if (file_config_file_root("advmenu.rc") != 0) {
		if (conf_input_file_load_adv(config_context, 3, file_config_file_root("advmenu.rc"), 0, 0, 1, STANDARD, sizeof(STANDARD)/sizeof(STANDARD[0]), error_callback, 0) != 0) {
			goto err_init;
		}
	}

	if (conf_input_file_load_adv(config_context, 0, file_config_file_home("advmenu.rc"), file_config_file_home("advmenu.rc"), 0, 1, STANDARD, sizeof(STANDARD)/sizeof(STANDARD[0]), error_callback, 0) != 0)
		goto err_init;

	section_map[0] = "";
	conf_section_set(config_context, section_map, 1);

	if (!(file_config_file_root("advmenu.rc") != 0 && access(file_config_file_root("advmenu.rc"), R_OK)==0)
		&& !(file_config_file_home("advmenu.rc") != 0 && access(file_config_file_home("advmenu.rc"), R_OK)==0)) {
		config_state::conf_default(config_context);
		conf_set_default_if_missing(config_context, "");
		conf_sort(config_context);
		if (conf_save(config_context, 1, error_callback, 0) != 0) {
			goto err_init;
		}
		target_out("Configuration file '%s' created with all the default options.\n", file_config_file_home("advmenu.rc"));
		goto done_init;
	}

	if (opt_default) {
		config_state::conf_default(config_context);
		conf_set_default_if_missing(config_context, "");
		if (conf_save(config_context, 1, error_callback, 0) != 0) {
			goto err_init;
		}
		target_out("Configuration file '%s' updated with all the default options.\n", file_config_file_home("advmenu.rc"));
		goto done_init;
	}

	if (opt_remove) {
		conf_remove_if_default(config_context, "");
		if (conf_save(config_context, 1, error_callback, 0) != 0) {
			goto err_init;
		}
		target_out("Configuration file '%s' updated with all the default options removed.\n", file_config_file_home("advmenu.rc"));
		goto done_init;
	}

	/* setup the include configuration file */
	include = conf_string_get_default(config_context, "include");
	if (include[0]) {
		const char* include_file = file_config_file_home(include);
		log_std(("menu: include file '%s'\n", include_file));
		if (access(include_file, R_OK)!=0) {
			target_err("Error opening the configuration include file '%s'.\n", include_file);
			goto err_init;
		}
		if (conf_input_file_load_adv(config_context, 1, include_file, 0, 0, 1, STANDARD, sizeof(STANDARD)/sizeof(STANDARD[0]), error_callback, 0) != 0) {
			goto err_init;
		}
	} else {
		log_std(("menu: no include file\n"));
	}

	log_std(("menu: *_load()\n"));

	if (!rs.load(config_context, opt_verbose)) {
		goto err_init;
	}
	if (!int_load(config_context)) {
		goto err_init;
	}
	if (!play_load(config_context)) {
		goto err_init;
	}

	if (os_inner_init("AdvanceMENU") != 0) {
		target_err("Error initializing the inner OS support.\n");
		goto err_init;
	}

	if (!int_init(rs.video_size, rs.sound_foreground_key)) {
		goto err_inner_init;
	}
	
	// print the messages after setting the video
	target_flush();

	// set the modifiable data
	rs.restore_load();

	key = run_all(config_context, rs);

	// restore or set the changed data
	if (rs.restore == restore_none) {
		rs.restore_save();
	}

	// print the messages before restoring the video
	target_flush();

	int_done();
	
	// save all the data
	rs.save(config_context);

	if (opt_log || opt_logsync) {
		log_done();
	}

	os_inner_done();
	
done_init:
	int_unreg();
	os_done();

	if (key == INT_KEY_OFF)
		target_apm_shutdown();

	conf_done(config_context);

	return EXIT_SUCCESS;

err_inner_init:
	os_inner_done();

err_init:
	int_unreg();
	os_done();

err_conf:
	conf_done(config_context);
	return EXIT_FAILURE;
}

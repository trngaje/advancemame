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
 *
 * In addition, as a special exception, Andrea Mazzoleni
 * gives permission to link the code of this program with
 * the MAME library (or with modified versions of MAME that use the
 * same license as MAME), and distribute linked combinations including
 * the two.  You must obey the GNU General Public License in all
 * respects for all of the code used other than MAME.  If you modify
 * this file, you may extend this exception to your version of the
 * file, but you are not obligated to do so.  If you do not wish to
 * do so, delete this exception statement from your version.
 */

#include "portable.h"

#include "emu.h"

#include "advance.h"

#include "ui_text.h" // by trngaje

#include <math.h>

/***************************************************************************/
/* Menu */

static int video_mode_menu(struct advance_video_context* context, struct advance_ui_context* ui_context, int selected, unsigned input)
{
	struct ui_menu menu;
	unsigned mac;
	int exit_index;
	int auto_index;
	unsigned i;
	const adv_crtc* crtc;

	if (selected >= 1)
		selected = selected - 1;
	else
		selected = 0;

	advance_ui_menu_init(&menu);

	advance_ui_menu_title_insert(&menu, ui_getstring(UI_video_mode)/*"Video Mode"*/);

	auto_index = advance_ui_menu_text_insert(&menu, "Auto");

	crtc = 0;
	for (i = 0; i < context->state.crtc_mac; ++i) {
		char buffer[128];
		mode_desc_print(context, buffer, sizeof(buffer), context->state.crtc_map[i]);
		if (advance_ui_menu_text_insert(&menu, buffer) == selected)
			crtc = context->state.crtc_map[i];
	}

	//exit_index = advance_ui_menu_text_insert(&menu, "Return to Main Menu");
	exit_index = advance_ui_menu_text_insert(&menu, ui_getstring(UI_returntomain));

	mac = advance_ui_menu_done(&menu, ui_context, selected);

	if (input == OSD_INPUT_DOWN) {
		selected = (selected + 1) % mac;
	}

	if (input == OSD_INPUT_UP) {
		selected = (selected + mac - 1) % mac;
	}

	if (input == OSD_INPUT_SELECT) {
		if (selected == exit_index) {
			selected = -1;
		} else if (selected == auto_index) {
			struct advance_video_config_context config = context->config;

			sncpy(config.resolution_buffer, sizeof(config.resolution_buffer), "auto");

			advance_video_reconfigure(context, &config);

			/* show at screen the new configuration name */
			advance_ui_message(ui_context, "%s", mode_current_name(context));
		} else {
			struct advance_video_config_context config = context->config;

			sncpy(config.resolution_buffer, sizeof(config.resolution_buffer), crtc_name_get(crtc));

			advance_video_reconfigure(context, &config);
		}
	}

	if (input == OSD_INPUT_CANCEL)
		selected = -1;

	if (input == OSD_INPUT_CONFIGURE)
		selected = -2;

	return selected + 1;
}

static int video_pipeline_menu(struct advance_video_context* context, struct advance_ui_context* ui_context, int selected, unsigned input)
{
	struct ui_menu menu;
	unsigned mac;
	int exit_index;
	unsigned i;
	const struct video_stage_horz_struct* stage;
	char buffer[256];

	if (selected >= 1)
		selected = selected - 1;
	else
		selected = 0;

	advance_ui_menu_init(&menu);

	snprintf(buffer, sizeof(buffer), "Video Pipeline (%d)", context->state.blit_pipeline_index);
	advance_ui_menu_title_insert(&menu, buffer);

	for (i = 1, stage = video_pipeline_begin(&context->state.blit_pipeline); stage != video_pipeline_end(&context->state.blit_pipeline); ++stage, ++i) {
		if (stage == video_pipeline_pivot(&context->state.blit_pipeline)) {
			snprintf(buffer, sizeof(buffer), "(%d) %s", i, pipe_name(video_pipeline_vert(&context->state.blit_pipeline)->type));
			advance_ui_menu_text_insert(&menu, buffer);
			++i;
		}
		if (stage->sbpp != stage->sdp)
			snprintf(buffer, sizeof(buffer), "(%d) %s, p %d, dp %d", i, pipe_name(stage->type), stage->sbpp, stage->sdp);
		else
			snprintf(buffer, sizeof(buffer), "(%d) %s, p %d", i, pipe_name(stage->type), stage->sbpp);
		advance_ui_menu_text_insert(&menu, buffer);
	}
	if (stage == video_pipeline_pivot(&context->state.blit_pipeline)) {
		snprintf(buffer, sizeof(buffer), "(%d) %s", i, pipe_name(video_pipeline_vert(&context->state.blit_pipeline)->type));
		advance_ui_menu_text_insert(&menu, buffer);
		++i;
	}

	advance_ui_menu_title_insert(&menu, ui_getstring(UI_pipeline_blit_time)/*"Pipeline Blit Time"*/);

	double timing = adv_measure_median(0.00001, 0.5, context->state.update_timing_map, PIPELINE_MEASURE_MAX);
	snprintf(buffer, sizeof(buffer), "Last render %.2f (ms)", timing * 1000);
	advance_ui_menu_text_insert(&menu, buffer);

	timing = adv_measure_median(0.00001, 0.5, context->state.pipeline_timing_map, PIPELINE_MEASURE_MAX);
	snprintf(buffer, sizeof(buffer), "Last write %.2f (ms)", timing * 1000);
	advance_ui_menu_text_insert(&menu, buffer);

	//exit_index = advance_ui_menu_text_insert(&menu, "Return to Main Menu");
	exit_index = advance_ui_menu_text_insert(&menu, ui_getstring(UI_returntomain));

	mac = advance_ui_menu_done(&menu, ui_context, selected);

	if (input == OSD_INPUT_DOWN) {
		selected = (selected + 1) % mac;
	}

	if (input == OSD_INPUT_UP) {
		selected = (selected + mac - 1) % mac;
	}

	if (input == OSD_INPUT_SELECT) {
		if (selected == exit_index) {
			selected = -1;
		}
	}

	if (input == OSD_INPUT_CANCEL)
		selected = -1;

	if (input == OSD_INPUT_CONFIGURE)
		selected = -2;

	return selected + 1;
}

int osd2_video_menu(int selected, unsigned input)
{
	struct advance_video_context* video_context = &CONTEXT.video;
	struct advance_global_context* global_context = &CONTEXT.global;
	struct advance_ui_context* ui_context = &CONTEXT.ui;

	struct ui_menu menu;
	unsigned mac;
	int resolution_index;
	int stretch_index;
	int vsync_index;
	int combine_index;
	int effect_index;
	int save_game_index;
	int save_resolution_index;
	int save_resolutionclock_index;
	int pipeline_index;
	int magnify_index;
	int index_index;
	int smp_index;
	int crash_index;
	int exit_index;
	char buffer[128];
	char freq[32];
	const char* text = 0;
	const char* option = 0;

	/* the menu data is not duplicated for the thread, so we have to wait for its completion */
	advance_video_thread_wait(video_context);

	if (selected >= 1)
		selected = selected - 1;
	else
		selected = 0;

	if (video_context->state.menu_sub_flag) {
		int ret = 0;
		switch (video_context->state.menu_sub_flag) {
		case 1: ret = video_mode_menu(video_context, ui_context, video_context->state.menu_sub_selected, input); break;
		case 2: ret = video_pipeline_menu(video_context, ui_context, video_context->state.menu_sub_selected, input); break;
		}
		switch (ret) {
		case -1: return -1;  /* hide interface */
		case 0: video_context->state.menu_sub_flag = 0; video_context->state.menu_sub_selected = 1; break;  /* close submenu */
		default: video_context->state.menu_sub_selected = ret; break;
		}
		return selected + 1;
	}

	advance_ui_menu_init(&menu);

	advance_ui_menu_title_insert(&menu, ui_getstring(UI_video_menu)/*"Video Menu"*/);

	snprintf(buffer, sizeof(buffer), "%dx%dx%d %.1f/%.1f/%.1f",
		video_size_x(),
		video_size_y(),
		video_bits_per_pixel(),
		(double)crtc_pclock_get(&video_context->state.crtc_effective) / 1E6,
		(double)crtc_hclock_get(&video_context->state.crtc_effective) / 1E3,
		(double)crtc_vclock_get(&video_context->state.crtc_effective)
	);
	advance_ui_menu_title_insert(&menu, buffer);

	resolution_index = advance_ui_menu_option_insert(&menu, ui_getstring(UI_mode)/*"Mode..."*/, video_context->config.resolution_buffer);

	if (!video_context->state.game_vector_flag) {
		if (strcmp(video_context->config.resolution_buffer, "auto") == 0) {
			switch (mode_current_magnify(video_context)) {
			default:
			case 1: text = ui_getstring(UI_magnify_1)/*"Magnify [1]"*/; break;
			case 2: text = ui_getstring(UI_magnify_2)/*"Magnify [2]"*/; break;
			case 3: text = ui_getstring(UI_magnify_3)/*"Magnify [3]"*/; break;
			case 4: text = ui_getstring(UI_magnify_4)/*"Magnify [4]"*/; break;
			}
			switch (video_context->config.magnify_factor) {
			default:
			case 0: option = "auto"; break;
			case 1: option = "1"; break;
			case 2: option = "2"; break;
			case 3: option = "3"; break;
			case 4: option = "4"; break;
			}
			magnify_index = advance_ui_menu_option_insert(&menu, text, option);
		} else {
			magnify_index = -1;
		}

		advance_ui_menu_title_insert(&menu, ui_getstring(UI_options)/*"Options"*/);

		switch (mode_current_stretch(video_context)) {
		case STRETCH_NONE: text = ui_getstring(UI_resize_no)/*"Resize [no]"*/; break;
		case STRETCH_INTEGER_XY: text = ui_getstring(UI_resize_integer)/*"Resize [integer]"*/; break;
		case STRETCH_INTEGER_X_FRACTIONAL_Y: text = ui_getstring(UI_resize_mixed)/*"Resize [mixed]"*/; break;
		case STRETCH_FRACTIONAL_XY: text = ui_getstring(UI_resize_fractional)/*"Resize [fractional]"*/; break;
		}
		switch (video_context->config.stretch) {
		case STRETCH_NONE: option = "no"; break;
		case STRETCH_INTEGER_XY: option = "integer"; break;
		case STRETCH_INTEGER_X_FRACTIONAL_Y: option = "mixed"; break;
		case STRETCH_FRACTIONAL_XY: option = "fractional"; break;
		}
		stretch_index = advance_ui_menu_option_insert(&menu, text, option);
	} else {
		magnify_index = -1;

		advance_ui_menu_title_insert(&menu, "Options");

		stretch_index = -1;
	}

	switch (video_index()) {
	case MODE_FLAGS_INDEX_PALETTE8:
		text = ui_getstring(UI_color_palette8)/*"Color [palette8]"*/;
		break;
	case MODE_FLAGS_INDEX_BGR8:
		text = ui_getstring(UI_color_bgr8)/*"Color [bgr8]"*/;
		break;
	case MODE_FLAGS_INDEX_BGR15:
		text = ui_getstring(UI_color_bgr15)/*"Color [bgr15]"*/;
		break;
	case MODE_FLAGS_INDEX_BGR16:
		text = ui_getstring(UI_color_bgr16)/*"Color [bgr16]"*/;
		break;
	case MODE_FLAGS_INDEX_BGR32:
		text = ui_getstring(UI_color_bgr32)/*"Color [bgr32]"*/;
		break;
	case MODE_FLAGS_INDEX_YUY2:
		text = ui_getstring(UI_color_yuy2)/*"Color [yuy2]"*/;
		break;
	default:
		text = ui_getstring(UI_color_unknown)/*"Color [unknown]"*/;
		break;
	}
	switch (video_context->config.index) {
	case MODE_FLAGS_INDEX_NONE:
		option = "auto";
		break;
	case MODE_FLAGS_INDEX_PALETTE8:
		option = "palette8";
		break;
	case MODE_FLAGS_INDEX_BGR8:
		option = "bgr8";
		break;
	case MODE_FLAGS_INDEX_BGR15:
		option = "bgr15";
		break;
	case MODE_FLAGS_INDEX_BGR16:
		option = "bgr16";
		break;
	case MODE_FLAGS_INDEX_BGR32:
		option = "bgr32";
		break;
	case MODE_FLAGS_INDEX_YUY2:
		option = "yuy2";
		break;
	default:
		option = "unknown";
		break;
	}
	index_index = advance_ui_menu_option_insert(&menu, text, option);

	switch (video_context->state.combine) {
	case COMBINE_NONE: text = ui_getstring(UI_resize_effect_no)/*"Resize Effect [no]"*/; break;
	case COMBINE_MAXMIN: text = ui_getstring(UI_resize_effect_max)/*"Resize Effect [max]"*/; break;
	case COMBINE_MEAN: text = ui_getstring(UI_resize_effect_mean)/*"Resize Effect [mean]"*/; break;
	case COMBINE_FILTER: text = ui_getstring(UI_resize_effect_filter)/*"Resize Effect [filter]"*/; break;
	case COMBINE_SCALEX: text = ui_getstring(UI_resize_effect_scalex)/*"Resize Effect [scalex]"*/; break;
	case COMBINE_SCALEK: text = ui_getstring(UI_resize_effect_scalek)/*"Resize Effect [scalek]"*/; break;
	case COMBINE_HQ: text = ui_getstring(UI_resize_effect_hq)/*"Resize Effect [hq]"*/; break;
	case COMBINE_XBR: text = ui_getstring(UI_resize_effect_xbr)/*"Resize Effect [xbr]"*/; break;
	}
	switch (video_context->config.combine) {
	case COMBINE_AUTO: option = "auto"; break;
	case COMBINE_NONE: option = "no"; break;
	case COMBINE_MAXMIN: option = "max"; break;
	case COMBINE_MEAN: option = "mean"; break;
	case COMBINE_FILTER: option = "filter"; break;
	case COMBINE_SCALEX: option = "scalex"; break;
	case COMBINE_SCALEK: option = "scalek"; break;
	case COMBINE_HQ: option = "hq"; break;
	case COMBINE_XBR: option = "xbr"; break;
	}
	combine_index = advance_ui_menu_option_insert(&menu, text, option);

	switch (video_context->state.rgb_effect) {
	case EFFECT_NONE: text = ui_getstring(UI_rgb_effect_no)/*"Rgb Effect [no]"*/; break;
	case EFFECT_RGB_TRIAD3PIX: text = ui_getstring(UI_rgb_effect_triad3dot)/*"Rgb Effect [triad3dot]"*/; break;
	case EFFECT_RGB_TRIAD6PIX: text = ui_getstring(UI_rgb_effect_triad6dot)/*Rgb Effect [triad6dot]"*/; break;
	case EFFECT_RGB_TRIAD16PIX: text = ui_getstring(UI_rgb_effect_triad16dot)/*"Rgb Effect [triad16dot]"*/; break;
	case EFFECT_RGB_TRIADSTRONG3PIX: text = ui_getstring(UI_rgb_effect_triadstrong3dot)/*"Rgb Effect [triadstrong3dot]"*/; break;
	case EFFECT_RGB_TRIADSTRONG6PIX: text = ui_getstring(UI_rgb_effect_triadstrong6dot)/*"Rgb Effect [triadstrong6dot]"*/; break;
	case EFFECT_RGB_TRIADSTRONG16PIX: text = ui_getstring(UI_rgb_effect_triadstrong16dot)/*"Rgb Effect [triadstrong16dot]"*/; break;
	case EFFECT_RGB_SCANDOUBLEHORZ: text = ui_getstring(UI_rgb_effect_scan2horz)/*"Rgb Effect [scan2horz]"*/; break;
	case EFFECT_RGB_SCANTRIPLEHORZ: text = ui_getstring(UI_rgb_effect_scan3horz)/*"Rgb Effect [scan3horz]"*/; break;
	case EFFECT_RGB_SCANDOUBLEVERT: text = ui_getstring(UI_rgb_effect_scan2vert)/*"Rgb Effect [scan2vert]"*/; break;
	case EFFECT_RGB_SCANTRIPLEVERT: text = ui_getstring(UI_rgb_effect_scan3vert)/*"Rgb Effect [scan3vert]"*/; break;
	}
	switch (video_context->config.rgb_effect) {
	case EFFECT_NONE: option = "no"; break;
	case EFFECT_RGB_TRIAD3PIX: option = "triad3dot"; break;
	case EFFECT_RGB_TRIAD6PIX: option = "triad6dot"; break;
	case EFFECT_RGB_TRIAD16PIX: option = "triad16dot"; break;
	case EFFECT_RGB_TRIADSTRONG3PIX: option = "triadstrong3dot"; break;
	case EFFECT_RGB_TRIADSTRONG6PIX: option = "triadstrong6dot"; break;
	case EFFECT_RGB_TRIADSTRONG16PIX: option = "triadstrong16dot"; break;
	case EFFECT_RGB_SCANDOUBLEHORZ: option = "scan2horz"; break;
	case EFFECT_RGB_SCANTRIPLEHORZ: option = "scan3horz"; break;
	case EFFECT_RGB_SCANDOUBLEVERT: option = "scan2vert"; break;
	case EFFECT_RGB_SCANTRIPLEVERT: option = "scan3vert"; break;
	}
	effect_index = advance_ui_menu_option_insert(&menu, text, option);

	switch (video_context->state.vsync_flag) {
	case 0: text = "Vsync [no]"; break;
	case 1:
		snprintf(freq, sizeof(freq), "Vsync [%.1f]", video_measured_vclock());
		text = freq;
		break;
	}
	switch (video_context->config.vsync_flag) {
	case 0: option = "no"; break;
	case 1: option = "yes"; break;
	}
	vsync_index = advance_ui_menu_option_insert(&menu, text, option);

#ifdef USE_SMP
	switch (video_context->config.smp_flag) {
	case 0: text = "SMP [no]"; break;
	case 1: text = "SMP [yes]"; break;
	}
	switch (video_context->config.smp_flag) {
	case 0: option = "no"; break;
	case 1: option = "yes"; break;
	}
	smp_index = advance_ui_menu_option_insert(&menu, text, option);
#else
	smp_index = -1;
#endif

	pipeline_index = advance_ui_menu_text_insert(&menu, ui_getstring(UI_details)/*"Details..."*/);

	if (global_context->state.is_config_writable) {
		save_game_index = advance_ui_menu_text_insert(&menu, ui_getstring(UI_save_for_this_game)/*"Save for this game"*/);

		if (!video_context->state.game_vector_flag) {
			save_resolutionclock_index = advance_ui_menu_text_insert(&menu, ui_getstring(UI_save_for_this_game_size_freq)/*"Save for this game size/freq"*/);
		} else {
			save_resolutionclock_index = -1;
		}

		if (video_context->state.game_vector_flag)
			text = ui_getstring(UI_save_for_all_vector_games)/*"Save for all vector games"*/;
		else
			text = ui_getstring(UI_save_for_this_game_size)/*"Save for this game size"*/;
		save_resolution_index = advance_ui_menu_text_insert(&menu, text);
	} else {
		save_game_index = -1;
		save_resolution_index = -1;
		save_resolutionclock_index = -1;
	}

	if (video_context->config.crash_flag) {
		crash_index = advance_ui_menu_text_insert(&menu, "Crash");
	} else {
		crash_index = -1;
	}

	//exit_index = advance_ui_menu_text_insert(&menu, "Return to Main Menu");
	exit_index = advance_ui_menu_text_insert(&menu, ui_getstring(UI_returntomain));

	mac = advance_ui_menu_done(&menu, ui_context, selected);

	if (input == OSD_INPUT_DOWN) {
		selected = (selected + 1) % mac;
	}

	if (input == OSD_INPUT_UP) {
		selected = (selected + mac - 1) % mac;
	}

	if (input == OSD_INPUT_SELECT) {
		if (selected == exit_index) {
			selected = -1;
		} else if (selected == resolution_index) {
			video_context->state.menu_sub_flag = 1;
		} else if (selected == pipeline_index) {
			video_context->state.menu_sub_flag = 2;
		} else if (selected == save_game_index) {
			advance_video_config_save(video_context, video_context->config.section_name_buffer);
		} else if (selected == save_resolution_index) {
			advance_video_config_save(video_context, video_context->config.section_resolution_buffer);
		} else if (selected == save_resolutionclock_index) {
			advance_video_config_save(video_context, video_context->config.section_resolutionclock_buffer);
		} else if (selected == crash_index) {
			target_crash();
		}
	}

	if (input == OSD_INPUT_RIGHT) {
		if (selected == combine_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.combine) {
			case COMBINE_AUTO: config.combine = COMBINE_NONE; break;
			case COMBINE_NONE: config.combine = COMBINE_MAXMIN; break;
			case COMBINE_MAXMIN: config.combine = COMBINE_MEAN; break;
			case COMBINE_MEAN: config.combine = COMBINE_FILTER; break;
			case COMBINE_FILTER: config.combine = COMBINE_SCALEX; break;
			case COMBINE_SCALEX: config.combine = COMBINE_SCALEK; break;
			case COMBINE_SCALEK: config.combine = COMBINE_HQ; break;
			case COMBINE_HQ: config.combine = COMBINE_XBR; break;
			case COMBINE_XBR: config.combine = COMBINE_AUTO; break;
			}
			advance_video_reconfigure(video_context, &config);
		} else if (selected == effect_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.rgb_effect) {
			case EFFECT_NONE: config.rgb_effect = EFFECT_RGB_TRIAD3PIX; break;
			case EFFECT_RGB_TRIAD3PIX: config.rgb_effect = EFFECT_RGB_TRIADSTRONG3PIX; break;
			case EFFECT_RGB_TRIADSTRONG3PIX: config.rgb_effect = EFFECT_RGB_TRIAD6PIX; break;
			case EFFECT_RGB_TRIAD6PIX: config.rgb_effect = EFFECT_RGB_TRIADSTRONG6PIX; break;
			case EFFECT_RGB_TRIADSTRONG6PIX: config.rgb_effect = EFFECT_RGB_TRIAD16PIX; break;
			case EFFECT_RGB_TRIAD16PIX: config.rgb_effect = EFFECT_RGB_TRIADSTRONG16PIX; break;
			case EFFECT_RGB_TRIADSTRONG16PIX: config.rgb_effect = EFFECT_RGB_SCANDOUBLEHORZ; break;
			case EFFECT_RGB_SCANDOUBLEHORZ: config.rgb_effect = EFFECT_RGB_SCANTRIPLEHORZ; break;
			case EFFECT_RGB_SCANTRIPLEHORZ: config.rgb_effect = EFFECT_RGB_SCANDOUBLEVERT; break;
			case EFFECT_RGB_SCANDOUBLEVERT: config.rgb_effect = EFFECT_RGB_SCANTRIPLEVERT; break;
			case EFFECT_RGB_SCANTRIPLEVERT: config.rgb_effect = EFFECT_NONE; break;
			}
			advance_video_reconfigure(video_context, &config);
		} else if (selected == vsync_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.vsync_flag) {
			case 0: config.vsync_flag = 1; break;
			case 1: config.vsync_flag = 0; break;
			}
			advance_video_reconfigure(video_context, &config);
		} else if (selected == smp_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.smp_flag) {
			case 0: config.smp_flag = 1; break;
			case 1: config.smp_flag = 0; break;
			}
			advance_video_reconfigure(video_context, &config);
		} else if (selected == magnify_index) {
			struct advance_video_config_context config = video_context->config;
			if (config.magnify_factor == 4)
				config.magnify_factor = 0;
			else
				config.magnify_factor += 1;
			advance_video_reconfigure(video_context, &config);
		} else if (selected == index_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.index) {
			case MODE_FLAGS_INDEX_NONE: config.index = MODE_FLAGS_INDEX_PALETTE8; break;
			case MODE_FLAGS_INDEX_PALETTE8: config.index = MODE_FLAGS_INDEX_BGR8; break;
			case MODE_FLAGS_INDEX_BGR8: config.index = MODE_FLAGS_INDEX_BGR15; break;
			case MODE_FLAGS_INDEX_BGR15: config.index = MODE_FLAGS_INDEX_BGR16; break;
			case MODE_FLAGS_INDEX_BGR16: config.index = MODE_FLAGS_INDEX_BGR32; break;
			case MODE_FLAGS_INDEX_BGR32: config.index = MODE_FLAGS_INDEX_YUY2; break;
			case MODE_FLAGS_INDEX_YUY2: config.index = MODE_FLAGS_INDEX_NONE; break;
			}
			advance_video_reconfigure(video_context, &config);
		} else if (selected == stretch_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.stretch) {
			case STRETCH_NONE: config.stretch = STRETCH_FRACTIONAL_XY; break;
			case STRETCH_INTEGER_XY: config.stretch = STRETCH_NONE; break;
			case STRETCH_INTEGER_X_FRACTIONAL_Y: config.stretch = STRETCH_INTEGER_XY; break;
			case STRETCH_FRACTIONAL_XY: config.stretch = STRETCH_INTEGER_X_FRACTIONAL_Y; break;
			}
			advance_video_reconfigure(video_context, &config);
		}
	}

	if (input == OSD_INPUT_LEFT) {
		if (selected == combine_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.combine) {
			case COMBINE_AUTO: config.combine = COMBINE_XBR; break;
			case COMBINE_NONE: config.combine = COMBINE_AUTO; break;
			case COMBINE_MAXMIN: config.combine = COMBINE_NONE; break;
			case COMBINE_MEAN: config.combine = COMBINE_MAXMIN; break;
			case COMBINE_FILTER: config.combine = COMBINE_MEAN; break;
			case COMBINE_SCALEX: config.combine = COMBINE_FILTER; break;
			case COMBINE_SCALEK: config.combine = COMBINE_SCALEX; break;
			case COMBINE_HQ: config.combine = COMBINE_SCALEK; break;
			case COMBINE_XBR: config.combine = COMBINE_HQ; break;
			}
			advance_video_reconfigure(video_context, &config);
		} else if (selected == effect_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.rgb_effect) {
			case EFFECT_NONE: config.rgb_effect = EFFECT_RGB_SCANTRIPLEVERT; break;
			case EFFECT_RGB_TRIAD3PIX: config.rgb_effect = EFFECT_NONE; break;
			case EFFECT_RGB_TRIADSTRONG3PIX: config.rgb_effect = EFFECT_RGB_TRIAD3PIX; break;
			case EFFECT_RGB_TRIAD6PIX: config.rgb_effect = EFFECT_RGB_TRIADSTRONG3PIX; break;
			case EFFECT_RGB_TRIADSTRONG6PIX: config.rgb_effect = EFFECT_RGB_TRIAD6PIX; break;
			case EFFECT_RGB_TRIAD16PIX: config.rgb_effect = EFFECT_RGB_TRIADSTRONG6PIX; break;
			case EFFECT_RGB_TRIADSTRONG16PIX: config.rgb_effect = EFFECT_RGB_TRIAD16PIX; break;
			case EFFECT_RGB_SCANDOUBLEHORZ: config.rgb_effect = EFFECT_RGB_TRIADSTRONG16PIX; break;
			case EFFECT_RGB_SCANTRIPLEHORZ: config.rgb_effect = EFFECT_RGB_SCANDOUBLEHORZ; break;
			case EFFECT_RGB_SCANDOUBLEVERT: config.rgb_effect = EFFECT_RGB_SCANTRIPLEHORZ; break;
			case EFFECT_RGB_SCANTRIPLEVERT: config.rgb_effect = EFFECT_RGB_SCANDOUBLEVERT; break;
			}
			advance_video_reconfigure(video_context, &config);
		} else if (selected == vsync_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.vsync_flag) {
			case 0: config.vsync_flag = 1; break;
			case 1: config.vsync_flag = 0; break;
			}
			advance_video_reconfigure(video_context, &config);
		} else if (selected == smp_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.smp_flag) {
			case 0: config.smp_flag = 1; break;
			case 1: config.smp_flag = 0;  break;
			}
			advance_video_reconfigure(video_context, &config);
		} else if (selected == magnify_index) {
			struct advance_video_config_context config = video_context->config;
			if (config.magnify_factor == 0)
				config.magnify_factor = 4;
			else
				config.magnify_factor -= 1;
			advance_video_reconfigure(video_context, &config);
		} else if (selected == index_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.index) {
			case MODE_FLAGS_INDEX_NONE: config.index = MODE_FLAGS_INDEX_YUY2; break;
			case MODE_FLAGS_INDEX_PALETTE8: config.index = MODE_FLAGS_INDEX_NONE; break;
			case MODE_FLAGS_INDEX_BGR8: config.index = MODE_FLAGS_INDEX_PALETTE8; break;
			case MODE_FLAGS_INDEX_BGR15: config.index = MODE_FLAGS_INDEX_BGR8; break;
			case MODE_FLAGS_INDEX_BGR16: config.index = MODE_FLAGS_INDEX_BGR15; break;
			case MODE_FLAGS_INDEX_BGR32: config.index = MODE_FLAGS_INDEX_BGR16; break;
			case MODE_FLAGS_INDEX_YUY2: config.index = MODE_FLAGS_INDEX_BGR32; break;
			}
			advance_video_reconfigure(video_context, &config);
		} else if (selected == stretch_index) {
			struct advance_video_config_context config = video_context->config;
			switch (config.stretch) {
			case STRETCH_NONE: config.stretch = STRETCH_INTEGER_XY; break;
			case STRETCH_INTEGER_XY: config.stretch = STRETCH_INTEGER_X_FRACTIONAL_Y; break;
			case STRETCH_INTEGER_X_FRACTIONAL_Y: config.stretch = STRETCH_FRACTIONAL_XY; break;
			case STRETCH_FRACTIONAL_XY: config.stretch = STRETCH_NONE; break;
			}
			advance_video_reconfigure(video_context, &config);
		}
	}

	if (input == OSD_INPUT_CANCEL)
		selected = -1;

	if (input == OSD_INPUT_CONFIGURE)
		selected = -2;

	return selected + 1;
}

static int audio_pipeline_menu(struct advance_sound_context* sound_context, struct advance_ui_context* ui_context, int selected, unsigned input)
{
	struct ui_menu menu;
	unsigned mac;
	int exit_index;
	char buffer[128];
	unsigned i;

	if (selected >= 1)
		selected = selected - 1;
	else
		selected = 0;

	i = 1;

	advance_ui_menu_init(&menu);

	advance_ui_menu_title_insert(&menu, ui_getstring(UI_audio_pipeline)/*"Audio Pipeline"*/);

	if (sound_context->config.normalize_flag) {
		snprintf(buffer, sizeof(buffer), "(%d) normalize", i);
		advance_ui_menu_text_insert(&menu, buffer);
		++i;
	}

	if (sound_context->state.sample_mult != 0) {
		int preamp = 20 * log10((double)sound_context->state.sample_mult / SAMPLE_MULT_BASE);
		snprintf(buffer, sizeof(buffer), "(%d) preamp %d (dB)", i, preamp);
		advance_ui_menu_text_insert(&menu, buffer);
		++i;
	}

	if (sound_context->state.equalizer_flag) {
		snprintf(buffer, sizeof(buffer), "(%d) equalizer", i);
		advance_ui_menu_text_insert(&menu, buffer);
		++i;
	}

	if (sound_context->state.input_mode != sound_context->state.output_mode) {
		const char* from;
		const char* to;

		switch (sound_context->state.input_mode) {
		case SOUND_MODE_MONO: from = "mono"; break;
		case SOUND_MODE_STEREO: from = "stereo"; break;
		case SOUND_MODE_SURROUND: from = "surround"; break;
		default: from = "unknown"; break;
		}
		switch (sound_context->state.output_mode) {
		case SOUND_MODE_MONO: to = "mono"; break;
		case SOUND_MODE_STEREO: to = "stereo"; break;
		case SOUND_MODE_SURROUND: to = "surround"; break;
		default: to = "unknown"; break;
		}

		snprintf(buffer, sizeof(buffer), "(%d) %s to %s", i, from, to);
		advance_ui_menu_text_insert(&menu, buffer);
		++i;
	}

	snprintf(buffer, sizeof(buffer), "(%d) write", i);
	advance_ui_menu_text_insert(&menu, buffer);
	++i;

	advance_ui_menu_title_insert(&menu, "Play Time");

	snprintf(buffer, sizeof(buffer), "Write %.2f (ms)", sound_context->state.time * 1000);
	advance_ui_menu_text_insert(&menu, buffer);

	advance_ui_menu_title_insert(&menu, "Play Overflow");

	snprintf(buffer, sizeof(buffer), "Overflow %d (samples)", sound_context->state.overflow);
	advance_ui_menu_text_insert(&menu, buffer);

	advance_ui_menu_title_insert(&menu, "Spectrum (dB/Hz)");

	advance_ui_menu_dft_insert(&menu, sound_context->state.dft_post_X, sound_context->state.dft_padded_size, sound_context->config.eql_cut1, sound_context->config.eql_cut2);

	//exit_index = advance_ui_menu_text_insert(&menu, "Return to Main Menu");
	exit_index = advance_ui_menu_text_insert(&menu, ui_getstring(UI_returntomain));

	mac = advance_ui_menu_done(&menu, ui_context, selected);

	if (input == OSD_INPUT_DOWN) {
		selected = (selected + 1) % mac;
	}

	if (input == OSD_INPUT_UP) {
		selected = (selected + mac - 1) % mac;
	}

	if (input == OSD_INPUT_SELECT) {
		if (selected == exit_index) {
			selected = -1;
		}
	}

	if (input == OSD_INPUT_CANCEL)
		selected = -1;

	if (input == OSD_INPUT_CONFIGURE)
		selected = -2;

	return selected + 1;
}

int osd2_audio_menu(int selected, unsigned input)
{
	struct advance_video_context* video_context = &CONTEXT.video;
	struct advance_sound_context* sound_context = &CONTEXT.sound;
	struct advance_global_context* global_context = &CONTEXT.global;
	struct advance_ui_context* ui_context = &CONTEXT.ui;

	struct ui_menu menu;
	unsigned mac;
	int mode_index;
	int adjust_index;
	int volume_index;
	int normalize_index;
	int eql_index;
	int eqm_index;
	int eqh_index;
	int save_game_index;
	int pipeline_index;
	int exit_index;
	char buffer[128];
	const char* text = 0;
	const char* option = 0;

	/* the menu data is not duplicated for the thread, so we have to wait for its completion */
	advance_video_thread_wait(video_context);

	if (selected >= 1)
		selected = selected - 1;
	else
		selected = 0;

	if (sound_context->state.menu_sub_flag) {
		int ret = 0;
		switch (sound_context->state.menu_sub_flag) {
		case 1: ret = audio_pipeline_menu(sound_context, ui_context, sound_context->state.menu_sub_selected, input); break;
		}
		switch (ret) {
		case -1: return -1;  /* hide interface */
		case 0: sound_context->state.menu_sub_flag = 0; sound_context->state.menu_sub_selected = 1; break;  /* close submenu */
		default: sound_context->state.menu_sub_selected = ret; break;
		}
		return selected + 1;
	}

	advance_ui_menu_init(&menu);

	advance_ui_menu_title_insert(&menu, ui_getstring(UI_audio_menu)/*"Audio Menu"*/);

	switch (sound_context->state.output_mode) {
	case SOUND_MODE_MONO: text = ui_getstring(UI_mode_mono)/*"Mode [mono]"*/; break;
	case SOUND_MODE_STEREO: text = ui_getstring(UI_mode_stereo)/*"Mode [stereo]"*/; break;
	case SOUND_MODE_SURROUND: text = ui_getstring(UI_mode_surround)/*"Mode [surround]"*/; break;
	}
	switch (sound_context->config.mode) {
	case SOUND_MODE_AUTO: option = "auto"; break;
	case SOUND_MODE_MONO: option = "mono"; break;
	case SOUND_MODE_STEREO: option = "stereo"; break;
	case SOUND_MODE_SURROUND: option = "surround"; break;
	}
	mode_index = advance_ui_menu_option_insert(&menu, text, option);

	if (sound_context->config.attenuation > -40)
		snprintf(buffer, sizeof(buffer), "%d", sound_context->config.attenuation);
	else
		sncpy(buffer, sizeof(buffer), "mute");
	volume_index = advance_ui_menu_option_insert(&menu, ui_getstring(UI_attenuation)/*"Attenuation (dB)"*/, buffer);

	advance_ui_menu_title_insert(&menu, "Normalize");

	if (sound_context->config.normalize_flag)
		option = "yes";
	else
		option = "no";
	normalize_index = advance_ui_menu_option_insert(&menu, ui_getstring(UI_auto_normalize)/*"Auto normalize"*/, option);

	snprintf(buffer, sizeof(buffer), "%d", sound_context->config.adjust);
	adjust_index = advance_ui_menu_option_insert(&menu, ui_getstring(UI_amplifier)/*"Amplifier (dB)"*/, buffer);

	advance_ui_menu_title_insert(&menu, ui_getstring(UI_equalizer)/*"Equalizer"*/);

	if (sound_context->config.equalizer_low > -40)
		snprintf(buffer, sizeof(buffer), "%d", sound_context->config.equalizer_low);
	else
		sncpy(buffer, sizeof(buffer), "mute");
	eql_index = advance_ui_menu_option_insert(&menu, ui_getstring(UI_low_freq)/*"Low Freq (dB)"*/, buffer);

	if (sound_context->config.equalizer_mid > -40)
		snprintf(buffer, sizeof(buffer), "%d", sound_context->config.equalizer_mid);
	else
		sncpy(buffer, sizeof(buffer), "mute");
	eqm_index = advance_ui_menu_option_insert(&menu, ui_getstring(UI_mid_freq)/*"Mid Freq (dB)"*/, buffer);

	if (sound_context->config.equalizer_high > -40)
		snprintf(buffer, sizeof(buffer), "%d", sound_context->config.equalizer_high);
	else
		sncpy(buffer, sizeof(buffer), "mute");
	eqh_index = advance_ui_menu_option_insert(&menu, ui_getstring(UI_high_freq)/*"High Freq (dB)"*/, buffer);

	pipeline_index = advance_ui_menu_text_insert(&menu, ui_getstring(UI_details)/*"Details..."*/);

	if (global_context->state.is_config_writable) {
		save_game_index = advance_ui_menu_text_insert(&menu, ui_getstring(UI_save_for_this_game)/*"Save for this game"*/);
	} else {
		save_game_index = -1;
	}

	//exit_index = advance_ui_menu_text_insert(&menu, "Return to Main Menu");
	exit_index = advance_ui_menu_text_insert(&menu, ui_getstring(UI_returntomain));

	mac = advance_ui_menu_done(&menu, ui_context, selected);

	if (input == OSD_INPUT_DOWN) {
		selected = (selected + 1) % mac;
	}

	if (input == OSD_INPUT_UP) {
		selected = (selected + mac - 1) % mac;
	}

	if (input == OSD_INPUT_SELECT) {
		if (selected == exit_index) {
			selected = -1;
		} else if (selected == pipeline_index) {
			sound_context->state.menu_sub_flag = 1;
		} else if (selected == save_game_index) {
			advance_sound_config_save(sound_context, video_context->config.section_name_buffer);
		}
	}

	if (input == OSD_INPUT_RIGHT) {
		if (selected == volume_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.attenuation = sound_context->config.attenuation + 1;
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == adjust_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.adjust = sound_context->config.adjust + 1;
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == normalize_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.normalize_flag = !sound_context->config.normalize_flag;
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == mode_index) {
			struct advance_sound_config_context config = sound_context->config;
			switch (config.mode) {
			case SOUND_MODE_AUTO: config.mode = SOUND_MODE_MONO; break;
			case SOUND_MODE_MONO: config.mode = SOUND_MODE_STEREO; break;
			case SOUND_MODE_STEREO: config.mode = SOUND_MODE_SURROUND; break;
			case SOUND_MODE_SURROUND: config.mode = SOUND_MODE_AUTO; break;
			}
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == eql_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.equalizer_low = sound_context->config.equalizer_low + 2;
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == eqm_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.equalizer_mid = sound_context->config.equalizer_mid + 2;
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == eqh_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.equalizer_high = sound_context->config.equalizer_high + 2;
			advance_sound_reconfigure(sound_context, &config);
		}
	}

	if (input == OSD_INPUT_LEFT) {
		if (selected == volume_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.attenuation = sound_context->config.attenuation - 1;
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == adjust_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.adjust = sound_context->config.adjust - 1;
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == normalize_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.normalize_flag = !sound_context->config.normalize_flag;
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == mode_index) {
			struct advance_sound_config_context config = sound_context->config;
			switch (config.mode) {
			case SOUND_MODE_AUTO: config.mode = SOUND_MODE_SURROUND; break;
			case SOUND_MODE_MONO: config.mode = SOUND_MODE_AUTO; break;
			case SOUND_MODE_STEREO: config.mode = SOUND_MODE_MONO; break;
			case SOUND_MODE_SURROUND: config.mode = SOUND_MODE_STEREO; break;
			}
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == eql_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.equalizer_low = sound_context->config.equalizer_low - 2;
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == eqm_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.equalizer_mid = sound_context->config.equalizer_mid - 2;
			advance_sound_reconfigure(sound_context, &config);
		} else if (selected == eqh_index) {
			struct advance_sound_config_context config = sound_context->config;
			config.equalizer_high = sound_context->config.equalizer_high - 2;
			advance_sound_reconfigure(sound_context, &config);
		}
	}

	if (input == OSD_INPUT_CANCEL)
		selected = -1;

	if (input == OSD_INPUT_CONFIGURE)
		selected = -2;

	return selected + 1;
}


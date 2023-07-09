/***************************************************************************

    ui_text.h

    Functions used to retrieve text used by MAME, to aid in
    translation.

    Copyright (c) 1996-2006, Nicola Salmoria and the MAME Team.
    Visit http://mamedev.org for licensing and usage restrictions.

***************************************************************************/

#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include <stdint.h>

/* Important: this must match the default_text list in ui_text.c! */
enum
{
	UI_select_command = 0,
	UI_continue,
	UI_Exit,
	UI_Shutdown,
	
	UI_select_sort_mode,
	UI_parent,
	UI_emulator,
	UI_name,
	UI_time_played,
	UI_smart_time,
	UI_play_times,
	UI_time_per_play,
	UI_group,
	UI_type,
	UI_manufacturer,
	UI_year,
	UI_size,
	UI_resolution,
	UI_info,
	
	UI_select_list_mode,
	UI_select_preview_mode,
	UI_include_groups,
	UI_include_emulators,
	UI_include_types,
	
	UI_In_the_game_menu_,
	UI_Main_menu,
	UI_Run_the_current_game_On_menu_accept_the_choice,
	UI_Next_preview_mode_On_menu_change_the_option,
	UI_Next_menu_mode, 
	UI_Next_game_group,
	UI_Next_game_type,
	UI_Include_Exclude_games_by_attribute,
	UI_Select_the_game_sort_method,
	UI_Next_emulator,
	UI_Commands,
	UI_Change_the_current_game_group,
	UI_Change_the_current_game_type,
	UI_Run_a_clone,
	UI_Rotate_the_screen,
	UI_In_the_submenus_,
	UI_Accept,
	UI_Unselect_all,
	UI_Select_all,
	UI_Toggle_include_exclude_required,
	UI_Cancel,
	
	UI_Listing_,
	UI_Settings_,
	UI_Emulators_,
	UI_Volume_,
	UI_Difficulty_,
	UI_Clone_,
	UI_Help,
	UI_Statistics,
	UI_Runcommand_,
	UI_Poweroff,
	UI_Menu,
	
	UI_Delete_game_snapshot,
	UI_Delete_game_clip,
	UI_Delete_game_flyer,
	UI_Delete_game_cabinet,
	UI_Delete_game_icon,
	UI_Delete_shown_image_from_parent,
	UI_Delete_game_sound,
	UI_Delete_played_sound_from_parent,
	UI_shown,
	UI_played,
	UI_No_commands_available,
	UI_Error,
	
	UI_Listing_Emulator,
	UI_Listing_Multiple,
	UI_Sort_,
	UI_Mode_,
	UI_Preview_,
	UI_Groups_,
	UI_Types_,
	UI_Filters_,
	
	UI_Game_Group_,
	UI_Game_Type_,
	UI_Calibration_,
	UI_Save_all_settings,
	UI_Restore_all_settings,
	UI_Clear_all_stats,
	UI_Lock_settings,
	UI_Settings,

	UI_Joystick_Configuration,
	UI_No_bluetooth_daemon,
	UI_Joystick,
	UI_none_,
	UI_Connect_one_joystick_to_continue,
	UI_Press_any_button_to_continue,
	
	UI_Listed,
	UI_Total,
	UI_Perc,
	UI_Games,
	UI_Play,
	UI_Time,
	UI_Most_time,
	UI_Most_play,
	UI_Most_time_per_play,
	
	UI_Capture_the_current_screen,
	
	UI_No_game_was_found,
	UI_No_game_was_found_for_the_emulator,
	UI_No_game_matches_the_group_selection_for,
	UI_No_game_matches_the_type_selection_for,
	UI_No_game_matches_the_filter_selection_for,
	UI_empty_,
};



struct _lang_struct
{
	int version;
	int multibyte;			/* UNUSED: 1 if this is a multibyte font/language */
//	UINT8_C *fontdata;		/* pointer to the raw font data to be decoded */
//	UINT16_C fontglyphs;		/* total number of glyps in the external font - 1 */
	char langname[255];
	char fontname[255];
	char author[255];
};
typedef struct _lang_struct lang_struct;

extern lang_struct lang;

int uistring_init (FILE *language_file);

const char * ui_getstring (int string_num);

#endif /* __UI_TEXT_H__ */


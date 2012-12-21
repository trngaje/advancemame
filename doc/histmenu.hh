<center><h2>History For AdvanceMENU</h2></center>
<h2> 
AdvanceMENU Version 2.6 2012/11
</h2>
<ul>
<li>
Fixed the "alsa" audio driver in Linux. In modern distributions it was generating no sound.
</li>
<li>
Support $HOME variable in directories specification for sdlmame.
</li>
<li>
For MAME bioses, devices and other not runnable roms are not listed anymore.
</li>
<li>
The new screen saver modes 'shutdown' and 'exit' works also with lock activated.
</li>
<li>
Added support for importing CatList files.
</li>
<li>
Renamed the old "ini" import format to "catver" to avoid confusion with the new "catlist".
</li>
<li>
Removed xmame support.
</li>
<li>
Removed DLL files from the Windows distribution. New new compiler uses static libraries.
</li>
<li>
Compiled with SDL-1.2.14, zlib-1.2.5, expat-2.0.1, freetype-2.4.4.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.5.0 2008/12
</h2>
<ul>
<li>
Fixed the recognition of the BIOS roms.
</li>
<li>
Smarter error detection of damaged zips.
</li>
<li>
Added the option 'exit' and 'shutdown' at the screensaver.
</li>
<li>
New Windows icon at high resolution.
</li>
<li>
Added automatic detection of mamepp.exe for Windows.
</li>
<li>
Added automatic detection of sdlmame for Windows and *nix.
</li>
<li>
Added a splash screen in Windows.
</li>
<li>
The 'overlaysize' option now has the default 'auto' which uses the current video mode.
</li>
<li>
Windows binary is provided with SDL.dll 1.2.13.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.14 2007/01
</h2>
<ul>
<li>
Fixed compilation problem with gcc 4.x.x.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.13 2005/12
</h2>
<ul>
<li>
Fixed the ui_help and ui_exit option in the DOS and
Windows platforms.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.12 2005/12
</h2>
<ul>
<li>
Fixed some bugs for the 64 bits platforms. It works now.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.11 2005/10
</h2>
<ul>
<li>
Added a new `mute' event. Default to period key on the keypad.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.10 2005/06
</h2>
<ul>
<li>
Improved the precision of all the joystick drivers.
Now a scale of 65536 values is used.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.9 2005/05
</h2>
<ul>
<li>
The directory is not chaged before running an
emulator in the *nix platforms. Instead the
full path name of the emulator is used.
</li>
<li>
Improved the support for low clocks of the SVGALIB
nVidia driver for DOS and Windows.
</li>
<li>
Readded in the DOS and Windows version the old
SVGALIB drivers for GeForce and Savage boards.
If you have problem with the new driver
you can still use the old ones with the
names "savage_leg" and "nv3_leg".
</li>
<li>
The `advv' and `advcfg' utilities are now compiled
and installed only if needed.
</li>
<li>
Fixed the detection of the new `unichrome' driver in DOS.
</li>
<li>
Added support for multiple mice in Windows XP using the
new `rawinput' mouse driver.
</li>
<li>
Added support for multiple mice in Windows 2000 using the
new `cpn' mouse driver.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.8 2005/03
</h2>
<ul>
<li>
The help screen exits when the played mng clip is
terminated.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.7 2005/03
</h2>
<ul>
<li>
Added a new `ui_startup' option to display a clip/image
at the menu startup.
</li>
<li>
Extended the `ui_help' and `ui_exit' option to
allow to display also a clip.
</li>
<li>
Updated to SVGALIB 1.9.20. This add support for the
most recent Radeon and nVidia boards at the DOS
version.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.6 2005/02
</h2>
<ul>
<li>
Before running any external program all the
privileges of suid/gid programs are dropped.
</li>
<li>
The option `display_restoreatgame' can now be customized
for different emulator.
</li>
<li>
Fixed the AC97 DOS audio driver.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.5 2005/01
</h2>
<ul>
<li>
Fixed the recognition of some USB mouses and keyboards
with the Linux event driver.
</li>
<li>
The USB devices in Linux now have always the same order
in the input_map[] specifications.
</li>
<li>
Fixed the creation of the .xml file in Windows with
directory names containing spaces.
</li>
<li>
The terminal cursor is now disabled using the Linux
SVGALIB driver with the restore_at_game
option set to no.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.4 2004/12
</h2>
<ul>
<li>
Fixed the screensaver and the alphanumeric selection.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.3 2004/12
</h2>
<ul>
<li>
The selection in the Groups/Types/Emulators
listing menu is not more intuitive. If only
an item is selected the cursor position
make an implicitely selection.
</li>
<li>
Fixed the sensitivity of the joystick movements.
</li>
<li>
Added a ncurses driver for text mode in Linux.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.2 2004/10
</h2>
<ul>
<li>
Added a new `input_hotkey' option to disable OS
hotkeys like CTRL+C.
</li>
<li>
Fixed the game autostart and screensaver.
</li>
<li>
Fixed the display of the "Listing" menu.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.4.1 2004/09
</h2>
<ul>
<li>
Added support for translucency in the user interface
with the `ui_translucency' option. Note that without
a background image you will not see difference.
</li>
<li>
The sound volume is propagated to the emulator only
in console mode (i.e. AdvanceCD).
</li>
<li>
When using a .bat file to start an emulator the .lst file
is now used if the .xml file is missing.
</li>
<li>
Added a sort by emulator option.
</li>
<li>
Fixed the screensaver.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.3.8 2004/08
</h2>
<ul>
<li>
Fixed the renaming of the AdvanceMESS snapshots,
clips and sounds. It was broken on
version 2.3.6.
</li>
<li>
Added support for specific emulator configuration.
</li>
<li>
New menu organization.
</li>
<li>
Added the new `ui_menukey' option to enable and disable
the keyboard shortcuts in the menus.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.3.7 2004/07
</h2>
<ul>
<li>
Fixed a Mac OS X crash problem with XML files with
empty items.
</li>
<li>
Only the XML driver.status entry is now used to recognize
the game emulation status. Game not marked as
"preliminary" are considered working games.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.3.6 2004/07
</h2>
<ul>
<li>
Removed the copyright message box at startup.
</li>
<li>
On Linux you can now specify relative directories
in the `emulator_altss/roms/*' options.
</li>
<li>
If no game is shown, the reason is explicated at the user.
</li>
<li>
Improved the xmame support.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.3.5 2004/06
</h2>
<ul>
<li>
Added volume control at the `oss' audio driver.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.3.4 2004/05
</h2>
<ul>
<li>
The program doesn't try to effectively load the
"none" image file used to disable the image
loading.
</li>
<li>
Added a new `device_alsa_mixer' option to control the sound mixing.
The default is to use an internal mixer to change to volume.
</li>
<li>
Documented in the `advdev.txt' file how use the ALSA `dmix' plug
for a software mixing from multiple programs.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.3.3 2004/04
</h2>
<ul>
<li>
Updated the expat library to version 1.95.7.
</li>
<li>
Added support for recent Radeon boards 9600/9700/9800 at
the DOS version.
</li>
<li>
Fixed the background music multi directory specification in Linux.
</li>
<li>
Fixed the `generic' emulator rom finding.
</li>
<li>
Added a new `device_alsa_device' option to select the ALSA
output device.
</li>
<li>
Removed the initial audio tick with the ALSA and OSS drivers.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.3.2 2004/03
</h2>
<ul>
<li>
Renamed the `video_*' options in `display_*' like AdvanceMAME.
</li>
<li>
Added new `ui_help' and `ui_exit' options to display arbitrary
help and exit images.
</li>
<li>
Added support for TrueType (TTF) fonts with alpha blending
using the FreeType2 library. Added also a new `ui_fontsize'
option to select the font size and a collection of TTF fonts
in the `contrib/' dir
</li>
<li>
Fixed the icon loading from a .zip file.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.3.1 2004/02
</h2>
<ul>
<li>
If no preview is available, the preview type is automatically
changed to a preview type not empty.
</li>
<li>
The default host configuration directory is now /usr/local/etc if you
install in /usr/local.
</li>
<li>
The default documentation directory is now /usr/local/share/doc/advance
if you install in /usr/local.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.3.0 2004/02
</h2>
<ul>
<li>
Added a the new entry `multiloopall' at the `ui_clip' option
which loops all the clips.
</li>
<li>
To compile in DOS/Window now you must use the `Makefile.usr' file.
</li>
<li>
Added the "m" library at the link step. This solve the linking
problem with recent MINGW compilers.
</li>
<li>
Fixed a problem when converting old options name to the new values.
The conversion was failing if more than one conversion was required.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.17 2004/01
</h2>
<ul>
<li>
Removed the MAME .cfg support. AdvanceMAME doesn't use them
anymore.
</li>
<li>
The DOS version should now restore correctly the video mode at the
program exit.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.16 2003/12
</h2>
<ul>
<li>
The menu is now more aggressive using CPU power. It helps
playing video clips at more than 50 Hz and with
multiple clips.
</li>
<li>
In the multi clip mode, the cursor clip and sound are now always
played at full speed. Only the background clips are played
in slow-motion if the CPU power isn't enough.
</li>
<li>
The DOS version now automatically disable any BIOS call on ATI
boards to prevent problems on broken video BIOS.
</li>
<li>
Reverted back the S3 Savage/Virge/Trio SVGALIB driver at
version 1.9.17. At least with S3 VirgeDX there is
a regression in version 1.9.18.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.15 2003/12
</h2>
<ul>
<li>
The mixer now uses a separate buffer for any sound effects.
This should decrease the sound overlapping of too long
sound buffers.
</li>
<li>
Fixed a segmentation fault bug on the DOS SVGALIB driver for
ATI Rage boards.
</li>
<li>
The ui_top/bottom/left/right values are now scaled according
to the ui_background image.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.14 2003/11
</h2>
<ul>
<li>
All the snapshots and animated clips are now reduced in size
using the AdvanceMAME `mean' effect.
</li>
<li>
In the DOS version removed the legacy support for unchained
VGA modes (8 bit modes with memory in 4 planes) and the
VBE1 banked modes (not linear modes).
</li>
<li>
Added a new `ui_clip' option which allow to play all the video
clips together.
</li>
<li>
Improved the ui_background option. It now supports the most common
RGB and palette formats and it's also able to stretch the image
to fit the screen. It's now working also in DOS/Windows.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.13 2003/10
</h2>
<ul>
<li>
The SIGHUP signal now kills and restarts the program.
</li>
<li>
Added external commands support with the new
`ui_command' option.
</li>
<li>
Upgraded the DOS/Windows SVGALIB drivers at version 1.9.18.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.12 2003/09
</h2>
<ul>
<li>
Added a new set of `ui_*' options to define an user interface skin
with a background image.
</li>
<li>
Added a new `misc_exit' option which substitute the old `exit_press'.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.11 2003/09
</h2>
<ul>
<li>
Fixed some problems on the SDL keyboard driver in the
Windows version.
</li>
<li>
Added a preliminary support for newer Radeon boards for the
DOS version.
</li>
<li>
The Linux version now works also if it cannot read the tty state.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.10 2003/09
</h2>
<ul>
<li>
Added a new set of Linux input driver for keyboards, mice
and joysticks based on the Linux input-event interfaces.
</li>
<li>
Added a new `include' option to include additionally
configuration files.
</li>
<li>
Added support for png images with alpha channel.
</li>
<li>
The `Run Clone' command on a MESS software now allow to select the
clone of the BIOS to use.
</li>
<li>
The `Types' and `Group' menus are shown only if the group and
type set are not empty.
</li>
<li>
In Linux the host configuration files are now read in /etc.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.9 2003/07
</h2>
<ul>
<li>
Added support for importing the XML output of AdvanceMESS 0.71.0.0.
</li>
<li>
Added support for ignoring the return code of the executed
generic emulator. Simply put a '-' in front of the emulator
executable name.
</li>
<li>
The number of listed games is now correct in any sort mode.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.8 2003/07
</h2>
<ul>
<li>
Added the volume control at the SDL sound driver. It's implemented
reducing the sample values and not using the hardware
volume control.
</li>
<li>
Fixed a possible bug on the SVGALIB Rage 128/Radeon drivers.
Also updated the SVGALIB patch in the contrib/ dir.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.7 2003/06
</h2>
<ul>
<li>
Fixed the horizontal and vertical sync polarity on the Linux
Framebuffer video driver [by Ralph]
</li>
<li>
The Windows version is now able to shutdown the system like the
other versions.
</li>
<li>
Added support for the new MAME -listxml option. It's now
used as default. The old -listinfo is still supported
and it's used if -listxml fails.
</li>
<li>
Fixed the restoring of the old cursor position.
</li>
<li>
Removed the legacy support for the "mm.cfg" configuration file.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.6 2003/05
</h2>
<ul>
<li>
Added support for Mac OS X. It compiles and run with the SDL library.
</li>
<li>
Added support for generic BigEndian targets.
</li>
<li>
In Linux you can specify an arbitrary data directory with the
$ADVANCE environment variable. This value overwrites the default
$HOME/.advance.
</li>
<li>
Added the "-version" command line option.
</li>
<li>
Removed some "buffer overflow".
</li>
<li>
Fixed the mouse handling in Linux with the SVGALIB library
[by Fabio Cavallo].
</li>
<li>
Added a new sort order `time_per_coin'.
</li>
<li>
Added a statistics information page.
</li>
<li>
Some fixes for the gcc 3.3 compiler.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.5 2003/03
</h2>
<ul>
<li>
Fixed the default colors.
</li>
<li>
Updated with autoconf 2.57.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.4 2003/02
</h2>
<ul>
<li>
Fixed the CPU detection by the configure script.
</li>
<li>
The Linux ALSA sound drivers now doesn't block the execution if the
DSP is already in use.
</li>
<li>
Fixed the Linux Frame Buffer driver when a DIRECTCOLOR mode
is used displaying fuzzy colors.
</li>
<li>
Added a new `device_video_cursor' option to control the visibility
of the mouse cursor.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.3 2003/01
</h2>
<ul>
<li>
Added a new attrib for the AdvanceMESS emulator
to exclude the systems without a BIOS rom.
</li>
<li>
Added the Alsa sound driver for Linux. It's now the
preferred choice.
</li>
<li>
The emulator rom info file is not updated if the existing
file is readonly.
</li>
<li>
Renamed the "Attrib" menu to "Selection".
</li>
<li>
Added a new `difficulty' option and menu to globally control
the difficulty level of the games. It works only
for the `advmame' emulator type and only for games
with a difficulty dip switch.
</li>
<li>
The `generic' emulator type is now able to import games
information from a MAME like information file
written manually.
</li>
<li>
The default key configuration now uses the numeric-pad keys
as arrows. It helps with HotRod.
</li>
<li>
For the `advmame' and `advmess' emulator types the
sound volume is automatically set at the same
level of the menu.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.2 2002/12
</h2>
<ul>
<li>
Fixed the MNG playing in 32 bits modes. This is potentially
a crash bug fix.
</li>
<li>
The bitmap scaler now uses the same algo of the MNG player.
</li>
<li>
The game index is now saved when the sort order is changed
or when the game position changes because you play it.
</li>
<li>
Added a new raw keyboard driver for Linux [Kari Hautio].
</li>
<li>
Fixed some issues in the ./configure scripts.
</li>
<li>
Removed the blinking cursor in the Linux `fb' video
driver [Kari Hautio].
</li>
<li>
Removed the option `video_depth'.
</li>
<li>
Upgraded at the SVGALIB 1.9.17 library.
</li>
<li>
Removed the support for the 8 bit palette mode.
</li>
<li>
The `color' option now support a true color specification with
the format RRGGBB in hex values.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.1 2002/11
</h2>
<ul>
<li>
Reduced the startup load time.
</li>
<li>
The event `group' and `type' now select the next group and type
instead of opening the menus.
</li>
<li>
Fixed the abort bug on the DOS and Windows versions for the
SVGALIB Rage 128/Radeon video boards and probably others.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.2.0 2002/11
</h2>
<ul>
<li>
In the Linux target the `shutdown' event runs "/sbin/poweroff".
</li>
<li>
Merged the sdl and native system of the Linux target.
You can now mix the SDL input/output drivers with the
native drivers.
</li>
<li>
Better ./configure script. It detects and automatically
enables all the available libraries.
</li>
<li>
Fixed a precision error checking the clock in the DOS vgaline
driver.
</li>
<li>
Removed the limitation of 8 bit crtc multiplier in the DOS
vgaline driver.
</li>
<li>
Revised the SVGALIB DOS compatibility layer. Some bugs fixed.
</li>
<li>
Added the support for the SVGALIB video drivers in Windows NT/2000/XP.
</li>
<li>
Fixed a slowdown bug on the SVGALIB video board detection.
</li>
<li>
Fixed a bug on the DOS/Windows SVGALIB Radeon driver. It should
help with the video modes at lower frequency generally used
in Arcade Monitors and TVs.
</li>
<li>
Fixed a bug on the SDL sound management. It should help the
interaction of AdvanceMENU and AdvanceMAME.
</li>
<li>
Better error reporting on the DOS/Windows SVGALIB drivers.
</li>
<li>
The Windows binary is now packaged with the SDL dll 1.2.5
</li>
</ul>
<h2> 
AdvanceMENU Version 2.1.2 2002/09
</h2>
<ul>
<li>
Added the support of the VSyncMAME audio drivers. This add
support for the AC97 chipset [Shigeaki Sakamaki].
</li>
<li>
Removed the compression of all the executables, and added
a little debug info on the precompiled binaries.
This should help the problem reporting.
</li>
<li>
Fixed a bug when setting emulator attributes without games
listed.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.1.1 2002/09
</h2>
<ul>
<li>
Fixed the window title and the icon in the sdl system.
</li>
<li>
In Windows the default sdl samples buffer is now 2048.
This solve the distorted sound.
</li>
<li>
In DOS the executables are now searched also in the current
directory.
</li>
<li>
Fixed the missing software bug when both DMESS and AdvanceMESS
are present.
</li>
<li>
Some minor fixes for the AdvanceMESS use.
</li>
<li>
Added the `cost' doc.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.1.0 2002/09
</h2>
<ul>
<li>
Revised the output format of all the documentation. Now
it's available as formatted text, html and man pages.
</li>
<li>
Fixed a bug on the emulator names. Now they can be uppercase.
</li>
<li>
Fixed the "Division by Zero" crash bug with the Rage128 board in
the DOS svgaline driver.
</li>
<li>
Upgraded at the SVGALIB 1.9.16 library.
</li>
<li>
Added a `./configure' script in all the distributions.
</li>
<li>
Removed the output of the `display_*' option when running
advcfg for AdvanceMENU.
</li>
<li>
Fixed the detection of the screen resize keys in the
advcfg and advv utility in the Linux platform.
The keys are now changed to 'i' and 'k'.
</li>
<li>
Added the %o option in the emulator command line to force the
emulators to use the same orientation of the menu.
Check the example in the advmenu.txt file.
</li>
<li>
The keyboard key codes are changed another times. Now they are
always the same for all the platform and for all the
keyboard drivers. If you are using the linux version probably
you need to reconfigure the event option with the new codes.
</li>
<li>
Renamed the `msg_run' option in `run_msg'.
</li>
<li>
Added the option `run_preview' to select which preview display
before to run a game.
</li>
<li>
Removed all the `select_*' option. They are now specified with the
new `emulator_attrib' option. It allow to specify different
display attribute for different emulator. For example you can
show clones for AdvancePAC and only parents for AdvanceMAME.
</li>
<li>
Fixed the slowdown bug with a lot of MESS roms.
</li>
<li>
Generally improved the speed of the select/sort implementation
of the list of games.
</li>
<li>
Added the `loop' option to continuously play the MNG files.
</li>
<li>
In a single column mode the left and right events are now like
page up and page down events.
</li>
<li>
Now the idle event never interrupt a long game clip or sound.
</li>
<li>
If the generic emulator path is empty the idle start event
is automatically disabled.
</li>
<li>
When returning from a idle start event the game clip and sound
aren't played.
</li>
<li>
Added a new `info_import' option to display arbitrary information
for every game.
</li>
<li>
You can now specify more than one `desc_import', `type_import'
and `group_import' options.
</li>
<li>
Added in the contrib/ dir the hiscore.ini file to display the top
hiscore of the MAME games.
</li>
<li>
Various fix at the `advcfg' utility.
</li>
<li>
Fixed some problems of the fullscreen use of the sdl system.
</li>
<li>
Various bug fixed.
</li>
</ul>
<h2> 
AdvanceMENU Version 2.0.0 2002/08
</h2>
<ul>
<li>
Added the `sdl' system which uses the libSDL graphics
library. This system enable the use of the program
in a Window Manager.
</li>
<li>
Added the HOST_SYSTEM section in the makefile.
</li>
<li>
Added the `windows' target in the Makefile. It use the `sdl'
system.
</li>
<li>
Added the `device_sdl_fullscreen' option to force the use of
a fullscreen mode with the SDL library.
</li>
<li>
The key specification in the config file are now by
name and not by scancode. The list of available
names is in the advmenu.txt file. This makes
the configuration file portable.
</li>
<li>
Added the support of the Windows MAME emulator with the
emulator type `mame'. The old DOS MAME is still
supported with the `dmame' emulator type.
</li>
<li>
Added the support of the XMAME emulator with the
emulator type `xmame'.
</li>
<li>
The Linux version now searches the emulators in the whole
PATH if a relative path is given.
</li>
<li>
Added support for the 32 bit graphics output.
</li>
<li>
The article `The' on the game names is put at the end
of the name.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.19.1 2002/07
</h2>
<ul>
<li>
Fixed a bug when forcing the use of the vbeline/3dfx driver.
</li>
<li>
Enabled the interlaced modes with the 3dfx svgalib driver.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.19.0 2002/06
</h2>
<ul>
<li>
Upgraded at the SVGALIB 1.9.15 library.
</li>
<li>
Added the joystick calibration on the main menu.
This fixes the inverted movement with some
joysticks on the DOS version.
</li>
<li>
The third mouse and joystick button now open the
main menu.
</li>
<li>
The `idle_screensaver_preview play' option now
play only the animated snapshots. The static
snapshots are ignored.
</li>
<li>
A better random selection of the screensaver
snapshot.
</li>
<li>
In the Linux version if the HOME environment variable
is not set all the files are read and written on the
PREFIX/share/advance directory.
</li>
<li>
Minor bugfix.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.18.3 2002/05
</h2>
<ul>
<li>
Fixed an hang bug when no games are listed.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.18.2 2002/05
</h2>
<ul>
<li>
Upgraded at the SVGALIB 1.9.14 library.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.18.1 2002/04
</h2>
<ul>
<li>
Added the support for AdvancePAC with the emulator
type `advpac'.
</li>
<li>
Added the `advcfg' utility in the distribution
package.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.18.0 2002/04
</h2>
<ul>
<li>
Added the list mode `text'. It displays only the
game names without any image.
</li>
<li>
Added description import from NMS files with the
option `desc_import'
</li>
<li>
Changed the behavior of the `*_import' options.
Now the imported values never overwrite the
user choices.
</li>
<li>
Upgraded at the SVGALIB 1.9.13 library.
</li>
<li>
Upgraded at the Allegro 4.01 library.
</li>
<li>
Fixed the mouse deinitialization.
</li>
<li>
Better screen separation with the list_mixed and
full_mixed modes in video modes without
square pixel.
</li>
<li>
Fixed the use of absolute path without the drive
specification.
</li>
<li>
Fixed the aspect of the marquee images with
vertical monitors.
</li>
<li>
Minor bugfix.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.17.5 2002/04
</h2>
<ul>
<li>
Fixed the `emulator_rom_filter' option when more than
one pattern is specified.
</li>
<li>
Fixed a crash bug with roms on the root directory.
</li>
<li>
Fixed a potentially crash bug on the MNG playing on
low resolution modes.
</li>
<li>
The fast movement ins/del keys now consider the games
with a digit as the first char in the description
of the same category.
</li>
<li>
Minor bugfix.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.17.4 2002/04
</h2>
<ul>
<li>
Added the `preview_expand' option to better control the
aspect of the images.
</li>
<li>
The sound_foreground_begin effect is now correctly played.
</li>
<li>
Added the option `idle_screensaver_preview play'.
</li>
<li>
Fixed the `generic' emulation support.
</li>
<li>
Fixed some minor issues on the image preview.
</li>
<li>
Fixed the delete commands.
</li>
<li>
Minor bugfix.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.17.3 2002/03
</h2>
<ul>
<li>
The the first frame of the .mng files is used as static
snapshot if none .png file is found.
</li>
<li>
Better screen split on the list_mixed and full_mixed modes.
</li>
<li>
Added the `resolution' sort order.
</li>
<li>
Better .mng file support. The files saved with
"Jasc Animation Shop" should work.
</li>
<li>
The game description is now displayed in two positions in
the screen saver.
</li>
<li>
The "event_emulator" key now switch to the next emulator
without the menu. You can still use the
emulator menu calling it from the main menu.
</li>
<li>
Added the `misc_quiet' option to disable the text message :
"AdvanceMENU - Copyright (C) 1999-2002 by..."
</li>
<li>
Fixed a crash bug on the rotated modes when a MNG end it's
playing period.
</li>
<li>
Fixed a crash bug when no video modes are available.
</li>
<li>
Added support for mp3 files with a RIFF header.
</li>
<li>
Minor bugfix.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.17.2 2002/03
</h2>
<ul>
<li>
Fixed the output of the `color' options with the
`-default' command.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.17.1 2002/03
</h2>
<ul>
<li>
Fixed a bug importing category files.
</li>
<li>
Fixed a compiling bug for gcc 2.95.x.
</li>
<li>
Added the sound_buffer option to control the size of
the sound buffer.
</li>
<li>
Increased the sound interrupt frequency.
</li>
<li>
Added the documentation of the `color' options.
</li>
<li>
Added the output of the `color' and `event_assign' options
with the `-default' command.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.17.0 2002/02
</h2>
<ul>
<li>
Added MNG animated snapshot !
</li>
<li>
Faster PNG decoding.
</li>
<li>
Added 3 new fonts in the contrib dir: amiga, c64_8x16 and
c64_8x8 [Flemming Dupont].
</li>
<li>
Bugfix for the case of the game descriptions.
</li>
<li>
Bugfix for the use of " with path in the config file.
</li>
<li>
The snapshot directories listed in the emulator configuration
file now have the precedence over the directories
specified in the advmenu.rc file.
</li>
<li>
Readded the sound interrupt at the SEAL sound driver.
It uses a lower frequency than the previous version
(18 Hz instead of 100 Hz). So, please report feedback.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.16.1 2002/01
</h2>
<ul>
<li>
Added support for AdvanceMESS with the `advmess' type.
The support for AdvanceMESS is better than the MESS
support because AdvanceMESS output more information
with the -listinfo option.
</li>
<li>
The linux config directories are now named `$home/.advance'
and `$prefix/share/advance'.
</li>
<li>
Fixed and renamed the options `group/type_inport' in
`group/type_import'.
</li>
<li>
All the MESS/AdvanceMESS software is always listed also if
you choose to display only the parent rom.
</li>
<li>
When you run a clone pressing F12 the MESS/AdvanceMESS
software is not listed.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.16.0 2002/01
</h2>
<ul>
<li>
Added a Linux port
</li>
<li>
Added a new emulator type `advmame' which support the new
AdvanceMAME configuration format (from 0.56.2).
</li>
<li>
Changed the name of the configuration file. Now it's
named `advmenu.rc'. Check the `release.txt' file for
the conversion instruction.
</li>
<li>
Removed the `like_emulator' value for all the options.
You should now explicitly configure them.
</li>
<li>
Now you can specify an arbitrary game description with
the `game' option. Check the `advmenu.txt' file.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.15.2
</h2>
<ul>
<li>
Solved a bug that prevented the read of the vbeline_* options.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.15.1
</h2>
<ul>
<li>
Added the new `[video] pclock' option like AdvanceMAME. Add it
in your configuration file.
</li>
<li>
Minor bugfix
</li>
</ul>
<h2> 
AdvanceMENU Version 1.15.0
</h2>
<ul>
<li>
Added the `emulator_roms_filter' option to filter the
roms files for the `generic' emulator.
</li>
<li>
The `coin' and `time' values of a parent games count also
the clones if they are not displayed.
</li>
<li>
Added a new event `shutdown' which exit promptly and
shutdown the PC. (default CTRL+ESC)
</li>
</ul>
<h2> 
AdvanceMENU Version 1.14.2
</h2>
<ul>
<li>
Removed the `cfg_scan' option. Now it's always active and
really fast! The coins number is cached in the
mm.cfg file and the .cfg files are read only the
first time.
</li>
<li>
Solved the `default_mode' selection bug.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.14.1
</h2>
<ul>
<li>
Added the emergency signal handlers like AdvanceMAME
</li>
<li>
Minor bugfix
</li>
</ul>
<h2> 
AdvanceMENU Version 1.14.0
</h2>
<ul>
<li>
The same video driver and modelines changes of AdvanceMAME 0.53
</li>
<li>
Added the option video_depth to control the depth of the
video mode used.
</li>
<li>
The default video mode is now 1024x768 at 16 bit.
</li>
<li>
The `format' option if present is used to create a video
from scratch like AdvanceMAME.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.13.0
</h2>
<ul>
<li>
Solved the empty list problem with the -default option
</li>
<li>
Changed the default font.
</li>
<li>
Added the `lock' option to prevent any modification at the user
interface. You can activate it from the config file,
from the runtime menu or pressing the lock key (scroll-lock).
</li>
<li>
Added the `coin' sort key.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.12.0
</h2>
<ul>
<li>
Added the support for a `generic' emulator type. With this you
should be able to use AdvanceMENU with any other emulator.
Check the `mm.txt' file in the `emulator' section.
</li>
<li>
Added a selection by Vertical/Horizontal
</li>
<li>
Solved the `hidden cursor' bug
</li>
<li>
The `&lt;undefined&gt;' selection is now saved
</li>
<li>
Support for additional command line argument in the emulator call
</li>
<li>
Solved a crash bug if no snapshot are available on the slideshow
</li>
<li>
Added the game name on the slideshow
</li>
<li>
The `enter' press on the slideshow now start the game
</li>
<li>
Added the -quiet option to prevent the startup messages.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.11.0
</h2>
<ul>
<li>
Added external import of the CATLIST file
[<a href="http://www.mameworld.net/catlist/]">http://www.mameworld.net/catlist/]</a>
and of the `MacMAME Grouping Files'.
[<a href="http://www.tznet.com/cmader/categories.html]">http://www.tznet.com/cmader/categories.html]</a>.
See the new options `type_import' and `group_import'.
</li>
<li>
Added two new preview modes `list_mixed' and `full_mixed'
that show the snap, title, flyer and cabinet images
together.
</li>
<li>
Added the new option `config' to select when and if the
runtime configuration  changes should be saved
or restored.
</li>
<li>
Added inclusion/exclusion attribute for the `Deco Cassette'
and `PlayChoice-10' games.
</li>
<li>
Added the `idle_screesaver_preview' option `off' to
shutdown the monitor in the screensaver mode
using the VESA/PM services if available.
</li>
<li>
Removed the auto cutoff of the preview images.
</li>
<li>
The icons and marquees images are now selectable with the
new two modes `tile_icon' and `tile_marquee'.
The older previous options `preview_icon' and
`preview_marquee' don't exist anymore.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.10.0
</h2>
<ul>
<li>
The "neogeo" selection filter is now working
</li>
<li>
Faster blit for MMX processors
</li>
<li>
Progressive screen update
</li>
<li>
Added a new fullscreen mode
</li>
<li>
Added the sort/mode/preview/attribute items in the main menu
</li>
<li>
Added the sort key in the status bar and in the game list
</li>
<li>
Solved a bug on the tiled modes with vertical monitor.
</li>
<li>
You can select the preview to use in the slide show mode with the
new option "idle_screensaver_preview"
</li>
</ul>
<h2> 
AdvanceMENU Version 1.9.0
</h2>
<ul>
<li>
Added a new event `rotate' to rotate the screen at runtime
for cocktail machines. This event is mapped for default
on the `0 PAD' key
</li>
<li>
Added support for zipped collections of images and sounds.
Simply copy the .zip archives with the .png,.pcx,.ico,
.mp3,.wav in the various snap, flyers... directories.
For best performance the .zip archives containing
`.png' and `mp3' files should be compressed in
`storing' mode (without compression). These file
formats are already compressed and don't need to
be compressed another time.
</li>
<li>
Added support for 4 bit .ico files
</li>
<li>
Long icon titles are now truncated and not skipped
</li>
<li>
Solved the crash bug with empty preview images.
</li>
<li>
The first preview image found is used. Not the last.
</li>
<li>
Changed the default value of `cfg_scan' and `crc_scan' to `no'
</li>
<li>
Added the `-default' option to force the creation of a
default `mm.cfg'
</li>
</ul>
<h2> 
AdvanceMENU Version 1.8.0
</h2>
<ul>
<li>
Added the new option `preview_adjust' to control how
the images is stretched.
Use `preview_adjust square' if you like images
with the correct aspect ratio.
</li>
<li>
The MP3 player is now able to skip the ID3 headers
</li>
<li>
The `Run game' message is now customizable with the
option `msg_run'.
</li>
<li>
Added the `preview_default_*' options to specify default images.
</li>
<li>
The mouse can be disabled with the option `mouse no'
</li>
<li>
The background sound now continues also in the screensaver mode.
</li>
<li>
Added a set of .cfg files for the most common TVs and Monitors.
</li>
<li>
Some changes at the timer used for the `idle_start'
and `idle_screensaver' options.
</li>
<li>
Changed the default value of the option "sound_interrupt"
to "yes". Enable it if your sound is choppy.
</li>
<li>
Simplified the use with a normal PC monitor. If no video
hclock/vclock or video modes are defined a default
PC multisync monitor is assumed with the use of the
standard VGA and VESA modes enabled.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.7.0
</h2>
<ul>
<li>
The screen saver now doesn't ignore the `exit_press' option
</li>
<li>
Added trackball support. See the new `mouse_delta' option
</li>
</ul>
<h2> 
AdvanceMENU Version 1.6.0
</h2>
<ul>
<li>
Added coinage support for mame. You can disable it with
the cfg_scan option (to speedup the menu loading time)
</li>
<li>
Removed the automatic creation of the info.txt file if the
emulator executable is a batch file
</li>
<li>
Added the option "exit_press 0" to completely disable the exit.
</li>
<li>
The snapshot displayed in the listing mode without the clones is
now the snapshot of the effective clone run if the parent
set is not runnable.
</li>
<li>
All the file name matching is now case insensitive.
</li>
<li>
Added the new type of snapshot named `titles'. It works
like `flyers' and other images.
</li>
<li>
Some changes at the sound options. Check the mm.txt files
for the sound_foreground_* and sound_background_*.
</li>
<li>
Renamed `video_mode_switch' in `video_mode_reset'.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.5.1
</h2>
<ul>
<li>
Corrected the option `event_assign'
</li>
<li>
Corrected the option `mode_skip'
</li>
<li>
Corrected some problems with `"' in the `mm.cfg'
</li>
<li>
Changed the game sort to be case insensitive
</li>
</ul>
<h2> 
AdvanceMENU Version 1.5.0
</h2>
<ul>
<li>
Added support for Trident cards using the new video
driver of VSyncMAME written by Saka.
Use the "vbeline_driver=vbe3" option to revert to the
old behavior.
</li>
<li>
Documentation update. Now in sync with the program.
</li>
<li>
Better icon support. Now you need to press `space' to activate it.
</li>
<li>
Added the option `icon_space' to control the space
between icons.
</li>
<li>
The VBE BIOS is now initialized after every emulator start.
This solves problems with some bogus VBE BIOS (3dfx).
</li>
<li>
Added the `event_alpha' option to disable the fast moving
with alphanumeric keys. This may be required with
keyboard encoders when alphanumeric keys are used for
others purpose.
</li>
<li>
Added marquee support with the new mame.cfg option `marquees'.
Press `space' to activate it.
</li>
<li>
Added multi emulator support. Now you can list all the games
of various emulators together. See the new mm.cfg
`emulator' option.
</li>
<li>
Added `raine' support.
</li>
<li>
Added PCX support. Used by `raine' for the snapshots.
</li>
<li>
Added a new emulator menu (key F6) and the `emulator_include'
option to select that emulators to show
</li>
<li>
Added the a new global menu (key ~) on which you can select
other menus
</li>
<li>
Added the option `video_font'
</li>
<li>
Added the option `mode_skip'
</li>
<li>
Added the option `video_config'
</li>
<li>
Added the option `video_mode_switch'
</li>
<li>
Added the option `joystick'
</li>
<li>
Added the option `joystick_config'
</li>
<li>
Added the option `video_orientation'
</li>
<li>
Added the option `video_gamma'
</li>
<li>
Added the option `video_brightness'
</li>
<li>
Added support for `not', `or', `and' key definitions with the
new option `event_assign'
</li>
</ul>
<h2> 
AdvanceMENU Version 1.4.0
</h2>
<ul>
<li>
Added a new option `event_mode fast' to select if to
wait or not for a complete screen redraw
</li>
<li>
Added a new option `event_repeat' to select the repeat rate
of the events
</li>
<li>
Automatic exclude of flyers/cabinet modes if no images are found
</li>
<li>
Added some new tiled modes `tile_big', `tile_enormous'
and `tile_giant'
</li>
<li>
More fast tiles scrolling
</li>
<li>
Added a flashing cursor
</li>
<li>
Added a new option `video_size' to select a generic video mode
size without the use of the `mv' utility
</li>
<li>
Added sound support. Play `wav' file for some action and `mp3'
files in background. See the new mm.cfg `sound_*' options.
</li>
<li>
Added per game sounds support. The mame.cfg option `snap' and
`altss' are used to found `mp3' or `wav' sounds with the
same name of the game to play when the cursor move
to a specific game.
</li>
<li>
Added support for `ico' (Windows Icons) snapshot.
These images are only used in the bigger tile modes if
available.
The `ico' files are read using the new mame.cfg `icons'
option.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.3.0 (0.37b8.0) 2000/10
</h2>
<ul>
<li>
Better support for MESS
</li>
<li>
Corrected some bugs related to long filenames and filenames
with spaces or dots
</li>
<li>
Added the command menu (F8) to do some file operations
</li>
<li>
Added support for "cabinets" snapshot
</li>
<li>
Corrected the proportional font display
</li>
<li>
Added a fast move to the next system with the INS and DEL keys.
</li>
</ul>
<h2> 
AdvanceMENU Version 1.2.0 (0.37b7.0)
</h2>
<ul>
<li>
Added support for MESS
</li>
<li>
Added support for loading GRX fonts
</li>
<li>
Added support for proportional fonts
</li>
<li>
Added some new GRX fonts in the contrib/fonts directory,
you can found the complete (big) set at
<a href="ftp://x2ftp.oulu.fi/pub/msdos/programming/djgpp/v1tk/cbgrf103.zip">ftp://x2ftp.oulu.fi/pub/msdos/programming/djgpp/v1tk/cbgrf103.zip</a>
</li>
</ul>
<h2> 
AdvanceMENU Version 1.1.0 (0.37b6.0)
</h2>
<ul>
<li>
Corrected the timing in the `idle_start' option, now
the timer also reset if the user exit from `mame'
pressing ESC (requires AdvanceMAME 0.37b5.1 or greater)
</li>
<li>
When the clone aren't listed and you start a game not emulated
correctly the best clone is run instead. If you want
to force the run of the bad game use the F12 command.
</li>
<li>
If the screenshot isn't found it's searched the screenshot of any
available clones or parent games.
</li>
<li>
The key for selecting/deselecting all in the menu are now 'INS'
and 'DEL' and are remappable with the cfg file.
</li>
<li>
Added the preview of the flyers pics. See the new options 'flyers'
in and press SPACE to switch from snap to flyers
</li>
<li>
New look
</li>
<li>
Colors adjustable in the file `mm.cfg'
</li>
<li>
Automatic cut of any symmetric left/right or up/down monochromatic
band in the previews
</li>
<li>
Compiled with the latest Allegro library to support all the
newer joysticks
</li>
<li>
Now works also if some games are missing in the info.txt
</li>
</ul>
<h2> 
AdvanceMENU Version 1.0.0 (0.37b5.0) 2000/08
</h2>
<ul>
<li>
Added a new menu mode to the utility `mm' which display
a lot of tiled preview at the same times. Press TAB to
cycle from one mode to another.
</li>
<li>
Now the utility `mm' is available also as a standalone project
named AdvanceMENU
</li>
</ul>
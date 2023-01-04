#pragma once
#include "Config.h"

const char *const introTextData =
	"\blThe Xphere's Mod " MTOS(MOD_MAJOR_VERSION) "." MTOS(MOD_MINOR_VERSION) ", \bU" APPNAME "*\bU " MTOS(SAVE_VERSION) "." MTOS(MINOR_VERSION) " - https://powdertoy.co.uk, irc.libera.chat #powder\n"
	"\n"
	"\n"
	"\bg[Ctrl] + [C]/[V]/[X] are Copy, Paste and cut respectively.\n"
	"\bgTo choose a material, hover over one of the icons on the right, it will show a selection of elements in that group.\n"
	"\bgPick your material from the menu using mouse left/right buttons.\n"
	"Draw freeform lines by dragging your mouse left/right button across the drawing area.\n"
	"[\xee\x9d\x92] + Drag will create straight lines of particles.\n"
	"[Ctrl] + Drag will result in filled rectangles.\n"
	"[Ctrl] + [\xee\x9d\x92] + Click will flood-fill a closed area.\n"
	"Use the mouse scroll wheel, or '[' and ']', to change the tool size for particles.\n"
	"Middle click or [Alt] + [Click] to \"sample\" the particles.\n"
	"[Ctrl] + [Z] will act as Undo.\n"
	"\n\boUse [Z] for a zoom tool. Click to make the drawable zoom window stay around. Use the wheel to change the zoom strength.\n"
	"The [Space] can be used to pause physics. Use [F] to step ahead by one frame.\n"
	"Use [S] to save parts of the window as 'stamps'. [L] loads the most recent stamp, [K] shows a library of stamps you saved.\n"
	"Use [P] to take a screenshot and save it into the current directory.\n"
	"Use [H] to toggle the HUD. Use [D] to toggle debug mode in the HUD.\n"
	"\n"
	"Contributors: \bgStanislaw K Skowronek (Designed the original Powder Toy),\n"
	"\bgSimon Robertshaw, Skresanov Savely, cracker64, Catelite, Bryan Hoyle, Nathan Cousins, jacksonmj,\n"
	"\bgFelix Wallin, Lieuwe Mosch, Anthony Boot, Me4502, MaksProg, jacob1, mniip, LBPHacker\n"
	"\boModder: \bgThe Xphere\n"
	"\n"
#ifndef BETA
	"\bgTo use online features such as saving, you need to register at: \brhttps://powdertoy.co.uk/Register.html\n"
#else
	"\brThis is a BETA, you cannot save things publicly, nor open local saves and stamps made with it in older versions.\n"
	"\brIf you are planning on publishing any saves, use the release version.\n"
#endif
	"\n"
	"\bt" MTOS(SAVE_VERSION) "." MTOS(MINOR_VERSION) "." MTOS(BUILD_NUM) " " IDENT
#ifdef SNAPSHOT
	" SNAPSHOT " MTOS(SNAPSHOT_ID)
#elif MOD_ID > 0
	" MODVER " MTOS(SNAPSHOT_ID)
#endif
#if defined(X86_SSE) || defined(X86_SSE2) || defined(X86_SSE3)
	" " IDENT_BUILD
#endif
#ifdef LUACONSOLE
	" LUACONSOLE"
#endif
#ifdef GRAVFFT
	" GRAVFFT"
#endif
#ifdef REALISTIC
	" REALISTIC"
#endif
#ifdef NOHTTP
	" NOHTTP"
#endif
#ifdef DEBUG
	" DEBUG"
#endif
#ifdef ENFORCE_HTTPS
	" HTTPS"
#endif
	;

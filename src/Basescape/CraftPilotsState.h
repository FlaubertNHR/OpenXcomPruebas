/*
 * Copyright 2010-2015 OpenXcom Developers.
 *
 * This file is part of OpenXcom.
 *
 * OpenXcom is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenXcom is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenXcom.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef OPENXCOM_CRAFTPILOTSSTATE_H
#define OPENXCOM_CRAFTPILOTSSTATE_H

#include "../Engine/State.h"

namespace OpenXcom
{

class Base;
class TextButton;
class Window;
class Text;
class TextList;

/**
 * Displays info about craft pilot(s).
 */
class CraftPilotsState : public State
{
private:
	Base *_base;
	size_t _craft;

	TextButton *_btnOk;
	Window *_window;
	Text *_txtTitle, *_txtFiringAcc, *_txtReactions, *_txtBravery, *_txtPilots;
	Text *_txtRequired, *_txtHint;
	TextList *_lstPilots;
	Text *_txtAccuracyBonus, *_txtAccuracyBonusValue;
	Text *_txtDodgeBonus, *_txtDodgeBonusValue;
	Text *_txtApproachSpeed, *_txtApproachSpeedValue;
public:
	/// Creates the Craft Pilots state.
	CraftPilotsState(Base *base, size_t craft);
	/// Cleans up the Craft Pilots state.
	~CraftPilotsState();
	/// Handler for clicking the OK button.
	void btnOkClick(Action *action);
};

}

#endif

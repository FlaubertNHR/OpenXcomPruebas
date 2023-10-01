/*
 * Copyright 2010-2016 OpenXcom Developers.
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
#include "StatsRecoveryFinishedState.h"
#include "../Engine/Game.h"
#include "../Engine/LocalizedText.h"
#include "../Mod/Mod.h"
#include "../Interface/TextButton.h"
#include "../Interface/Window.h"
#include "../Interface/Text.h"
#include "../Interface/TextList.h"
#include "../Savegame/Base.h"
#include "../Savegame/Soldier.h"
#include "../Basescape/CraftsState.h"
#include "../Engine/Options.h"

namespace OpenXcom
{
/**
 * Initializes all the elements in the Wounds Recovery Finished screen.
 * @param base Pointer to the base to get info from.
 * @param list List of soldiers who finished their recovery
 * @param mana Is mana recovery?
 */
StatsRecoveryFinishedState::StatsRecoveryFinishedState(Base *base, const std::vector<Soldier *> & list, bool mana) : _base(base), _mana(mana)
{
	_screen = false;

	// Create objects
	_window = new Window(this, 288, 180, 16, 10);
	_btnOk = new TextButton(160, 14, 80, 149);
	_btnOpen = new TextButton(160, 14, 80, 165);
	_txtTitle = new Text(288, 33, 16, 20);
	_lstPossibilities = new TextList(250, 80, 35, 60);

	// Set palette
	setInterface("recoveryFinished");

	add(_window, "window", "recoveryFinished");
	add(_btnOk, "button", "recoveryFinished");
	add(_btnOpen, "button", "recoveryFinished");
	add(_txtTitle, "text1", "recoveryFinished");
	add(_lstPossibilities, "text2", "recoveryFinished");

	centerAllSurfaces();

	// Set up objects
	setWindowBackground(_window, "recoveryFinished");

	_btnOk->setText(tr("STR_OK"));
	_btnOk->onMouseClick((ActionHandler)&StatsRecoveryFinishedState::btnOkClick);
	_btnOk->onKeyboardPress((ActionHandler)&StatsRecoveryFinishedState::btnOkClick, Options::keyCancel);
	_btnOpen->setText(tr("STR_EQUIP_CRAFT"));
	_btnOpen->onMouseClick((ActionHandler)&StatsRecoveryFinishedState::btnOpenClick);
	_btnOpen->onKeyboardPress((ActionHandler)&StatsRecoveryFinishedState::btnOpenClick, Options::keyOk);
	_txtTitle->setBig();
	_txtTitle->setAlign(ALIGN_CENTER);
	_txtTitle->setText(tr(_mana ? "STR_MANA_RECOVERY_FINISHED" : "STR_WOUNDS_RECOVERY_FINISHED").arg(base->getName()));

	_lstPossibilities->setColumns(1, 250);
	_lstPossibilities->setBig();
	_lstPossibilities->setAlign(ALIGN_CENTER);
	_lstPossibilities->setScrolling(true, 0);
	for (const auto* soldier : list)
	{
		_lstPossibilities->addRow (1, soldier->getName().c_str());
	}
}

/**
 * Closes the screen.
 * @param action Pointer to an action.
 */
void StatsRecoveryFinishedState::btnOkClick(Action *)
{
	_game->popState();
}

/**
 * Opens the AllocateTrainingState/AllocatePsiTrainingState.
 * @param action Pointer to an action.
 */
void StatsRecoveryFinishedState::btnOpenClick(Action *)
{
	_game->popState();
    _game->pushState(new CraftsState(_base));

}

}

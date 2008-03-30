#include <cstdlib>

#include <iostream>
#include <sstream>

#include <windows.h>

#include "tibia.h"
#include "tibiapacket.h"

/*
Tibia Test for Tibia 8.1
by Evremonde
Last updated on March 29th, 2008
*/

// tibia class
CTibia Tibia;

// tibia packet class
CTibiaPacket TibiaPacket;

int main()
{
	// introduction
	std::cout << "Tibia Test for Tibia 8.1" << std::endl << std::endl;

	// x
	//std::cout << "x: " << Tibia.x << std::endl;

	// findTibia()
	std::cout << "findTibia(): " << Tibia.findTibia() << std::endl;

	// getTibiaWindow()
	std::cout << "getTibiaWindow(): " << Tibia.getTibiaWindow() << std::endl;

	// calcExpForLevel()
	std::cout << "calcExpForLevel(level 8): " << Tibia.calcExpForLevel(8) << std::endl;

	// isOnline()
	std::cout << "isOnline(): " << Tibia.isOnline() << std::endl;

	// isPlayerWalking()
	std::cout << "isPlayerWalking(): " << Tibia.isPlayerWalking() << std::endl;

	// isTargetVisible()
	std::cout << "isTargetVisible(): " << Tibia.isTargetVisible() << std::endl;

	// isFullLightEnabled()
	std::cout << "isFullLightEnabled(): " << Tibia.isFullLightEnabled() << std::endl;

	// isLevelSpyEnabled()
	std::cout << "isLevelSpyEnabled(): " << Tibia.isLevelSpyEnabled() << std::endl;

	// isNameSpyEnabled()
	std::cout << "isNameSpyEnabled(): " << Tibia.isNameSpyEnabled() << std::endl;

	// isShowInvisibleCreaturesEnabled()
	std::cout << "isShowInvisibleCreaturesEnabled(): " << Tibia.isShowInvisibleCreaturesEnabled() << std::endl;

	// getHotkeyType(int hotkeyNumber)
	std::cout << "getHotkeyType(hotkey #1): " << Tibia.getHotkeyType(1) << std::endl;
	std::cout << "getHotkeyType(hotkey #2): " << Tibia.getHotkeyType(2) << std::endl;
	std::cout << "getHotkeyType(hotkey #3): " << Tibia.getHotkeyType(3) << std::endl;

	// getHotkeyByObject(int itemId, HotkeyType_t hotkeyType)
	std::cout << "getHotkeyByObject(rope, use with crosshairs): " << Tibia.getHotkeyByObject(3003, HOTKEY_TYPE_WITH_CROSSHAIRS) << std::endl;

	// getHotkeyByText(std::string hotkeyText)
	std::cout << "getHotkeyByText(asdf123): " << Tibia.getHotkeyByText("asdf123") << std::endl;

	// getExpToLevelUp()
	std::cout << "getExpToLevelUp(): " << Tibia.getExpToLevelUp() << std::endl;

	// getStatusbarText()
	std::cout << "getStatusbarText(): " << Tibia.getStatusbarText() << std::endl;

	// getFpsLimit()
	std::cout << "getFpsLimit(): " << Tibia.getFpsLimit() << std::endl;

	//getConnectionStatus()
	std::cout << "getConnectionStatus(): " << Tibia.getConnectionStatus() << std::endl;

	//getLightAmount()
	std::cout << "getLightAmount(): " << Tibia.getLightAmount() << std::endl;

	// getClickX()
	std::cout << "getClickX(): " << Tibia.getClickX() << std::endl;

	// getClickY()
	std::cout << "getClickY(): " << Tibia.getClickY() << std::endl;

	// getClickZ()
	std::cout << "getClickZ(): " << Tibia.getClickZ() << std::endl;

	// getSeeX()
	std::cout << "getSeeX(): " << Tibia.getSeeX() << std::endl;

	// getSeeY()
	std::cout << "getSeeY(): " << Tibia.getSeeY() << std::endl;

	// getSeeZ()
	std::cout << "getSeeZ(): " << Tibia.getSeeZ() << std::endl;

	// getModeCombat()
	std::cout << "getModeCombat(): " << Tibia.getModeCombat() << std::endl;

	// getModeFollow()
	std::cout << "getModeFollow(): " << Tibia.getModeFollow() << std::endl;

	// getModeSecure()
	std::cout << "getModeSecure(): " << Tibia.getModeSecure() << std::endl;

	// getPlayerStamina()
	std::cout << "getPlayerStamina(): " << Tibia.getPlayerStamina() << std::endl;

	// setFpsLimit()
	//Tibia.setFpsLimit(1);

	// setMouseId()
	//Tibia.setMouseId(MOUSE_USING);

	// setPlayerOutfit()
	//Tibia.setPlayerOutfit(OUTFIT_MALE_NIGHTMARE);

	// setPlayerOutfitAddon()
	//Tibia.setPlayerOutfitAddon(OUTFIT_ADDON_BOTH);
	
	// doTreesToBushes()
	//Tibia.doTreesToBushes();

	// doShowFishingWater()
	//Tibia.doShowFishingWater();

	// doPlayerWalkTo()
	//Location playerLocation = Tibia.getPlayerLocation();
	//playerLocation.x = playerLocation.x + 1;
	//Tibia.doPlayerWalkTo(playerLocation);

	// doPlayerStopTo()
	//Tibia.doPlayerStopTo();

	// wait
	system("pause");

	// exit
	return 0;
}

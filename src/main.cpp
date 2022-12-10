#include "raylib.h"
#include "utilities_raylib.h"

#if defined(PLATFORM_WEB)
    #define CUSTOM_MODAL_DIALOGS            // Force custom modal dialogs usage
    #include <emscripten/emscripten.h>      // Emscripten library - LLVM to JavaScript compiler
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for: 

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif



#include "globals.h"
#include "GameManager.h"



//--------------------

void UpdateDrawFrame()
{
   gameManager.UpdateDrawFrame();
}


bool load_data()
{
	
	if (!load_texture(unknownTexture, "Data/Graphics/unknown.png") ) return false;
	
	if (!load_texture(splashScreenTexture, "Data/Graphics/splash_screen.png") ) return false;
	if (!load_texture(endingTexture, "Data/Graphics/ending.png") ) return false;
	if (!load_texture(tilesTexture, "Data/Graphics/tiles.png") ) return false;
	if (!load_texture(playerTexture, "Data/Graphics/player.png") ) return false;
	if (!load_texture(swordAnimationTexture, "Data/Graphics/sword_animation.png") ) return false;
	if (!load_texture(particleTexture, "Data/Graphics/particle.png") ) return false;
	if (!load_texture(chestTexture, "Data/Graphics/chest.png") ) return false;
	
	
	// enemies
	if (!load_texture(ratTexture, "Data/Graphics/Enemies/rat.png") ) return false;
	if (!load_texture(SlimeMonsterTexture, "Data/Graphics/Enemies/slime.png") ) return false;
	if (!load_texture(goblinTexture, "Data/Graphics/Enemies/goblin.png") ) return false;
	if (!load_texture(impTexture, "Data/Graphics/Enemies/imp.png") ) return false;
	if (!load_texture(skeletonTexture, "Data/Graphics/Enemies/skeleton.png") ) return false;
	if (!load_texture(skeletonArcherTexture, "Data/Graphics/Enemies/skeleton_archer.png") ) return false;
	if (!load_texture(ghostTexture, "Data/Graphics/Enemies/ghost.png") ) return false;
	if (!load_texture(deathBladeTexture, "Data/Graphics/Enemies/death_blade.png") ) return false;
	if (!load_texture(mimicTexture, "Data/Graphics/Enemies/mimic.png") ) return false;
	if (!load_texture(skeletonWarriorTexture, "Data/Graphics/Enemies/skeleton_warrior.png") ) return false;
	if (!load_texture(deathWizardTexture, "Data/Graphics/Enemies/death_wizard.png") ) return false;
	if (!load_texture(dragonTexture, "Data/Graphics/Enemies/dragon.png") ) return false;
	if (!load_texture(reaperTexture, "Data/Graphics/Enemies/reaper.png") ) return false;
	if (!load_texture(bossTexture, "Data/Graphics/Enemies/boss.png") ) return false;
	
	
	// items
	if (!load_texture(coinsTexture, "Data/Graphics/Items/coins.png") ) return false;
	
	if (!load_texture(arrowItemTexture, "Data/Graphics/Items/arrow_item.png") ) return false;
	if (!load_texture(arrowQuiverTexture, "Data/Graphics/Items/arrow_quiver.png") ) return false;
	
	if (!load_texture(hpPotionTexture, "Data/Graphics/Items/hp_potion.png") ) return false;
	if (!load_texture(manaPotionTexture, "Data/Graphics/Items/mana_potion.png") ) return false;
	if (!load_texture(pissPotionTexture, "Data/Graphics/Items/piss_potion.png") ) return false;
	if (!load_texture(poisonPotionTexture, "Data/Graphics/Items/poison_potion.png") ) return false;
	if (!load_texture(explosionPotionTexture, "Data/Graphics/Items/explosion_potion.png") ) return false;
	
	if (!load_texture(shortSwordTexture, "Data/Graphics/Items/short_sword.png") ) return false;
	if (!load_texture(longSwordTexture, "Data/Graphics/Items/long_sword.png") ) return false;
	if (!load_texture(bastardSwordTexture, "Data/Graphics/Items/bastard_sword.png") ) return false;
	if (!load_texture(greatSwordTexture, "Data/Graphics/Items/great_sword.png") ) return false;
	
	if (!load_texture(shortBowTexture, "Data/Graphics/Items/short_bow.png") ) return false;
	if (!load_texture(longBowTexture, "Data/Graphics/Items/long_bow.png") ) return false;
	
	if (!load_texture(leatherArmorTexture, "Data/Graphics/Items/leather_armor.png") ) return false;
	if (!load_texture(chainMailTexture, "Data/Graphics/Items/chain_mail.png") ) return false;
	if (!load_texture(dragonLeatherArmorTexture, "Data/Graphics/Items/dragon_leather_armor.png") ) return false;
	if (!load_texture(plateMailTexture, "Data/Graphics/Items/plate_mail.png") ) return false;
	if (!load_texture(goldenPlateTexture, "Data/Graphics/Items/golden_plate.png") ) return false;
	
	if (!load_texture(bookFireboltTexture, "Data/Graphics/Items/book_firebolt.png") ) return false;
	if (!load_texture(bookFireballTexture, "Data/Graphics/Items/book_fireball.png") ) return false;
	if (!load_texture(bookFrostboltTexture, "Data/Graphics/Items/book_frostbolt.png") ) return false;
	if (!load_texture(bookFreezingCloudTexture, "Data/Graphics/Items/book_freezing_cloud.png") ) return false;
	if (!load_texture(bookLightningTexture, "Data/Graphics/Items/book_lightning.png") ) return false;
	if (!load_texture(bookChainLightningTexture, "Data/Graphics/Items/book_chain_lightning.png") ) return false;
	
	
	// projectiles
	if (!load_texture(arrowTexture, "Data/Graphics/Projectiles/arrow.png") ) return false;
	if (!load_texture(throwingKnifeTexture, "Data/Graphics/Projectiles/throwing_knife.png") ) return false;
	if (!load_texture(fireboltTexture, "Data/Graphics/Projectiles/firebolt.png") ) return false;
	if (!load_texture(fireballTexture, "Data/Graphics/Projectiles/fireball.png") ) return false;
	if (!load_texture(frostboltTexture, "Data/Graphics/Projectiles/frostbolt.png") ) return false;
	if (!load_texture(lightningTexture, "Data/Graphics/Projectiles/lightning.png") ) return false;
	
	
	// hud
	if (!load_texture(barTexture, "Data/Graphics/UI_bar.png") ) return false;
	if (!load_texture(barBackgroundTexture, "Data/Graphics/UI_bar_background.png") ) return false;
	
	if (!load_texture(squareTexture, "Data/Graphics/square.png") ) return false;
	if (!load_texture(hudIconsTexture, "Data/Graphics/hud_icons.png") ) return false;
	if (!load_texture(youdiedTexture, "Data/Graphics/youdied.png") ) return false;
	if (!load_texture(levelUpMenuTexture, "Data/Graphics/level_up_menu.png") ) return false;
	if (!load_texture(selectionArrowTexture, "Data/Graphics/selection_arrow.png") ) return false;
	
	
	// sounds
	if (!load_sound(s_Blip, "Data/Sounds/blip.wav") ) return false;
	if (!load_sound(s_Swing, "Data/Sounds/swing.wav") ) return false;
	if (!load_sound(s_Throw, "Data/Sounds/throw.wav") ) return false;
	if (!load_sound(s_PlayerHurt, "Data/Sounds/player_hurt.wav") ) return false;
	if (!load_sound(s_EnemyHurt, "Data/Sounds/enemy_hurt.wav") ) return false;
	if (!load_sound(s_EnemyDeath, "Data/Sounds/enemy_death.wav") ) return false;
	if (!load_sound(s_Pickup, "Data/Sounds/pickup.wav") ) return false;
	if (!load_sound(s_LevelUp, "Data/Sounds/level_up.wav") ) return false;
	if (!load_sound(s_Open, "Data/Sounds/open.wav") ) return false;
	if (!load_sound(s_Drink, "Data/Sounds/drink.wav") ) return false;
	if (!load_sound(s_NotEnoughMana, "Data/Sounds/not_enough_mana.wav") ) return false;
	if (!load_sound(s_NoAmmo, "Data/Sounds/no_ammo.wav") ) return false;
	if (!load_sound(s_Scream, "Data/Sounds/scream.wav") ) return false;
	if (!load_sound(s_Explode, "Data/Sounds/explode.wav") ) return false;
	if (!load_sound(s_Spell, "Data/Sounds/spell.wav") ) return false;
	if (!load_sound(s_PlayerDeath, "Data/Sounds/player_death.wav") ) return false;
	if (!load_sound(s_EscapeNow, "Data/Sounds/escape_now.wav") ) return false;
	
	
	
	return true;
	
}


void shutdown()
{
	
	UnloadRenderTexture(target);
	
	UnloadTexture(unknownTexture);
	
	UnloadTexture(splashScreenTexture);
	UnloadTexture(endingTexture);
	UnloadTexture(tilesTexture);
	UnloadTexture(playerTexture);
	UnloadTexture(swordAnimationTexture);
	UnloadTexture(particleTexture);
	UnloadTexture(chestTexture);
	
	// enemies
	UnloadTexture(ratTexture);
	UnloadTexture(SlimeMonsterTexture);
	UnloadTexture(goblinTexture);
	UnloadTexture(impTexture);
	UnloadTexture(skeletonTexture);
	UnloadTexture(skeletonArcherTexture);
	UnloadTexture(ghostTexture);
	UnloadTexture(deathBladeTexture);
	UnloadTexture(mimicTexture);
	UnloadTexture(skeletonWarriorTexture);
	UnloadTexture(deathWizardTexture);
	UnloadTexture(dragonTexture);
	UnloadTexture(reaperTexture);
	UnloadTexture(bossTexture);
	
	// items
	UnloadTexture(coinsTexture);
	
	UnloadTexture(arrowItemTexture);
	UnloadTexture(arrowQuiverTexture);
	
	UnloadTexture(hpPotionTexture);
	UnloadTexture(manaPotionTexture);
	UnloadTexture(pissPotionTexture);
	UnloadTexture(poisonPotionTexture);
	UnloadTexture(explosionPotionTexture);
	
	UnloadTexture(shortSwordTexture);
	UnloadTexture(longSwordTexture);
	UnloadTexture(bastardSwordTexture);
	UnloadTexture(greatSwordTexture);
	
	UnloadTexture(shortBowTexture);
	UnloadTexture(longBowTexture);
	
	UnloadTexture(leatherArmorTexture);
	UnloadTexture(chainMailTexture);
	UnloadTexture(dragonLeatherArmorTexture);
	UnloadTexture(plateMailTexture);
	UnloadTexture(goldenPlateTexture);
	
	UnloadTexture(bookFireboltTexture);
	UnloadTexture(bookFireballTexture);
	UnloadTexture(bookFrostboltTexture);
	UnloadTexture(bookFreezingCloudTexture);
	UnloadTexture(bookLightningTexture);
	UnloadTexture(bookChainLightningTexture);
	
	// projectiles
	UnloadTexture(arrowTexture);
	UnloadTexture(throwingKnifeTexture);
	UnloadTexture(fireboltTexture);
	UnloadTexture(fireballTexture);
	UnloadTexture(frostboltTexture);
	UnloadTexture(lightningTexture);
	
	// hud
	UnloadTexture(barTexture);
	UnloadTexture(barBackgroundTexture);
	
	UnloadTexture(squareTexture);
	UnloadTexture(hudIconsTexture);
	UnloadTexture(youdiedTexture);
	UnloadTexture(levelUpMenuTexture);
	UnloadTexture(selectionArrowTexture);
	
	// sounds
	UnloadSound(s_Blip);
	UnloadSound(s_Swing);
	UnloadSound(s_Throw);
	UnloadSound(s_PlayerHurt);
	UnloadSound(s_EnemyHurt);
	UnloadSound(s_EnemyDeath);
	UnloadSound(s_Pickup);
	UnloadSound(s_LevelUp);
	UnloadSound(s_Open);
	UnloadSound(s_Drink);
	UnloadSound(s_NotEnoughMana);
	UnloadSound(s_NoAmmo);
	UnloadSound(s_Scream);
	UnloadSound(s_Explode);
	UnloadSound(s_Spell);
	UnloadSound(s_PlayerDeath);
	UnloadSound(s_EscapeNow);
	
	// core
	CloseAudioDevice();
	CloseWindow(); 
	
}



int main(void)
{
	
	#if !defined(_DEBUG)
		SetTraceLogLevel(LOG_NONE); // Disable raylib trace log messsages
	#endif
	
    InitWindow(screenWidth, screenHeight, "The Chaotic Dungeon");
	InitAudioDevice(); 
    
    if (!load_data() )
	{
		printf("Failed to load data\n");
		return -1;
	}
	
    // Render texture to draw full screen, enables screen scaling
    // NOTE: If screen is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(screenWidth, screenHeight);
    //SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);
	
	//gameManager.start_game();
	

	#if defined(PLATFORM_WEB)
		emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
	#else
		SetTargetFPS(60); 
		while (!WindowShouldClose())    // Detect window close button
		{
			UpdateDrawFrame();
		}
		gameManager.shutdown();
	#endif
	
	shutdown();
    return 0;
	
}

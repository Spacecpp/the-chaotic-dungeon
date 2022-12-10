#include "globals.h"

const int screenWidth = 256;
const int screenHeight = 256;

unsigned int screenScale = 1; 
unsigned int prevScreenScale = 1;

RenderTexture2D target = { 0 };  // Initialized at init

// graphics
Texture2D unknownTexture;

Texture2D splashScreenTexture;
Texture2D endingTexture;
Texture2D tilesTexture;
Texture2D playerTexture;
Texture2D swordAnimationTexture;
Texture2D particleTexture;
Texture2D chestTexture;

// enemies
Texture2D ratTexture;
Texture2D SlimeMonsterTexture;
Texture2D goblinTexture;
Texture2D impTexture;
Texture2D skeletonTexture;
Texture2D skeletonArcherTexture;
Texture2D ghostTexture;
Texture2D deathBladeTexture;
Texture2D mimicTexture;
Texture2D skeletonWarriorTexture;
Texture2D deathWizardTexture;
Texture2D dragonTexture;
Texture2D reaperTexture;
Texture2D bossTexture;

// items
Texture2D coinsTexture;

Texture2D arrowItemTexture;
Texture2D arrowQuiverTexture;

Texture2D hpPotionTexture;
Texture2D manaPotionTexture;
Texture2D pissPotionTexture;
Texture2D poisonPotionTexture;
Texture2D explosionPotionTexture;

Texture2D shortSwordTexture;
Texture2D longSwordTexture;
Texture2D bastardSwordTexture;
Texture2D greatSwordTexture;

Texture2D shortBowTexture;
Texture2D longBowTexture;

Texture2D leatherArmorTexture;
Texture2D chainMailTexture;
Texture2D dragonLeatherArmorTexture;
Texture2D plateMailTexture;
Texture2D goldenPlateTexture;

Texture2D bookFireboltTexture;
Texture2D bookFireballTexture;
Texture2D bookFrostboltTexture;
Texture2D bookFreezingCloudTexture;
Texture2D bookLightningTexture;
Texture2D bookChainLightningTexture;

// projectiles
Texture2D arrowTexture;
Texture2D throwingKnifeTexture;
Texture2D fireboltTexture;
Texture2D fireballTexture;
Texture2D frostboltTexture;
Texture2D lightningTexture;

// hud
Texture2D barTexture;
Texture2D barBackgroundTexture;

Texture2D squareTexture;
Texture2D hudIconsTexture;
Texture2D youdiedTexture;
Texture2D levelUpMenuTexture;
Texture2D selectionArrowTexture;

// sounds
Sound s_Blip;
Sound s_Swing;
Sound s_Throw;
Sound s_PlayerHurt;
Sound s_EnemyHurt;
Sound s_EnemyDeath;
Sound s_Pickup;
Sound s_LevelUp;
Sound s_Open;
Sound s_Drink;
Sound s_NotEnoughMana;
Sound s_NoAmmo;
Sound s_Scream;
Sound s_Explode;
Sound s_Spell;
Sound s_PlayerDeath;
Sound s_EscapeNow;

GameManager gameManager;

#include "raylib.h"
#include "GameManager.h"

extern const int screenWidth;
extern const int screenHeight;

extern unsigned int screenScale; 
extern unsigned int prevScreenScale;

extern RenderTexture2D target;

// graphics
extern Texture2D unknownTexture;

extern Texture2D splashScreenTexture;
extern Texture2D endingTexture;
extern Texture2D tilesTexture;
extern Texture2D playerTexture;
extern Texture2D swordAnimationTexture;
extern Texture2D particleTexture;
extern Texture2D chestTexture;

// enemies
extern Texture2D ratTexture;
extern Texture2D SlimeMonsterTexture;
extern Texture2D goblinTexture;
extern Texture2D impTexture;
extern Texture2D skeletonTexture;
extern Texture2D skeletonArcherTexture;
extern Texture2D ghostTexture;
extern Texture2D deathBladeTexture;
extern Texture2D mimicTexture;
extern Texture2D skeletonWarriorTexture;
extern Texture2D deathWizardTexture;
extern Texture2D dragonTexture;
extern Texture2D reaperTexture;
extern Texture2D bossTexture;

// items
extern Texture2D coinsTexture;

extern Texture2D arrowItemTexture;
extern Texture2D arrowQuiverTexture;

extern Texture2D hpPotionTexture;
extern Texture2D manaPotionTexture;
extern Texture2D pissPotionTexture;
extern Texture2D poisonPotionTexture;
extern Texture2D explosionPotionTexture;

extern Texture2D shortSwordTexture;
extern Texture2D longSwordTexture;
extern Texture2D bastardSwordTexture;
extern Texture2D greatSwordTexture;

extern Texture2D shortBowTexture;
extern Texture2D longBowTexture;

extern Texture2D leatherArmorTexture;
extern Texture2D chainMailTexture;
extern Texture2D dragonLeatherArmorTexture;
extern Texture2D plateMailTexture;
extern Texture2D goldenPlateTexture;

extern Texture2D bookFireboltTexture;
extern Texture2D bookFireballTexture;
extern Texture2D bookFrostboltTexture;
extern Texture2D bookFreezingCloudTexture;
extern Texture2D bookLightningTexture;
extern Texture2D bookChainLightningTexture;


// projectiles
extern Texture2D arrowTexture;
extern Texture2D throwingKnifeTexture;
extern Texture2D fireboltTexture;
extern Texture2D fireballTexture;
extern Texture2D frostboltTexture;
extern Texture2D lightningTexture;

// hud
extern Texture2D barTexture;
extern Texture2D barBackgroundTexture;

extern Texture2D squareTexture;
extern Texture2D hudIconsTexture;
extern Texture2D youdiedTexture;
extern Texture2D levelUpMenuTexture;
extern Texture2D selectionArrowTexture;

// sounds
extern Sound s_Blip;
extern Sound s_Swing;
extern Sound s_Throw;
extern Sound s_PlayerHurt;
extern Sound s_EnemyHurt;
extern Sound s_EnemyDeath;
extern Sound s_Pickup;
extern Sound s_LevelUp;
extern Sound s_Open;
extern Sound s_Drink;
extern Sound s_NotEnoughMana;
extern Sound s_NoAmmo;
extern Sound s_Scream;
extern Sound s_Explode;
extern Sound s_Spell;
extern Sound s_PlayerDeath;
extern Sound s_EscapeNow;



extern GameManager gameManager;

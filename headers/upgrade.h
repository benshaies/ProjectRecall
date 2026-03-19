#ifndef UPGRADE_H
#define UPGRADE_H
#include <raylib.h>

    typedef enum{
        BIGGER_WEAPON,
        FASTER_WEAPON,
        DEFLECT_OFF_WALLS,
        INCREASED_DAMAGE,
        
        INCREASE_PLAYER_SPEED,
    }Upgrades;

    typedef struct{
        bool isActive;
        Rectangle baseRec;
        Rectangle upgradeRecs[3];
        Upgrades upgrade[3];
    }UpgradeScreen;


    void createUpgrades(UpgradeScreen *upgradeScreen);

    void drawUpgrades(UpgradeScreen *upgradeScreen);
    


#endif

#include "World/FGameInstance.h"
#include "Suppot/Managers/FLevelManager.h"

class AFLevelManager *UFGameInstance::GetLevelManager()
{
    if (LevelManagerClass && !IsValid(LevelManagerInstance))
        LevelManagerInstance = NewObject<AFLevelManager>(this, LevelManagerClass, TEXT("LevelManager"));

    return IsValid(LevelManagerInstance) ? LevelManagerInstance : LevelManagerInstance = NewObject<AFLevelManager>(this, FName("LevelManager"));
}

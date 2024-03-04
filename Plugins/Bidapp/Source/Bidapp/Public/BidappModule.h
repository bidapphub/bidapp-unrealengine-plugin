#pragma once

#include "Modules/ModuleManager.h"

class FBidappModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};


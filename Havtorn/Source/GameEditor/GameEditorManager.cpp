// Copyright 2026 Team Havtorn. All Rights Reserved.

#include "GameEditorManager.h"

#include "Ghosty/GhostyComponentView.h"
#include "Ghosty/SGhostyNodeView.h"

#include "HexRune/InterpolatePositionNodeView.h"
#include "HexRune/TimerNodeView.h"
#include "HexRune/SetPositionNodeView.h"

#include <Ghosty/GhostyComponent.h>
#include <GhostyNode.h>
#include <HexRune/InterpolatePosition.h>
#include <HexRune/TimerNode.h>
#include <HexRune/SetPositionNode.h>

namespace Havtorn
{
    bool CGameEditorManager::Init(CPlatformManager* platformManager, CRenderManager* renderManager)
    {
        bool returnValue = CEditorManager::Init(platformManager, renderManager);
        if (!returnValue)
            return false;

        RegisterComponentView<SGhostyComponentView, SGhostyComponent>();
        
        RegisterNodeView<SGhostyNodeView, HexRune::SGhostyNode>();
        RegisterNodeView<SInterpolatePositionNodeView, HexRune::SInterpolatePosition>();
        RegisterNodeView<STimerNodeView, HexRune::STimerNode>();
        RegisterNodeView<SSetPositionNodeView, HexRune::SSetPositionNode>();

        return true;
    }
}

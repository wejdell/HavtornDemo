// Copyright 2024 Team Havtorn. All Rights Reserved.

#pragma once
#include "GhostyComponentView.h"

#include <Ghosty/GhostyComponent.h>

#include <Scene/Scene.h>
#include <GUI.h>

namespace Havtorn
{
	void SGhostyComponentView::View(const SEntity& entityOwner, CScene* scene) const
	{
		SGhostyComponent* component = scene->GetComponent<SGhostyComponent>(entityOwner);

		GUI::DragFloat3("GhostyState", component->State.Input, 0.0f);

		GUI::Checkbox("IsInWalkingAnimation", component->State.IsInWalkingAnimationState);
	}
}

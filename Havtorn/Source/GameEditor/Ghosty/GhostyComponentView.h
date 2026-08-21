// Copyright 2024 Team Havtorn. All Rights Reserved.

#pragma once
#include <ComponentView.h>

namespace Havtorn
{
	struct SGhostyComponentView : public SComponentView
	{
		void View(const SEntity& entityOwner, CScene* scene) const override;
		virtual const char* GetComponentName() const override { return "Ghosty"; };
	};
}

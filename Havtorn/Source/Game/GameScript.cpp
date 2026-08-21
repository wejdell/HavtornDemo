// Copyright 2025 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "GameScript.h"
#include "NodeInclude.h"

#include <Scene/World.h>

namespace Havtorn
{
	SGameScript::SGameScript()
		: SScript()
	{
	}

	SGameScript::~SGameScript()
	{
	}

	void SGameScript::Init()
	{
		HexRune::SScript::Init();

		NodeFactory->RegisterNodeType<HexRune::SGhostyNode>(100010);
		NodeFactory->RegisterNodeType<HexRune::SGhostyPositionNode>(100020);
		NodeFactory->RegisterNodeType<HexRune::SInterpolatePosition>(100030);
		NodeFactory->RegisterNodeType<HexRune::STimerNode>(100040);
		NodeFactory->RegisterNodeType<HexRune::SSetPositionNode>(100050);
	}
}

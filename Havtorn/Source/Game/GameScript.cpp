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

		NodeFactory->RegisterNodeType<HexRune::SGhostyNode, HexRune::SGhostyNodeEditorContext>(this, 100010);
		NodeFactory->RegisterNodeType<HexRune::SGhostyPositionNode, HexRune::SGhostyNodePositionEditorContex>(this, 100020);
		NodeFactory->RegisterNodeType<HexRune::SInterpolatePosition, HexRune::SInterpolatePositionNodeEditorContext>(this, 100030);
		NodeFactory->RegisterNodeType<HexRune::STimerNode, HexRune::STimerNodeEditorContext>(this, 100040);
		NodeFactory->RegisterNodeType<HexRune::SSetPositionNode, HexRune::SSetPositionNodeEditorContext>(this, 100050);
	}
}

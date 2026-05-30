// Copyright 2024 Team Havtorn. All Rights Reserved.

#include "hvpch.h"
#include "GameScene.h"
#include "Engine.h"

#include "Ghosty/GhostyComponent.h"
#include "Ghosty/GhostyComponentEditorContext.h"

namespace Havtorn
{
	bool CGameScene::Init(const std::string& sceneName)
	{
		if (!CScene::Init(sceneName))
			return false;

		RegisterTrivialComponent<SGhostyComponent, SGhostyComponentEditorContext>(100000, 1);

		return true;
	}

	std::vector<SVector4> CreateAnimationClip(const F32 width, const F32 height, const F32 frameSize, const U32 row, const U32 column, const U32 frameCount)
	{
		std::vector<SVector4> uvRects;
		F32 normalizedFrameSize = frameSize / width;

		for (U32 i = 0; i < frameCount; i++)
		{
			F32 x = (column + i) * normalizedFrameSize;
			F32 y = row * (frameSize / height);
			F32 z = x + normalizedFrameSize;
			F32 w = y + (frameSize / height);

			uvRects.push_back(SVector4{ x, y, z, w });
		}

		return uvRects;
	}

	void CGameScene::OpenDefault()
    {
		CWorld* world = GEngine::GetWorld();

		if (world->GetWorldPlayDimensions() == EWorldPlayDimensions::World3D)
		{
			if (!Init("3DDemoScene"))
				return;

			if (!Init3DDefaults())
				return;

			SDirectionalLightComponent* directionalLightComponent = GetComponents<SDirectionalLightComponent>()[0];
			directionalLightComponent->Color = { 212.0f / 255.0f, 175.0f / 255.0f, 55.0f / 255.0f, 0.25f };

			SEnvironmentLightComponent* environmentLightComponent = GetComponents<SEnvironmentLightComponent>()[0];
			environmentLightComponent->AssetReference = SAssetReference("Assets/Textures/Cubemaps/CubemapTheVisit.hva");

			// === Point light ===
			const SEntity& pointLightEntity = AddEntity("Point Light");
			if (!pointLightEntity.IsValid())
				return; // From this point it's ok if we fail to load the rest of the demo scene

			STransformComponent& pointLightTransform = *AddComponent<STransformComponent>(pointLightEntity);
			AddComponentEditorContext(pointLightEntity, &STransformComponentEditorContext::Context);
			SMatrix pointLightMatrix = pointLightTransform.Transform.GetMatrix();
			pointLightMatrix.SetTranslation({ 1.75f, 0.35f, -2.15f });
			pointLightTransform.Transform.SetMatrix(pointLightMatrix);

			SPointLightComponent& pointLightComp = *AddComponent<SPointLightComponent>(pointLightEntity);
			AddComponentEditorContext(pointLightEntity, &SPointLightComponentEditorContext::Context);
			pointLightComp.ColorAndIntensity = { 0.0f, 1.0f, 1.0f, 10.0f };
			pointLightComp.Range = 1.0f;

			SVolumetricLightComponent& volumetricPointLight = *AddComponent<SVolumetricLightComponent>(pointLightEntity);
			AddComponentEditorContext(pointLightEntity, &SVolumetricLightComponentEditorContext::Context);
			volumetricPointLight.IsActive = false;

			const SMatrix constantProjectionMatrix = SMatrix::PerspectiveFovLH(UMath::DegToRad(90.0f), 1.0f, 0.001f, pointLightComp.Range);
			const SVector4 constantPosition = pointLightTransform.Transform.GetMatrix().GetTranslation4();

			// Forward
			SShadowmapViewData& view1 = pointLightComp.ShadowmapViews[0];
			view1.ShadowPosition = constantPosition;
			view1.ShadowmapViewportIndex = 1;
			view1.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Forward).ToVector3(), SVector::Up);
			view1.ShadowProjectionMatrix = constantProjectionMatrix;

			// Right
			SShadowmapViewData& view2 = pointLightComp.ShadowmapViews[1];
			view2.ShadowPosition = constantPosition;
			view2.ShadowmapViewportIndex = 2;
			view2.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Right).ToVector3(), SVector::Up);
			view2.ShadowProjectionMatrix = constantProjectionMatrix;

			// Backward
			SShadowmapViewData& view3 = pointLightComp.ShadowmapViews[2];
			view3.ShadowPosition = constantPosition;
			view3.ShadowmapViewportIndex = 3;
			view3.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Backward).ToVector3(), SVector::Up);
			view3.ShadowProjectionMatrix = constantProjectionMatrix;

			// Left
			SShadowmapViewData& view4 = pointLightComp.ShadowmapViews[3];
			view4.ShadowPosition = constantPosition;
			view4.ShadowmapViewportIndex = 4;
			view4.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Left).ToVector3(), SVector::Up);
			view4.ShadowProjectionMatrix = constantProjectionMatrix;

			// Up
			SShadowmapViewData& view5 = pointLightComp.ShadowmapViews[4];
			view5.ShadowPosition = constantPosition;
			view5.ShadowmapViewportIndex = 5;
			view5.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Up).ToVector3(), SVector::Backward);
			view5.ShadowProjectionMatrix = constantProjectionMatrix;

			// Down
			SShadowmapViewData& view6 = pointLightComp.ShadowmapViews[5];
			view6.ShadowPosition = constantPosition;
			view6.ShadowmapViewportIndex = 6;
			view6.ShadowViewMatrix = SMatrix::LookAtLH(constantPosition.ToVector3(), (constantPosition + SVector4::Down).ToVector3(), SVector::Forward);
			view6.ShadowProjectionMatrix = constantProjectionMatrix;
			// === !Point light ===

			// === Spotlight ===
			const SEntity& spotlight = AddEntity("Spot Light");
			if (!spotlight.IsValid())
				return;

			STransform& spotlightTransform = (*AddComponent<STransformComponent>(spotlight)).Transform;
			AddComponentEditorContext(spotlight, &STransformComponentEditorContext::Context);
			SMatrix spotlightMatrix = spotlightTransform.GetMatrix();
			spotlightMatrix.SetTranslation({ 2.0f, 0.5f, -1.5f });
			spotlightTransform.SetMatrix(spotlightMatrix);

			SSpotLightComponent& spotlightComp = *AddComponent<SSpotLightComponent>(spotlight);
			AddComponentEditorContext(spotlight, &SSpotLightComponentEditorContext::Context);
			spotlightComp.Direction = SVector4::Forward;
			spotlightComp.DirectionNormal1 = SVector4::Right;
			spotlightComp.DirectionNormal2 = SVector4::Up;
			spotlightComp.ColorAndIntensity = { 0.0f, 1.0f, 0.0f, 5.0f };
			spotlightComp.OuterAngle = 25.0f;
			spotlightComp.InnerAngle = 5.0f;
			spotlightComp.Range = 3.0f;

			SVolumetricLightComponent& volumetricSpotLight = *AddComponent<SVolumetricLightComponent>(spotlight);
			AddComponentEditorContext(spotlight, &SVolumetricLightComponentEditorContext::Context);
			volumetricSpotLight.IsActive = false;

			const SMatrix spotlightProjection = SMatrix::PerspectiveFovLH(UMath::DegToRad(90.0f), 1.0f, 0.001f, spotlightComp.Range);
			const SVector4 spotlightPosition = spotlightTransform.GetMatrix().GetTranslation4();

			spotlightComp.ShadowmapView.ShadowPosition = spotlightPosition;
			spotlightComp.ShadowmapView.ShadowmapViewportIndex = 7;
			spotlightComp.ShadowmapView.ShadowViewMatrix = SMatrix::LookAtLH(spotlightPosition.ToVector3(), (spotlightPosition + spotlightComp.Direction).ToVector3(), spotlightComp.DirectionNormal2.ToVector3());
			spotlightComp.ShadowmapView.ShadowProjectionMatrix = spotlightProjection;
			// === !Spotlight ===

			// === Decal ===
			const SEntity& decal = AddEntity("Decal");
			if (!decal.IsValid())
				return;

			STransform& decalTransform = (*AddComponent<STransformComponent>(decal)).Transform;
			AddComponentEditorContext(decal, &STransformComponentEditorContext::Context);
			decalTransform.Translate({ 0.75f, 1.60f, 0.35f });

			// TODO.NW: Still want to use a Material in decals instead of this texture setup
			std::vector<std::string> decalTextures = { "Assets/Textures/T_noscare_AL_c.hva", "Assets/Textures/T_noscare_AL_m.hva", "Assets/Textures/T_noscare_AL_n.hva" };
			SDecalComponent& decalComp = *AddComponent<SDecalComponent>(decal, decalTextures);
			AddComponentEditorContext(decal, &SDecalComponentEditorContext::Context);

			decalComp.ShouldRenderAlbedo = true;
			decalComp.ShouldRenderMaterial = true;
			decalComp.ShouldRenderNormal = true;

			// === !Decal ===

			const std::string modelPath1 = "Assets/Meshes/En_P_PendulumClock.hva";
			const std::vector<std::string> materialNames1 = { "Assets/Materials/M_PendulumClock.hva", "Assets/Materials/M_Misc.hva" };
			const std::string modelPath2 = "Assets/Meshes/En_P_Bed.hva";
			const std::vector<std::string> materialNames2 = { "Assets/Materials/M_Bed.hva", "Assets/Materials/M_Bedsheet.hva" };
			const std::string modelPath3 = "Assets/Meshes/Plane.hva";
			const std::vector<std::string> materialNames3 = { "Assets/Materials/M_Quad.hva" };
			const std::string modelPath4 = "Assets/Meshes/En_P_WallLamp.hva";
			const std::vector<std::string> materialNames4 = { "Assets/Materials/M_Quad.hva", "Assets/Materials/M_Emissive.hva", "Assets/Materials/M_Headlamp.hva" };
			const std::string modelPath5 = "Assets/Meshes/Cube_1.hva";
			const std::vector<std::string> materialNames5 = { "Assets/Materials/M_Quad.hva" };

			// === Pendulum ===
			const SEntity& pendulum = AddEntity("Clock");
			if (!pendulum.IsValid())
				return;

			STransform& transform1 = (*AddComponent<STransformComponent>(pendulum)).Transform;
			AddComponentEditorContext(pendulum, &STransformComponentEditorContext::Context);
			transform1.Translate({ 1.8f, 0.0f, -0.2f });

			AddComponent<SStaticMeshComponent>(pendulum, modelPath1);
			AddComponentEditorContext(pendulum, &SStaticMeshComponentEditorContext::Context);
			AddComponent<SMaterialComponent>(pendulum, materialNames1);
			AddComponentEditorContext(pendulum, &SMaterialComponentEditorContext::Context);

			SPhysics3DComponent* clockPhysics = AddComponent<SPhysics3DComponent>(pendulum);
			AddComponentEditorContext(pendulum, &SPhysics3DComponentEditorContext::Context);

			clockPhysics->BodyType = EPhysics3DBodyType::Static;
			clockPhysics->ShapeType = EPhysics3DShapeType::Box;
			clockPhysics->ShapeLocalExtents = SVector(0.6f, 1.9f, 0.3f);
			clockPhysics->ShapeLocalOffset = SVector(0.0f, 0.95f, 0.0f);
			// === !Pendulum ===

			// === Bed ===
			const SEntity& bed = AddEntity("Bed");
			if (!bed.IsValid())
				return;

			STransform& transform2 = (*AddComponent<STransformComponent>(bed)).Transform;
			AddComponentEditorContext(bed, &STransformComponentEditorContext::Context);
			transform2.Translate({ 0.2f, 0.0f, 0.0f });

			AddComponent<SStaticMeshComponent>(bed, modelPath2);
			AddComponentEditorContext(bed, &SStaticMeshComponentEditorContext::Context);
			AddComponent<SMaterialComponent>(bed, materialNames2);
			AddComponentEditorContext(bed, &SMaterialComponentEditorContext::Context);

			SPhysics3DComponent* bedPhysics = AddComponent<SPhysics3DComponent>(bed);
			AddComponentEditorContext(bed, &SPhysics3DComponentEditorContext::Context);

			bedPhysics->BodyType = EPhysics3DBodyType::Static;
			bedPhysics->ShapeType = EPhysics3DShapeType::Box;
			bedPhysics->ShapeLocalExtents = SVector(1.8f, 0.7f, 2.5f);
			bedPhysics->ShapeLocalOffset = SVector(0.0f, bedPhysics->ShapeLocalExtents.Y * 0.5f, -bedPhysics->ShapeLocalExtents.Z * 0.5f);
			// === !Bed ===

			// === Lamp ===
			const SEntity& lamp = AddEntity("Lamp");
			if (!lamp.IsValid())
				return;

			STransform& lampTransform = (*AddComponent<STransformComponent>(lamp)).Transform;
			AddComponentEditorContext(lamp, &STransformComponentEditorContext::Context);
			lampTransform.Translate({ -1.0f, 1.4f, -1.25f });
			lampTransform.Rotate({ 0.0f, UMath::DegToRad(90.0f), 0.0f });

			AddComponent<SStaticMeshComponent>(lamp, modelPath4);
			AddComponentEditorContext(lamp, &SStaticMeshComponentEditorContext::Context);
			AddComponent<SMaterialComponent>(lamp, materialNames4);
			AddComponentEditorContext(lamp, &SMaterialComponentEditorContext::Context);
			// === !Lamp ===

			// === Player Proxy ===
			const SEntity& playerProxy = AddEntity("Player");
			if (!playerProxy.IsValid())
				return;

			STransform& playerTransform = AddComponent<STransformComponent>(playerProxy)->Transform;
			AddComponentEditorContext(playerProxy, &STransformComponentEditorContext::Context);
			SMatrix playerMatrix = playerTransform.GetMatrix();
			//playerMatrix.SetScale(SVector(0.01f));
			playerMatrix.SetTranslation({ 2.6f, 0.0f, -0.24f });
			playerTransform.SetMatrix(playerMatrix);

			SPhysics3DControllerComponent* controllerComponent = AddComponent<SPhysics3DControllerComponent>(playerProxy);
			AddComponentEditorContext(playerProxy, &SPhysics3DControllerComponentEditorContext::Context);

			controllerComponent->ControllerType = EPhysics3DControllerType::Capsule;
			controllerComponent->ShapeLocalRadiusAndHeight = SVector2(0.25f, 1.0f);

			// Skeletal Mesh
			std::string meshPath = "Assets/Meshes/CH_Enemy_SK.hva";
			AddComponent<SSkeletalMeshComponent>(playerProxy, meshPath);
			AddComponentEditorContext(playerProxy, &SSkeletalMeshComponentEditorContext::Context);

			const std::vector<std::string> animationPaths = { "Assets/Meshes/CH_Enemy_Walk.hva", "Assets/Meshes/CH_Enemy_Chase.hva" };
			SSkeletalAnimationComponent* animationComponent = AddComponent<SSkeletalAnimationComponent>(playerProxy, animationPaths);
			AddComponentEditorContext(playerProxy, &SSkeletalAnimationComponentEditorContext::Context);

			animationComponent->PlayData.emplace_back(SSkeletalAnimationPlayData());
			animationComponent->PlayData.emplace_back(SSkeletalAnimationPlayData(1));

			std::vector<std::string> enemyMaterialPaths = { "Assets/Materials/M_Enemy.hva" };
			AddComponent<SMaterialComponent>(playerProxy, enemyMaterialPaths);
			AddComponentEditorContext(playerProxy, &SMaterialComponentEditorContext::Context);
			// === !Player Proxy ===

			// === Crate ===
			const SEntity& crate = AddEntity("Crate");
			if (!crate.IsValid())
				return;

			STransform& crateTransform = AddComponent<STransformComponent>(crate)->Transform;
			AddComponentEditorContext(crate, &STransformComponentEditorContext::Context);
			SMatrix crateMatrix = crateTransform.GetMatrix();
			SMatrix::Recompose(SVector(1.0f, 4.7f, -1.5f), SVector(45.0f, 0.0f, 45.0f), SVector(0.5f), crateMatrix);
			crateTransform.SetMatrix(crateMatrix);

			AddComponent<SStaticMeshComponent>(crate, modelPath5);
			AddComponentEditorContext(crate, &SStaticMeshComponentEditorContext::Context);
			AddComponent<SMaterialComponent>(crate, materialNames5);
			AddComponentEditorContext(crate, &SMaterialComponentEditorContext::Context);

			SPhysics3DComponent* cratePhysics = AddComponent<SPhysics3DComponent>(crate);
			AddComponentEditorContext(crate, &SPhysics3DComponentEditorContext::Context);

			cratePhysics->BodyType = EPhysics3DBodyType::Dynamic;
			cratePhysics->ShapeType = EPhysics3DShapeType::Box;
			cratePhysics->ShapeLocalExtents = SVector(0.5f);
			// === !Crate ===

			// === Trigger ===
			const SEntity& trigger = AddEntity("Trigger");
			if (!trigger.IsValid())
				return;

			STransform& triggerTransform = AddComponent<STransformComponent>(trigger)->Transform;
			AddComponentEditorContext(trigger, &STransformComponentEditorContext::Context);
			triggerTransform.Translate({ 0.2f, 1.0f, -1.25f });

			SPhysics3DComponent* triggerPhysics = AddComponent<SPhysics3DComponent>(trigger);
			AddComponentEditorContext(trigger, &SPhysics3DComponentEditorContext::Context);

			triggerPhysics->BodyType = EPhysics3DBodyType::Static;
			triggerPhysics->ShapeType = EPhysics3DShapeType::Box;
			triggerPhysics->ShapeLocalExtents = SVector(1.6f, 0.5f, 2.3f);
			triggerPhysics->IsTrigger = true;
			// === !Trigger ===

			// === Room ===
			const SEntity& room = AddEntity("Room");
			STransformComponent* roomTransform = AddComponent<STransformComponent>(room);
			AddComponentEditorContext(room, &STransformComponentEditorContext::Context);
			// === !Room ===

			// === Floor/Walls ===
			struct SWallAndFloorInitData
			{
				SWallAndFloorInitData(const SVector& translation, const SVector& eulerAngles, std::string editorName)
					: Translation(translation), EulerAngles(eulerAngles), EditorName(std::move(editorName))
				{
				}

				SVector Translation = SVector::Zero;
				SVector EulerAngles = SVector::Zero;
				std::string EditorName;
			};

			std::vector<SWallAndFloorInitData> initData;
			SVector floorRotation = SVector{ 90.0f, 0.0f, 0.0f };
			SVector largeWallRotation = SVector{ 0.0f, 0.0f, 0.0f };
			// TODO.NW: There's still a singularity happening here, need to figure out why
			SVector smallWallRotation = SVector{ 0.0f, -90.0f, 0.0f };

			initData.emplace_back(SVector{ -0.5f, 0.0f, -2.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ 0.5f, 0.0f, -2.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ 1.5f, 0.0f, -2.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ 2.5f, 0.0f, -2.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ -0.5f, 0.0f, -1.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ 0.5f, 0.0f, -1.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ 1.5f, 0.0f, -1.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ 2.5f, 0.0f, -1.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ -0.5f, 0.0f, -0.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ 0.5f, 0.0f, -0.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ 1.5f, 0.0f, -0.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ 2.5f, 0.0f, -0.5f }, floorRotation, std::string("Floor"));
			initData.emplace_back(SVector{ -0.5f, 0.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ 0.5f, 0.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ 1.5f, 0.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ 2.5f, 0.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ -0.5f, 1.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ 0.5f, 1.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ 1.5f, 1.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ 2.5f, 1.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ -0.5f, 2.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ 0.5f, 2.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ 1.5f, 2.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ 2.5f, 2.5f, 0.0f }, largeWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ -1.0f, 0.5f, -2.5f }, smallWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ -1.0f, 0.5f, -1.5f }, smallWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ -1.0f, 0.5f, -0.5f }, smallWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ -1.0f, 1.5f, -2.5f }, smallWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ -1.0f, 1.5f, -1.5f }, smallWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ -1.0f, 1.5f, -0.5f }, smallWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ -1.0f, 2.5f, -2.5f }, smallWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ -1.0f, 2.5f, -1.5f }, smallWallRotation, std::string("Wall"));
			initData.emplace_back(SVector{ -1.0f, 2.5f, -0.5f }, smallWallRotation, std::string("Wall"));

			for (const SWallAndFloorInitData& data : initData)
			{
				const SEntity& entity = AddEntity(data.EditorName);
				if (!entity.IsValid())
					return;

				STransformComponent* transformComponent = AddComponent<STransformComponent>(entity);
				STransform& transform3 = transformComponent->Transform;
				AddComponentEditorContext(entity, &STransformComponentEditorContext::Context);
				SMatrix matrix3 = transform3.GetMatrix();
				matrix3.SetTranslation(data.Translation);
				matrix3.SetRotation(data.EulerAngles);
				transform3.SetMatrix(matrix3);

				AddComponent<SStaticMeshComponent>(entity, modelPath3);
				AddComponentEditorContext(entity, &SStaticMeshComponentEditorContext::Context);
				AddComponent<SMaterialComponent>(entity, materialNames3);
				AddComponentEditorContext(entity, &SMaterialComponentEditorContext::Context);

				SPhysics3DComponent* physicsComponent = AddComponent<SPhysics3DComponent>(entity);
				AddComponentEditorContext(entity, &SPhysics3DComponentEditorContext::Context);

				physicsComponent->BodyType = EPhysics3DBodyType::Static;
				physicsComponent->ShapeType = EPhysics3DShapeType::Box;
				physicsComponent->ShapeLocalExtents = SVector(1.0f, 1.0f, 0.1f);

				roomTransform->Attach(transformComponent);
			}
			// === !Floor/Walls ===

			// === Main Menu ===
			const SEntity& mainMenuCanvas = AddEntity("Main Menu Canvas", MainMenuEntityGUID);
			const SEntity& settingsMenuCanvas = AddEntity("Settings Canvas");

			if (!mainMenuCanvas.IsValid() || !settingsMenuCanvas.IsValid())
				return;

			STransform2DComponent& mainMenuCanvasTransformComp = *AddComponent<STransform2DComponent>(mainMenuCanvas);
			AddComponentEditorContext(mainMenuCanvas, &STransform2DComponentEditorContext::Context);

			mainMenuCanvasTransformComp.Position = { 0.5f, 0.5f };

			SUICanvasComponent& mainMenuCanvasComponent = *AddComponent<SUICanvasComponent>(mainMenuCanvas);
			AddComponentEditorContext(mainMenuCanvas, &SUICanvasComponentEditorContext::Context);

			const std::vector<SAssetReference> assets = { SAssetReference("Assets/Textures/UITextures.hva"), SAssetReference("Assets/Textures/UITextures.hva"), SAssetReference("Assets/Textures/UITextures.hva") };
			const SVector2<F32> buttonScale = SVector2<F32>(0.3f, 0.1f);
			const F32 halfHeight = 0.5f * buttonScale.Y;
			const F32 halfWidth = 0.5f * buttonScale.X * (9.0f / 16.0f);
			const SVector4 collisionRect = { -halfWidth, -halfHeight, halfWidth, halfHeight };

			SUIElement& playButton = mainMenuCanvasComponent.Elements.emplace_back();
			playButton.StateAssetReferences = assets;
			playButton.LocalPosition = SVector2<F32>(0.0f, 0.3f);
			playButton.LocalScale = buttonScale;
			playButton.CollisionRect = collisionRect;
			playButton.UVRects = { SVector4(0.0f, 0.0f, 0.5f, 1 / 8.0f), SVector4(0.5f, 0.0f, 1.0f, 1 / 8.0f), SVector4(0.0f, 1 / 8.0f, 0.5f, 2 / 8.0f) };
			playButton.BindingType = EUIBindingType::NamedFunction;
			playButton.BoundData = std::hash<std::string>{}("CGameManager::PlayGame");

			SUIElement& settingsButton = mainMenuCanvasComponent.Elements.emplace_back();
			settingsButton.StateAssetReferences = assets;
			settingsButton.LocalPosition = SVector2<F32>(0.0f, 0.1f);
			settingsButton.LocalScale = buttonScale;
			settingsButton.CollisionRect = collisionRect;
			settingsButton.UVRects = { SVector4(0.5f, 1 / 8.0f, 1.0f, 2 / 8.0f), SVector4(0.0f, 2 / 8.0f, 0.5f, 3 / 8.0f), SVector4(0.5f, 2 / 8.0f, 1.0f, 3 / 8.0f) };
			settingsButton.BindingType = EUIBindingType::OtherCanvas;
			settingsButton.BoundData = settingsMenuCanvas.GUID;

			SUIElement& quitButton = mainMenuCanvasComponent.Elements.emplace_back();
			quitButton.StateAssetReferences = assets;
			quitButton.LocalPosition = SVector2<F32>(0.0f, -0.1f);
			quitButton.LocalScale = buttonScale;
			quitButton.CollisionRect = collisionRect;
			quitButton.UVRects = { SVector4(0.0f, 3 / 8.0f, 0.5f, 4 / 8.0f), SVector4(0.5f, 3 / 8.0f, 1.0f, 4 / 8.0f), SVector4(0.0f, 4 / 8.0f, 0.5f, 5 / 8.0f) };
			quitButton.BindingType = EUIBindingType::NamedFunction;
			quitButton.BoundData = std::hash<std::string>{}("CGameManager::QuitGame");
			// === !Main Menu ===

			// === Settings Menu ===
			if (!settingsMenuCanvas.IsValid())
				return;

			STransform2DComponent& settingsMenuCanvasTransformComp = *AddComponent<STransform2DComponent>(settingsMenuCanvas);
			AddComponentEditorContext(settingsMenuCanvas, &STransform2DComponentEditorContext::Context);

			settingsMenuCanvasTransformComp.Position = { 0.5f, 0.5f };

			SUICanvasComponent& settingsMenuCanvasComponent = *AddComponent<SUICanvasComponent>(settingsMenuCanvas);
			AddComponentEditorContext(settingsMenuCanvas, &SUICanvasComponentEditorContext::Context);

			SUIElement& muteButton = settingsMenuCanvasComponent.Elements.emplace_back();
			muteButton.StateAssetReferences = assets;
			muteButton.LocalPosition = SVector2<F32>(0.0f, 0.3f);
			muteButton.LocalScale = buttonScale;
			muteButton.CollisionRect = collisionRect;
			muteButton.UVRects = { SVector4(0.5f, 4 / 8.0f, 1.0f, 5 / 8.0f), SVector4(0.0f, 5 / 8.0f, 0.5f, 6 / 8.0f), SVector4(0.5f, 5 / 8.0f, 1.0f, 6 / 8.0f) };

			SUIElement& backButton = settingsMenuCanvasComponent.Elements.emplace_back();
			backButton.StateAssetReferences = assets;
			backButton.LocalPosition = SVector2<F32>(0.0f, 0.1f);
			backButton.LocalScale = buttonScale;
			backButton.CollisionRect = collisionRect;
			backButton.UVRects = { SVector4(0.0f, 6 / 8.0f, 0.5f, 7 / 8.0f), SVector4(0.5f, 6 / 8.0f, 1.0f, 7 / 8.0f), SVector4(0.0f, 7 / 8.0f, 0.5f, 8 / 8.0f) };
			backButton.BindingType = EUIBindingType::OtherCanvas;
			backButton.BoundData = mainMenuCanvas.GUID;
			// === !Settings Menu ===
		}
		else
		{
			if (!Init("2DDemoScene"))
				return;

			// === Camera ===
			{
				SEntity mainCamera = AddEntity("Camera");
				if (!mainCamera.IsValid())
					return;

				// Setup entities (create components)
				STransformComponent& transform = (*AddComponent<STransformComponent>(mainCamera));
				AddComponentEditorContext(mainCamera, &STransformComponentEditorContext::Context);
				transform.Transform.Translate({ 0.0f, 1.0f, -5.0f });
				transform.Transform.Translate(SVector::Right * 0.25f);

				SCameraComponent& camera = *AddComponent<SCameraComponent>(mainCamera);
				AddComponentEditorContext(mainCamera, &SCameraComponentEditorContext::Context);
				camera.ProjectionMatrix = SMatrix::PerspectiveFovLH(UMath::DegToRad(70.0f), (16.0f / 9.0f), 0.1f, 1000.0f);
				camera.IsActive = true;
				camera.IsStartingCamera = true;

				AddComponent<SCameraControllerComponent>(mainCamera);
				AddComponentEditorContext(mainCamera, &SCameraControllerComponentEditorContext::Context);
			}
			// === !Camera ===

			// === Game Camera ===
			{
				SEntity gameCamera = AddEntity("Game Camera");

				// Setup entities (create components)
				STransformComponent& transform = *AddComponent<STransformComponent>(gameCamera);
				AddComponentEditorContext(gameCamera, &STransformComponentEditorContext::Context);

				transform.Transform.Translate({ 1.5f, 1.0f, -3.0f });

				SCameraComponent& camera = *AddComponent<SCameraComponent>(gameCamera);
				AddComponentEditorContext(gameCamera, &SCameraComponentEditorContext::Context);
				camera.ProjectionMatrix = SMatrix::PerspectiveFovLH(UMath::DegToRad(70.0f), (16.0f / 9.0f), 0.1f, 6.0f);
				camera.FarClip = 6.0f;

				AddComponent<SCameraControllerComponent>(gameCamera);
				AddComponentEditorContext(gameCamera, &SCameraControllerComponentEditorContext::Context);
			}

			// === Environment light ===
			const SEntity& environmentLightEntity = AddEntity("Environment Light");
			if (!environmentLightEntity.IsValid())
				return;

			AddComponent<STransformComponent>(environmentLightEntity);
			AddComponentEditorContext(environmentLightEntity, &STransformComponentEditorContext::Context);
			AddComponent<SEnvironmentLightComponent>(environmentLightEntity, "Assets/Textures/Cubemaps/CubemapTheVisit.hva");
			AddComponentEditorContext(environmentLightEntity, &SEnvironmentLightComponentEditorContext::Context);
			// === !Environment light ===

			// === Directional light ===
			const SEntity& directionalLightEntity = AddEntity("Directional Light");
			if (!directionalLightEntity.IsValid())
				return;

			// NW: Add transform to directional light so it can filter environmental lights based on distance
			AddComponent<STransformComponent>(directionalLightEntity);
			AddComponentEditorContext(directionalLightEntity, &STransformComponentEditorContext::Context);

			SDirectionalLightComponent& directionalLight = *AddComponent<SDirectionalLightComponent>(directionalLightEntity);
			AddComponentEditorContext(directionalLightEntity, &SDirectionalLightComponentEditorContext::Context);
			directionalLight.Direction = { 1.0f, 1.0f, -1.0f, 0.0f };
			directionalLight.Color = { 212.0f / 255.0f, 175.0f / 255.0f, 55.0f / 255.0f, 0.25f };
			directionalLight.ShadowmapView.ShadowmapViewportIndex = 0;
			directionalLight.ShadowmapView.ShadowProjectionMatrix = SMatrix::OrthographicLH(directionalLight.ShadowViewSize.X, directionalLight.ShadowViewSize.Y, directionalLight.ShadowNearAndFarPlane.X, directionalLight.ShadowNearAndFarPlane.Y);

			SVolumetricLightComponent& volumetricLight = *AddComponent<SVolumetricLightComponent>(directionalLightEntity);
			AddComponentEditorContext(directionalLightEntity, &SVolumetricLightComponentEditorContext::Context);
			volumetricLight.IsActive = false;
			// === !Directional light ===

			// === Spotlight ===
			const SEntity& spotlight = AddEntity("Spot Light");
			if (!spotlight.IsValid())
				return;

			STransform& spotlightTransform = (*AddComponent<STransformComponent>(spotlight)).Transform;
			AddComponentEditorContext(spotlight, &STransformComponentEditorContext::Context);
			SMatrix spotlightMatrix = spotlightTransform.GetMatrix();
			spotlightMatrix.SetTranslation({ 0.0f, 0.0f, 0.0f });
			spotlightTransform.SetMatrix(spotlightMatrix);

			SSpotLightComponent& spotlightComp = *AddComponent<SSpotLightComponent>(spotlight);
			AddComponentEditorContext(spotlight, &SSpotLightComponentEditorContext::Context);
			spotlightComp.Direction = SVector4::Forward;
			spotlightComp.DirectionNormal1 = SVector4::Right;
			spotlightComp.DirectionNormal2 = SVector4::Up;
			spotlightComp.ColorAndIntensity = { 0.0f, 1.0f, 0.0f, 5.0f };
			spotlightComp.OuterAngle = 40.0f;
			spotlightComp.InnerAngle = 30.0f;
			spotlightComp.Range = 3.0f;

			SVolumetricLightComponent& volumetricSpotLight = *AddComponent<SVolumetricLightComponent>(spotlight);
			AddComponentEditorContext(spotlight, &SVolumetricLightComponentEditorContext::Context);
			volumetricSpotLight.IsActive = false;

			const SMatrix spotlightProjection = SMatrix::PerspectiveFovLH(UMath::DegToRad(90.0f), 1.0f, 0.001f, spotlightComp.Range);
			const SVector4 spotlightPosition = spotlightTransform.GetMatrix().GetTranslation4();

			spotlightComp.ShadowmapView.ShadowPosition = spotlightPosition;
			spotlightComp.ShadowmapView.ShadowmapViewportIndex = 7;
			spotlightComp.ShadowmapView.ShadowViewMatrix = SMatrix::LookAtLH(spotlightPosition.ToVector3(), (spotlightPosition + spotlightComp.Direction).ToVector3(), spotlightComp.DirectionNormal2.ToVector3());
			spotlightComp.ShadowmapView.ShadowProjectionMatrix = spotlightProjection;
			// === !Spotlight ===

			const SEntity& ghosty = AddEntity("Ghosty");
			if (!ghosty.IsValid())
				return;

			STransformComponent& ghostyTransform = *AddComponent<STransformComponent>(ghosty);
			AddComponentEditorContext(ghosty, &STransformComponentEditorContext::Context);
			AddComponent<SSpriteComponent>(ghosty, "Assets/Textures/Circle_c.hva");
			AddComponentEditorContext(ghosty, &SSpriteComponentEditorContext::Context);
			AddComponent<SGhostyComponent>(ghosty);
			AddComponentEditorContext(ghosty, &SGhostyComponentEditorContext::Context);

			ghostyTransform.Transform.Move({ 0.0f, 2.0f, 0.0f });

			//spriteWSComp.UVRect = { 0.0f, 0.0f, 0.125f, 0.125f };

			//Define UVRects for Animation Frames on row 0, 1, 2
			//F32 width = 1152.0f;
			//F32 height = 384.0f;
			//F32 frameSize = 96.0f;
			F32 width = 128.0f;
			F32 height = 128.0f;
			F32 frameSize = 128.0f;
			std::vector<SVector4> uvRectsIdle = CreateAnimationClip(width, height, frameSize, 3, 6, 6);
			std::vector<SVector4> uvRectsMoveLeft = CreateAnimationClip(width, height, frameSize, 0, 0, 6);
			std::vector<SVector4> uvRectsMoveRight = CreateAnimationClip(width, height, frameSize, 1, 0, 6);

			SSpriteAnimationClip idle;
			idle.UVRects = uvRectsIdle;
			idle.Durations.push_back(0.15f);
			idle.Durations.push_back(0.15f);

			SSpriteAnimationClip moveLeft;
			moveLeft.UVRects = uvRectsMoveLeft;
			moveLeft.Durations.push_back(0.15f);
			moveLeft.Durations.push_back(0.15f);
			moveLeft.Durations.push_back(0.15f);

			SSpriteAnimationClip moveRight
			{
				uvRectsMoveRight, //UVRects
				{ 0.15f, 0.15f, 0.15f }, //Duration per Frame
				true	//IsLooping
			};

			SSpriteAnimatorGraphComponent& spriteAnimatorGraphComponent = *AddComponent<SSpriteAnimatorGraphComponent>(ghosty);
			AddComponentEditorContext(ghosty, &SSpriteAnimatorGraphComponentEditorContext::Context);

			SSpriteAnimatorGraphNode& rootNode = spriteAnimatorGraphComponent.SetRoot(std::string("Idle | Locomotion"), "CGhostySystem::EvaluateIdle");
			rootNode.AddClipNode(&spriteAnimatorGraphComponent, std::string("Idle"), idle);

			SSpriteAnimatorGraphNode& locomotionNode = rootNode.AddSwitchNode(std::string("Locomotion: Left | Right"), "CGhostySystem::EvaluateLocomotion");
			locomotionNode.AddClipNode(&spriteAnimatorGraphComponent, std::string("Move Left"), moveLeft);
			locomotionNode.AddClipNode(&spriteAnimatorGraphComponent, std::string("Move Right"), moveRight);

			SPhysics2DComponent& phys2DComponent = *AddComponent<SPhysics2DComponent>(ghosty);
			AddComponentEditorContext(ghosty, &SPhysics2DComponentEditorContext::Context);
			phys2DComponent.BodyType = EPhysics2DBodyType::Dynamic;
			phys2DComponent.ShapeType = EPhysics2DShapeType::Circle;
			phys2DComponent.ShapeLocalExtents = { ghostyTransform.Transform.GetMatrix().GetScale().X, ghostyTransform.Transform.GetMatrix().GetScale().Y };
			//phys2DComponent.ConstrainRotation = true;
			GEngine::GetWorld()->Initialize2DPhysicsData(ghosty);

			const SEntity& floor = AddEntity("Floor");
			STransformComponent& floorTransform = *AddComponent<STransformComponent>(floor);
			AddComponentEditorContext(floor, &STransformComponentEditorContext::Context);
			floorTransform.Transform.Move({ 0.f, -2.f, 0.f });
			floorTransform.Transform.Scale({ 0.4f, 0.5f, 1.f });
			SSpriteComponent& floorSprite = *AddComponent<SSpriteComponent>(floor, "Assets/Textures/T_Checkboard_128x128_c.hva");
			AddComponentEditorContext(floor, &SSpriteComponentEditorContext::Context);

			floorSprite.UVRect = { 0.0f, 0.0f, 1.f, 1.f };

			SPhysics2DComponent& floorPhys2DComponent = *AddComponent<SPhysics2DComponent>(floor);
			AddComponentEditorContext(floor, &SPhysics2DComponentEditorContext::Context);
			floorPhys2DComponent.BodyType = EPhysics2DBodyType::Static;
			floorPhys2DComponent.ShapeType = EPhysics2DShapeType::Capsule;
			floorPhys2DComponent.ShapeLocalExtents = { floorTransform.Transform.GetMatrix().GetScale().X, floorTransform.Transform.GetMatrix().GetScale().Y };
			GEngine::GetWorld()->Initialize2DPhysicsData(floor);
		}
    }
}

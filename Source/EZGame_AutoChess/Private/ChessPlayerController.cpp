// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayerController.h"
#include "EngineUtils.h"
#include "GridMap.h"
#include "ChessMovementComponent.h"
#include "AIController.h"
#include "AITask_MoveToNode.h"
#include "AITask_TurnTo.h"

//���캯��
AChessPlayerController::AChessPlayerController()
{
	this->bShowMouseCursor = true;
}

//�󶨰���
void AChessPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AChessPlayerController::HandleLeftClick);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AChessPlayerController::HandleRightClick);
}

//��ȡ����
AGridMap* AChessPlayerController::GetBattleMap()
{
	if (BattleMap)
		return BattleMap;
	//ʹ�õ���������
	for (TActorIterator<AGridMap> It(GetWorld()); It; ++It)
	{
		if (!*It)
			continue;
		BattleMap = *It;
		break;
	}
	return BattleMap;
}

//ˢ���Ե�����
void AChessPlayerController::FlushCheckNodes()
{
	if (!GetBattleMap())
		return;
	switch (DebugMode)
	{
	case EChessDebugMode::DebugPath:
		//����2��ѡ���������ò���
		if (SelectedNodes.Num() >= 2)
		{
			SelectedNodes.Reset();
			BattleMap->ResetNodeMaterialAll();
		}
		break;
	case EChessDebugMode::DebugPawn:
	case EChessDebugMode::DebugTaskMove:
	case EChessDebugMode::DebugTaskRotate:
		//����1��ѡ���������ò���
		if (SelectedNodes.Num() >= 1)
		{
			SelectedNodes.Reset();
			BattleMap->ResetNodeMaterialAll();
		}
		break;
	default:
		BattleMap->ResetNodeMaterialAll();
		break;
	}
}

//��ȡ����Ӧ���̵�λ��
FVector AChessPlayerController::GetMouseCursorPosition() const
{
	if (!BattleMap)
		return FVector::ZeroVector;
	FVector WorldLocation, WorldDirection;
	//��Ļ����ת�������꣬�ش�����ͷ���
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		//��ȡZ���ֵ
		float tScale = BattleMap->GetActorLocation().Z - WorldLocation.Z;
		tScale /= WorldDirection.Z;
		WorldLocation += tScale * WorldDirection;
		return WorldLocation;
	}
	return FVector::ZeroVector;
}

//����¼�
void AChessPlayerController::HandleLeftClick()
{
	switch (DebugMode)
	{
	case EChessDebugMode::None:
		break;
	case EChessDebugMode::DebugPath:
		HandleDebugPath();
		break;
	case EChessDebugMode::DebugPawn:
		HandleDebugPawn();
	case EChessDebugMode::DebugTaskMove:
		HandleDebugTaskMove();
	case EChessDebugMode::DebugTaskRotate:
		HandleDebugTaskRotate();
	default:
		break;
	}
}

//�Ҽ��¼�
void AChessPlayerController::HandleRightClick()
{
	if (!GetBattleMap())
		return;
	FlushCheckNodes();
	//��ȡѡ�����
	UGridNode* tHitNode = BattleMap->CheckHitNode(GetMouseCursorPosition());
	if (tHitNode)
	{
		//ͨ��״̬�л�
		tHitNode->PassFlag = tHitNode->PassFlag == ENodePassFlag::Pass ? ENodePassFlag::Block : ENodePassFlag::Pass;
		BattleMap->ResetNodeMaterial(tHitNode);
	}
}

//DebugѰ·
void AChessPlayerController::HandleDebugPath()
{
	if (!GetBattleMap())
		return;
	FlushCheckNodes();
	//��ȡѡ�����
	UGridNode* tHitNode = BattleMap->CheckHitNode(GetMouseCursorPosition());
	//�ж����ͨ��״̬
	if (tHitNode && tHitNode->PassFlag != ENodePassFlag::Block)
	{
		SelectedNodes.AddUnique(tHitNode);
		BattleMap->SetNodeMaterial(tHitNode, BattleMap->DebugMaterial1);
	}
	//���ѡ����񳬹�2���������Ѱ·
	if (SelectedNodes.Num() >= 2)
	{
		TArray<UGridNode*> tPath;
		bool bResult = BattleMap->FindPath(tPath, nullptr, SelectedNodes[0], SelectedNodes[1]);
		if (!bResult)
			return;
		for (auto a : tPath)
		{
			//�����յ��������
			if (a == SelectedNodes[1])
				continue;
			//����·������
			BattleMap->SetNodeMaterial(a, BattleMap->DebugMaterial2);
		}
	}
}

//Debug��ɫ
void AChessPlayerController::HandleDebugPawn()
{
	FVector WorldLocation, WorldDirection;
	//��ȡ������Ľ�ɫ
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		//���߼��
		FVector Start, End;
		Start = WorldLocation;
		End = Start + WorldDirection * 2000;
		FCollisionQueryParams Params;
		FCollisionObjectQueryParams ObjectParams;
		//��ײ����ѡ��pawn
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		FHitResult OutHit;
		UWorld* World = GetWorld();
		if (World)
		{
			if (World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParams, Params))
			{
				//�趨��ǰѡ�н�ɫ
				SelectedPawn = Cast<APawn>(OutHit.GetActor());
				return;
			}
		}
	}

	if (SelectedPawn && GetBattleMap())
	{
		FlushCheckNodes();
		//��ȡ�ƶ����
		UChessMovementComponent* ChessMovementComponent = SelectedPawn->FindComponentByClass<UChessMovementComponent>();
		if (!ChessMovementComponent || !ChessMovementComponent->NowNode)
			return;
		//��ȡ��㣬�յ�
		UGridNode* FromNode = ChessMovementComponent->NowNode;
		UGridNode* ToNode = BattleMap->CheckHitNode(GetMouseCursorPosition());
		if (!FromNode || !ToNode)
			return;
		if (ToNode->PassFlag == ENodePassFlag::Block)
			return;
		SelectedNodes.AddUnique(ToNode);
		//���ò���
		BattleMap->SetNodeMaterial(ToNode, BattleMap->DebugMaterial1);
		TArray<UGridNode*> tPath;
		//����Ѱ·
		bool bResult = BattleMap->FindPath(tPath, SelectedPawn, FromNode, ToNode);
		if (!bResult)
			return;
		//����
		ChessMovementComponent->SetMovePath(tPath);

	}
}

//DebugMoveToTask
void AChessPlayerController::HandleDebugTaskMove()
{
	FVector WorldLocation, WorldDirection;
	//��ȡ������Ľ�ɫ
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		//���߼��
		FVector Start, End;
		Start = WorldLocation;
		End = Start + WorldDirection * 2000;
		FCollisionQueryParams Params;
		FCollisionObjectQueryParams ObjectParams;
		//��ײ����ѡ��pawn
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		FHitResult OutHit;
		UWorld* World = GetWorld();
		if (World)
		{
			if (World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParams, Params))
			{
				SelectedPawn = Cast<APawn>(OutHit.GetActor());
				return;
			}
		}
	}

	if (SelectedPawn && GetBattleMap())
	{
		FlushCheckNodes();
		//��ȡ�յ�
		UGridNode* ToNode = BattleMap->CheckHitNode(GetMouseCursorPosition());
		//��ȡAIController
		AAIController* AIController = Cast<AAIController>(SelectedPawn->GetController());
		if (!ToNode)
			return;
		if (!AIController)
			return;
		if (ToNode->PassFlag == ENodePassFlag::Block)
			return;
		SelectedNodes.AddUnique(ToNode);
		BattleMap->SetNodeMaterial(ToNode, BattleMap->DebugMaterial1);
		if (NowMoveTask)
		{
			//ǿ��ȡ����һ��Task
			NowMoveTask->ForceEndTask();
		}
		//����Task
		NowMoveTask = UAITask_MoveToNode::AIMoveTo(AIController, ToNode, nullptr, 0);
		//ִ��Task
		NowMoveTask->ForceActiveTask();
	}
}

void AChessPlayerController::HandleDebugTaskRotate()
{
	//��ȡ������Ľ�ɫ
	FVector WorldLocation, WorldDirection;
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		//���߼��
		FVector Start, End;
		Start = WorldLocation;
		End = Start + WorldDirection * 2000;
		FCollisionQueryParams Params;
		FCollisionObjectQueryParams ObjectParams;
		//��ײ����ѡ��pawn
		ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		FHitResult OutHit;
		UWorld* World = GetWorld();
		if (World)
		{
			if (World->LineTraceSingleByObjectType(OutHit, Start, End, ObjectParams, Params))
			{
				SelectedPawn = Cast<APawn>(OutHit.GetActor());
				return;
			}
		}
	}

	if (SelectedPawn && GetBattleMap())
	{
		FlushCheckNodes();
		//��ȡĿ���
		UGridNode* ToNode = BattleMap->CheckHitNode(GetMouseCursorPosition());
		AAIController* AIController = Cast<AAIController>(SelectedPawn->GetController());
		//��ȫ�ж�
		if (!ToNode)
			return;
		if (!AIController)
			return;
		if (ToNode->PassFlag == ENodePassFlag::Block)
			return;
		SelectedNodes.AddUnique(ToNode);
		BattleMap->SetNodeMaterial(ToNode, BattleMap->DebugMaterial1);
		if (NowRotateTask)
		{
			//ǿ��ȡ����һ��Task
			NowRotateTask->ForceEndTask();
		}
		//����Task
		NowRotateTask = UAITask_TurnTo::AITurnTo(AIController, nullptr, ToNode, FRotator::ZeroRotator, 0);
		//ִ��Task
		NowRotateTask->ForceActiveTask();
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "GridMap.h"
#include "HexNode.h"
#include "ProceduralMeshComponent.h"

//���캯��
AGridMap::AGridMap()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UProceduralMeshComponent>("MapMesh");
	this->SetRootComponent(Mesh);
}

//��Ϸ��һ֡����
void AGridMap::BeginPlay()
{
	Super::BeginPlay();
	//������������
	GenerateGridMap();
}

//��ʼ������ʱ����
void AGridMap::PostInitProperties()
{
	Super::PostInitProperties();
	GenerateGridMap();
}

#if WITH_EDITOR
//�༭���иı�����ʱ����
void AGridMap::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	//���С��С���С�ı�ʱ������������
	if (PropertyChangedEvent.Property->GetName() == "Roll"
		|| PropertyChangedEvent.Property->GetName() == "Colume"
		|| PropertyChangedEvent.Property->GetName() == "Size")
	{
		//������������
		GenerateGridMap();
	}

}
#endif

//�����������
void AGridMap::GenerateGridMap()
{
	for (auto a : NodeMap)
	{
		//�����������
		if (a.Value)
			a.Value->MarkPendingKill();
	}
	//����Map
	NodeMap.Reset();
	//�����������
	GenerateNodes(Size, Roll, Colume);
	//��������ģ��
	GenerateMapMesh();
}

//����������
void AGridMap::GenerateNodes(float InSize, int InRoll, int InColume)
{
	switch (MapType)
	{
	case EGridType::None:
		break;
	//���������
	case EGridType::Hex:
		GenerateHexNodes(InSize, InRoll, InColume);
		break;
	default:
		break;
	}
	//��ʼ�����
	InitNodes();
}

//�������������
void AGridMap::GenerateHexNodes(float InHexSize, int InRoll, int InColume)
{
	FVector tHexLocation;
	for (int i = 0; i < InRoll; ++i)
	{
		for (int j = 0; j < InColume; ++j)
		{
			//UE�к���������ΪY������������ΪX����Ҫ�����ڶ�ά����ϵXY��ֵ
			FGridVector tHexVector = FGridVector(j, i);
			tHexLocation.X = 1.5 * InHexSize * i;
			tHexLocation.Y = i % 2 == 0 ? (FMath::Sqrt(3) * InHexSize * j) : (FMath::Sqrt(3) * InHexSize * j + FMath::Sqrt(3) * 0.5 * InHexSize);
			tHexLocation.Z = 0;
			tHexLocation += GetActorLocation();
			UHexNode* tNode = NewObject<UHexNode>(this);
			//����ʼ��
			tNode->InitNode(this, tHexLocation, tHexVector, InHexSize);
			NodeMap.Add(tHexVector, tNode);
		}
	}
}

//��ʼ��������
void AGridMap::InitNodes()
{
	switch (MapType)
	{
	case EGridType::None:
		break;
	//���������
	case EGridType::Hex:
		InitHexNodes();
		break;
	default:
		break;
	}
}

//��ʼ�����������
void AGridMap::InitHexNodes()
{
	//���������������������ж϶�Ӧ��������Ƿ���ڣ�����ֵ
	for (auto a : NodeMap)
	{
		UHexNode* tHexNode = Cast<UHexNode>(a.Value);
		if (!tHexNode)
			continue;
		FGridVector tRightUp = a.Key.Y % 2 == 0 ? a.Key + FGridVector(0, 1) : a.Key + FGridVector(1, 1);
		if (NodeMap.Contains(tRightUp) && NodeMap[tRightUp]->IsA(UHexNode::StaticClass()))
			tHexNode->RightUpNode = Cast<UHexNode>(NodeMap[tRightUp]);

		FGridVector tRight = a.Key + FGridVector(1, 0);
		if (NodeMap.Contains(tRight) && NodeMap[tRight]->IsA(UHexNode::StaticClass()))
			tHexNode->RightNode = Cast<UHexNode>(NodeMap[tRight]);

		FGridVector tRightDown = a.Key.Y % 2 == 0 ? a.Key + FGridVector(0, -1) : a.Key + FGridVector(1, -1);
		if (NodeMap.Contains(tRightDown) && NodeMap[tRightDown]->IsA(UHexNode::StaticClass()))
			tHexNode->RightDownNode = Cast<UHexNode>(NodeMap[tRightDown]);

		FGridVector tLeftDown = a.Key.Y % 2 == 0 ? a.Key + FGridVector(-1, -1) : a.Key + FGridVector(0, -1);
		if (NodeMap.Contains(tLeftDown) && NodeMap[tLeftDown]->IsA(UHexNode::StaticClass()))
			tHexNode->LeftDownNode = Cast<UHexNode>(NodeMap[tLeftDown]);

		FGridVector tLeft = a.Key + FGridVector(-1, 0);
		if (NodeMap.Contains(tLeft) && NodeMap[tLeft]->IsA(UHexNode::StaticClass()))
			tHexNode->LeftNode = Cast<UHexNode>(NodeMap[tLeft]);

		FGridVector tLeftUp = a.Key.Y % 2 == 0 ? a.Key + FGridVector(-1, 1) : a.Key + FGridVector(0, 1);
		if (NodeMap.Contains(tLeftUp) && NodeMap[tLeftUp]->IsA(UHexNode::StaticClass()))
			tHexNode->LeftUpNode = Cast<UHexNode>(NodeMap[tLeftUp]);

	}
}

//�������������ȡ���
UGridNode* AGridMap::GetNode(FGridVector InCoord) const
{
	if (NodeMap.Contains(InCoord))
		return NodeMap[InCoord];
	else
		return nullptr;
}

//https://zhuanlan.zhihu.com/p/34352477 A*
//https://zhuanlan.zhihu.com/p/54510444 A*++
bool AGridMap::FindPath(TArray<UGridNode*>& Path, AActor* InActor, UGridNode* FromNode, UGridNode* ToNode, int StopSteps)
{
	Path.Empty();
	//��ȫ�ж�
	if (!FromNode || !ToNode)
		return false;
	if (!NodeMap.FindKey(FromNode) || !NodeMap.FindKey(ToNode))
		return false;
	//��ȡʵ�������յ�
	TArray<UGridNode*> ToNodes = GetNodeNeighbors(ToNode, StopSteps);
	for (int i = ToNodes.Num() - 1; i >= 0; i--)
	{
		if (!ToNodes[i]->CanPass(InActor))
			ToNodes.RemoveAt(i);
	}
	//�ж�����յ��Ƿ����
	if (!FromNode->CanPass(InActor))
		return false;
	if (ToNodes.Num() == 0)
		return false;
	//�ж��Ƿ��Ѿ������յ�
	if (ToNodes.Contains(FromNode))
		return true;
	
	//��Ҫ������·��
	TArray<UGridNode*> openList;
	//�Ѿ���ɱ�����·��
	TArray<UGridNode*> closeList;
	//��ǰ����·��
	UGridNode* nowNode;
	nowNode = FromNode;
	openList.Add(nowNode);
	bool bFinded = false;
	//A*Ѱ·
	while (!bFinded)
	{
		//�Ƴ�openList������closeList
		openList.Remove(nowNode);
		closeList.Add(nowNode);
		//��ȡ����·��
		TArray<UGridNode*> neighbors = nowNode->GetNeighbors();
		for (auto neighbor : neighbors)
		{
			if (!neighbor)
				continue;
			//�ж�����·���Ƿ�Ϊ�յ�
			if (ToNodes.Contains(neighbor))
			{
				bFinded = true;
				ToNode = neighbor;
				neighbor->PreNode = nowNode;
			}
			//�����closeList����ͨ��������
			if (closeList.Contains(neighbor) || !neighbor->CanPass(InActor))
				continue;
			//�������openlist�������openlist�Ķ�β���Ա�����
			if (!openList.Contains(neighbor))
			{
				openList.Add(neighbor);
				neighbor->PreNode = nowNode;
			}
		}
		//ȡ�����׵�·�㣬����Ϊ�´�ѭ��������·��
		if (openList.Num() <= 0)
			break;
		else
			nowNode = openList[0];
	}
	openList.Empty();
	closeList.Empty();
	//�ҵ���·��
	if (bFinded)
	{
		UGridNode* tNode = ToNode;
		while (tNode != FromNode)
		{
			Path.Add(tNode);
			tNode = Cast<UGridNode>(tNode->PreNode);
		}
		//��ȡ��·��ʱ���յ�->����·������Ҫ��ת�����->�յ��·��
		Algo::Reverse(Path);

		return true;
	}

	return false;

}

//�ж�·���Ƿ����
bool AGridMap::IsPathExist(AActor* InActor, UGridNode* FromNode, UGridNode* ToNode, int StopSteps)
{
	//�ش�FindPath��ֵ
	TArray<UGridNode*> Path;
	return FindPath(Path, InActor, FromNode, ToNode, StopSteps);
}

//��ȡ�������
TArray<UGridNode*> AGridMap::GetNodeNeighbors(UGridNode* InNode, int InStep) const
{
	int neighborSteps = InStep;
	TArray<UGridNode*> nowCheckList;
	TArray<UGridNode*> nextCheckList;
	TArray<UGridNode*> findList;
	nextCheckList.AddUnique(InNode);
	findList.AddUnique(InNode);

	//ʹ��While���������ж�
	while (neighborSteps > 0)
	{
		nowCheckList = nextCheckList;
		nextCheckList.Empty();
		for (UGridNode* a : nowCheckList)
		{
			if (!a)
				continue;
			TArray<UGridNode*> neighbors = a->GetNeighbors();
			for (UGridNode* b : neighbors)
			{
				if (findList.Contains(b))
					continue;
				findList.AddUnique(b);
				nextCheckList.AddUnique(b);
			}
		}
		neighborSteps -= 1;
	}

	return findList;
}

//��������ģ��
void AGridMap::GenerateMapMesh()
{
	//ģ����������ʾ�����Mesh�ϵڼ�������
	int Index = 0;
	//����
	Mesh->ClearAllMeshSections();
	for (auto a : NodeMap)
	{
		if (!a.Value)
			continue;
		TArray<FVector> Vertices;
		TArray<int32> Indecies;
		TArray<FVector> Normals;
		TArray<FVector2D> UV1, EmptyArray;
		TArray<FColor> VertexColors;
		TArray<FVector> Tangents;
		TArray<FProcMeshTangent> MeshTangents;
		//������ֵ
		a.Value->MeshIndex = Index;
		//����DrawNode�������÷�ʽ�ش�������Ϣ
		a.Value->DrawNode(Vertices, Indecies, Normals, UV1, VertexColors, Tangents, GetActorLocation());
		//�����������������߽ṹ��
		for (FVector& b : Tangents)
			MeshTangents.Add(FProcMeshTangent(b, false));
		//������������ģ��
		Mesh->CreateMeshSection(Index, Vertices, Indecies, Normals, UV1, EmptyArray, EmptyArray, EmptyArray, VertexColors, MeshTangents, false);
		//���ò���
		ResetNodeMaterial(a.Value);
		Index++;
	}

}

//����ģ�Ͳ���
void AGridMap::SetNodeMaterial(UGridNode* InNode, UMaterialInterface* InMaterial)
{
	if (!InNode || !InMaterial)
		return;
	Mesh->SetMaterial(InNode->MeshIndex, InMaterial);
}

//����ģ�Ͳ���
void AGridMap::ResetNodeMaterial(UGridNode* InNode)
{
	if (!InNode)
		return;

	if (InNode->NodeActors.Num() > 0 && DebugMaterial1)
	{
		SetNodeMaterial(InNode, DebugMaterial1);
		return;
	}
	//����ͨ��״̬���ò���
	if (InNode->PassFlag == ENodePassFlag::Pass && PassMaterial)
		SetNodeMaterial(InNode, PassMaterial);
	else if (InNode->PassFlag == ENodePassFlag::Block && BlockMaterial)
		SetNodeMaterial(InNode, BlockMaterial);
	
}

//��������ģ�Ͳ���
void AGridMap::ResetNodeMaterialAll()
{
	for (auto a : NodeMap)
	{
		if (!a.Value)
			continue;
		ResetNodeMaterial(a.Value);
	}
}

//�ж��Ƿ��������
UGridNode* AGridMap::CheckHitNode(FVector InPosition)
{
	UGridNode* tHitNode = nullptr;
	//�ж�Z�ᣬ������ֱ�ӷ���
	if (FMath::Abs(InPosition.Z - GetActorLocation().Z) > 1)
		return tHitNode;
	switch (MapType)
	{
	case EGridType::None:
		break;
	case EGridType::Hex:
		tHitNode = CheckHitHexNode(InPosition);
		break;
	default:
		break;
	}
	return tHitNode;
}

//�ж��Ƿ�����������
//https://blog.csdn.net/iteye_10018/article/details/82541460
UGridNode* AGridMap::CheckHitHexNode(FVector InPosition)
{
	UGridNode* tHitNode = nullptr;
	for (auto h : NodeMap)
	{
		if (!h.Value)
			continue;
		float x = FMath::Abs(InPosition.X - h.Value->Location.X);
		float y = FMath::Abs(InPosition.Y - h.Value->Location.Y);
		float a = h.Value->Size;
		if (x > a || y > FMath::Sqrt(3) * 0.5 * a)
			continue;
		if (a - x > y / FMath::Sqrt(3))
		{
			tHitNode = h.Value;
			break;
		}
	}
	return tHitNode;
}

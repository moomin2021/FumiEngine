#include "NavMesh.h"

#include "Collision.h"
#include "PipelineManager.h"
#include "NavMeshPath.h"

#include <list>

void NavMesh::Initialize(const std::string& fileName)
{
	// モデル生成
	model_ = std::make_unique<Model>(fileName);

	// オブジェクト3D生成
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetPosition({ 0.0f, 0.25f, 0.0f });
	object_->SetColor({ 1.0f, 1.0f, 1.0f, 0.3f });

	// リンクライン
	linkLines_ = std::make_unique<Line3D>();
	linkLines_->Initialize(200);
	linkLines_->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });

	// セルを作成
	CreateCell();

	// セルをリンク
	LinkCell();
}

void NavMesh::MatUpdate()
{
	object_->MatUpdate();
	linkLines_->MatUpdate();
}

void NavMesh::Draw()
{
	object_->Draw();

	PipelineManager::PreDraw("Line3D", D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	if (isLinkLineDraw_)
	{
		linkLines_->Draw();
	}
	PipelineManager::PreDraw("Object3D");
}

bool NavMesh::RouteSearch(const Vector3& startVec, const Vector3& goalVec, std::vector<Vector3>& outputRoute)
{
	// スタートとゴールのセルIDを求める
	int32_t startID = CheckRay2Cell(Ray(startVec, { 0.0f, -1.0f, 0.0f }));
	int32_t goalID = CheckRay2Cell(Ray(goalVec, { 0.0f, -1.0f, 0.0f }));

	if (startID == -1 || goalID == -1) return false;

	// 1. 現在の参照ノード
	// 2. オープンリスト、クローズリスト
	std::shared_ptr<NavNode> current = nullptr;
	std::list<std::shared_ptr<NavNode>> open, close;
	std::shared_ptr<NavNode> startNode = std::make_shared<NavNode>(goalID);
	//startNode->cCost = 0.0f;
	//startNode->hCost = 0.0f;

	// ノードを格納
	open.emplace_back(startNode);

	// 過剰なループ防止
	int32_t loopCount = 0;
	const int32_t loopLimit = 1000000;

	// openリストの中身がある間はループ
	while (open.size() || loopCount < loopLimit)
	{
		loopCount++;

		// スコアが最も低いノードを取り出す

		// 1. 最もスコアの低いノードのポインタ
		// 2. 最も低いスコアの保存する変数
		// 3. 削除するインデックス
		std::shared_ptr<NavNode> minScoreNode = nullptr;
		float minScore = FLT_MAX;
		uint32_t deleteIndex = 0;
		uint32_t count = 0;

		for (auto& it : open)
		{
			// スコアが今までで最も低かったら保存する
			if (it->GetTotalCost() < minScore)
			{
				// 1. 今までで最も低いノードとして保存
				// 2. 今までで最も低いスコアとして保存
				current = it;
				minScore = it->GetTotalCost();
				deleteIndex = count;
			}

			count++;
		}
		open.erase(std::next(open.begin(), deleteIndex));

		// 取り出したノードがスタート地点と同じセルだったらパス検索官僚
		if (current->cellID == startID) break;

		// closeに保存
		close.emplace_back(current);

		// currentCellの隣接セルを取得
		NavCell* currentCell = GetNavCell(current->cellID);
		for (int32_t neighborIndex : currentCell->GetLinkID())
		{
			// neighborIndexが無効な値 または closeリストに含まれている場合は以降の処理を実行しない
			std::shared_ptr<NavNode> neighborNode = std::make_shared<NavNode>(neighborIndex);
			bool containsInCloseList = false;
			for (auto& it : close)
			{
				if (it->cellID == neighborNode->cellID)
				{
					containsInCloseList = true;
					break;
				}
			}

			if (neighborIndex == ID_NONE || containsInCloseList) continue;

			NavCell* neighborCell = GetNavCell(neighborNode->cellID);
			float totalCost = current->cCost + Vector3(currentCell->GetCenter() - neighborCell->GetCenter()).length();
			bool containsInOpenList = false;

			for (auto& it : open)
			{
				if (it->cellID == neighborNode->cellID)
				{
					containsInOpenList = true;
					break;
				}
			}

			// OpenリストにNeighborNodeがあるかチェック
			if (containsInOpenList)
			{
				// 「スタートからNeighborまでの推定コスト」がこれまでに記録した「スタートからNeighborまでの最小コスト」よりも小さい
				if (totalCost < neighborNode->cCost)
				{
					neighborNode->parent = current;
					neighborNode->cCost = totalCost;
				}
			}

			// 未だOpenリストにない
			else
			{
				// 必要な情報を記録してOpenリストへ格納
				neighborNode->parent = current;
				neighborNode->cCost = totalCost;
				neighborNode->hCost = Vector3(neighborCell->GetCenter() - neighborCell->GetCenter()).length();
				open.emplace_back(neighborNode);
			}
		}
	}

	// 過剰にループするようであればパス検索は失敗
	if (loopCount >= loopLimit) return false;

	// パスが見つかった
	std::unique_ptr<NavMeshPath> navMeshPath = std::make_unique<NavMeshPath>();

	navMeshPath->AddWayPoint(startVec);
	while (current->parent != nullptr)
	{
		NavCell* currentCell = GetNavCell(current->cellID);
		NavCell* parentCell = GetNavCell(current->parent->cellID);
		navMeshPath->AddWayPoint(currentCell, parentCell);
		current = current->parent;
	}
	navMeshPath->AddWayPoint(goalVec);

	std::vector<Vector3> result = navMeshPath->GetStraightPath(0.3f);
	
	//std::vector<NavMeshPath::NavWayPoint> navWayPoints = navMeshPath->GetWayPoints();
	//std::vector<Vector3> result = {};

	//for (auto& it : navWayPoints)
	//{
	//	result.emplace_back(it.location);
	//}

	outputRoute = result;

	return true;
}

int32_t NavMesh::CheckRay2Cell(const Ray& ray)
{
	for (auto& it : cells_)
	{
		if (Collision::CheckRay2Triangle(ray, it->GetTriangle()))
		{
			return it->GetCellID();
		}
	}

	return ID_NONE;
}

void NavMesh::CreateCell()
{
	// モデルからメッシュを読み込む
	Mesh mesh = model_->GetMesh()[0];

	// 頂点データ
	std::vector<Mesh::Vertex> vertices = mesh.GetVertex();

	for (uint32_t i = 0; i < vertices.size() / 3; i++)
	{
		cells_.emplace_back(std::make_unique<NavCell>(
			i, vertices[i * 3].pos, vertices[i * 3 + 1].pos, vertices[i * 3 + 2].pos));
		cellsCenter_.emplace_back(cells_.back()->GetCenter());
	}
}

void NavMesh::LinkCell()
{
	Vector3 addVec = { 0.0f, 0.5f, 0.0f };

	// セル同士を総当たりしてリンク処理をする
	for (auto& itA : cells_)
	{
		for (auto& itB : cells_)
		{
			// 同じセル場合は飛ばす
			if (itA == itB) continue;

			// リンク済みの場合は飛ばす
			if (itA->AlreadyLinkCell(itB->GetCellID())) continue;

			// 既にリンクしている数が3つだったら飛ばす
			if (itA->LinkCount() == 3) continue;

			CellSide side;

			// itAセルは辺AB側にリンクしているセルがない かつ
			// itBセルは辺AB側でitAセルとのリンクが可能である
			if (itA->GetLinkID(CellSide::SIDE_AB) == ID_NONE &&
				itB->CheckSharedEdge(itA->GetEdge(CellSide::SIDE_AB), side))
			{
				itA->SetLinkID(CellSide::SIDE_AB, itB->GetCellID());
				itB->SetLinkID(side, itA->GetCellID());
				linkLines_->AddPoint(itA->GetCenter() + addVec, itB->GetCenter() + addVec);
			}

			// itAセルは辺BC側にリンクしているセルがない かつ
			// itBセルは辺BC側でitAセルとのリンクが可能である
			else if (itA->GetLinkID(CellSide::SIDE_BC) == ID_NONE &&
				itB->CheckSharedEdge(itA->GetEdge(CellSide::SIDE_BC), side))
			{
				itA->SetLinkID(CellSide::SIDE_BC, itB->GetCellID());
				itB->SetLinkID(side, itA->GetCellID());
				linkLines_->AddPoint(itA->GetCenter() + addVec, itB->GetCenter() + addVec);
			}

			// itAセルは辺CA側にリンクしているセルがない かつ
			// itBセルは辺CA側でitAセルとのリンクが可能である
			else if (itA->GetLinkID(CellSide::SIDE_CA) == ID_NONE &&
				itB->CheckSharedEdge(itA->GetEdge(CellSide::SIDE_CA), side))
			{
				itA->SetLinkID(CellSide::SIDE_CA, itB->GetCellID());
				itB->SetLinkID(side, itA->GetCellID());
				linkLines_->AddPoint(itA->GetCenter() + addVec, itB->GetCenter() + addVec);
			}
		}
	}
}

NavCell* NavMesh::GetNavCell(int32_t cellID)
{
	for (uint32_t i = 0; i < cells_.size(); i++)
	{
		if (*cells_[i] == cellID) return cells_[i].get();
	}

	return nullptr;
}

float NavMesh::CalcCellDist(int32_t startID, int32_t endID)
{
	return Vector3(cells_[startID]->GetCenter() - cells_[endID]->GetCenter()).length();
}
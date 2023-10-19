#include "NavMesh.h"

#include "Collision.h"

#include <list>

void NavMesh::Initialize(const std::string& fileName)
{
	// モデル生成
	model_ = std::make_unique<Model>(fileName);

	// オブジェクト3D生成
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetRotation({ 0.0f, 180.0f, 0.0f });

	// セルを作成
	CreateCell();

	// セルをリンク
	LinkCell();

	std::vector<Vector3> route;

	RouteSearch(0, 5, route);
}

void NavMesh::MatUpdate()
{
	object_->MatUpdate();
}

void NavMesh::Draw()
{
	object_->Draw();
}

void NavMesh::RouteSearch(int32_t startID, int32_t endID, std::vector<Vector3>& outputRoute)
{
	std::vector<Vector3> route;
	std::vector<std::unique_ptr<NavNode>> nodes;
	std::vector<NavNode*> open;
	std::vector<int32_t> closeID;
	NavNode* goalNode = nullptr;

	// -----最初の処理----- //
	// 1. ノードを生成
	// 2. スタートIDをもとにセルのポインタを保存
	// 3. 推定コストを計算(スタート位置からエンド位置までの直線距離)
	// 4. スコアを計算(実スコア + 推定スコア)
	// 5. オープンリストに追加
	nodes.emplace_back(std::make_unique<NavNode>());
	nodes.back()->cell = GetNavCell(startID);
	nodes.back()->hCost = CalcCellDist(startID, endID);
	nodes.back()->GetScore();
	open.emplace_back(nodes.back().get());
#pragma endregion

#pragma region ゴールまで繰り返す
	bool isBreak = false;

	while (isBreak == false)
	{
		// オープンリストから一番スコアの低いノードを取り出す
		NavNode* minScoreNode = nullptr;
		float minScore = FLT_MAX;
		uint32_t deleteNum = 0;
		for (uint32_t i = 0; i < open.size(); i++)
		{
			if (open[i]->cell->GetCellID() == endID)
			{
				goalNode = open[i];
				isBreak = true;
				break;
			}

			if (open[i]->GetScore() < minScore)
			{
				minScoreNode = open[i];
				deleteNum = i;
			}
		}

		if (isBreak) break;

		// 取り出したノードのIDをクローズリストに追加
		closeID.emplace_back(open[deleteNum]->cell->GetCellID());

		// 取り出したノードをオープンリストから削除
		open.erase(open.begin() + deleteNum);

		// オープンリストから取り出したノードから新たにノードを生成
		std::vector<int32_t> linkID = minScoreNode->cell->GetLinkID();
		for (uint8_t i = 0; i < linkID.size(); i++)
		{
			if (linkID[i] == ID_NONE) continue;

			bool isClose = false;
			for (uint32_t j = 0; j < closeID.size(); j++)
			{
				if (linkID[i] == closeID[j])
				{
					isClose = true;
					break;
				}
			}

			if (isClose == false)
			{
				nodes.emplace_back(std::make_unique<NavNode>());
				nodes.back()->parent = minScoreNode;
				nodes.back()->cell = GetNavCell(linkID[i]);
				nodes.back()->hCost = CalcCellDist(linkID[i], endID);
				nodes.back()->GetScore();
				open.emplace_back(nodes.back().get());
			}
		}
	}

	NavNode* nowNode = nullptr;
	nowNode = goalNode;

	while (nowNode != nullptr)
	{
		route.emplace_back(nowNode->cell->GetCenter());
		nowNode = nowNode->parent;
	}
#pragma endregion

	outputRoute = route;
}

void NavMesh::RouteSearch(const Vector3& startVec, const Vector3& endVec, std::vector<Vector3>& outputRoute)
{
	std::vector<Vector3> route;
	std::vector<std::unique_ptr<NavNode>> nodes;
	std::vector<NavNode*> open;
	std::vector<int32_t> closeID;
	NavNode* goalNode = nullptr;
	Ray startRay = { startVec, {0.0f, -1.0f, 0.0f} };
	Ray endRay = { endVec, {0.0f, -1.0f, 0.0f} };
	int32_t startID = CheckRay2Cell(startRay);
	int32_t endID = CheckRay2Cell(endRay);

	// -----最初の処理----- //
	// 1. ノードを生成
	// 2. スタートIDをもとにセルのポインタを保存
	// 3. 推定コストを計算(スタート位置からエンド位置までの直線距離)
	// 4. スコアを計算(実スコア + 推定スコア)
	// 5. オープンリストに追加
	nodes.emplace_back(std::make_unique<NavNode>());
	nodes.back()->cell = GetNavCell(startID);
	nodes.back()->hCost = CalcCellDist(startID, endID);
	nodes.back()->GetScore();
	open.emplace_back(nodes.back().get());
#pragma endregion

#pragma region ゴールまで繰り返す
	bool isBreak = false;

	while (isBreak == false)
	{
		// オープンリストから一番スコアの低いノードを取り出す
		NavNode* minScoreNode = nullptr;
		float minScore = FLT_MAX;
		uint32_t deleteNum = 0;
		for (uint32_t i = 0; i < open.size(); i++)
		{
			if (open[i]->cell->GetCellID() == endID)
			{
				goalNode = open[i];
				isBreak = true;
				break;
			}

			if (open[i]->GetScore() < minScore)
			{
				minScoreNode = open[i];
				deleteNum = i;
			}
		}

		if (isBreak) break;

		// 取り出したノードのIDをクローズリストに追加
		closeID.emplace_back(open[deleteNum]->cell->GetCellID());

		// 取り出したノードをオープンリストから削除
		open.erase(open.begin() + deleteNum);

		// オープンリストから取り出したノードから新たにノードを生成
		std::vector<int32_t> linkID = minScoreNode->cell->GetLinkID();
		for (uint8_t i = 0; i < linkID.size(); i++)
		{
			if (linkID[i] == ID_NONE) continue;

			bool isClose = false;
			for (uint32_t j = 0; j < closeID.size(); j++)
			{
				if (linkID[i] == closeID[j])
				{
					isClose = true;
					break;
				}
			}

			if (isClose == false)
			{
				nodes.emplace_back(std::make_unique<NavNode>());
				nodes.back()->parent = minScoreNode;
				nodes.back()->cell = GetNavCell(linkID[i]);
				nodes.back()->hCost = CalcCellDist(linkID[i], endID);
				nodes.back()->GetScore();
				open.emplace_back(nodes.back().get());
			}
		}
	}

	NavNode* nowNode = nullptr;
	nowNode = goalNode;

	route.emplace_back(endVec);

	while (nowNode != nullptr)
	{
		route.emplace_back(nowNode->cell->GetCenter());
		nowNode = nowNode->parent;
	}

	route.emplace_back(startVec);

	std::vector<Vector3> resultRoute;
	resultRoute.resize(route.size());

	for (uint16_t i = 0; i < resultRoute.size(); i++)
	{
		resultRoute[i] = route[resultRoute.size() - i - 1];
	}
#pragma endregion

	outputRoute = resultRoute;
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
	}
}

void NavMesh::LinkCell()
{
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
			}

			// itAセルは辺BC側にリンクしているセルがない かつ
			// itBセルは辺BC側でitAセルとのリンクが可能である
			else if (itA->GetLinkID(CellSide::SIDE_BC) == ID_NONE &&
				itB->CheckSharedEdge(itA->GetEdge(CellSide::SIDE_BC), side))
			{
				itA->SetLinkID(CellSide::SIDE_BC, itB->GetCellID());
				itB->SetLinkID(side, itA->GetCellID());
			}

			// itAセルは辺CA側にリンクしているセルがない かつ
			// itBセルは辺CA側でitAセルとのリンクが可能である
			else if (itA->GetLinkID(CellSide::SIDE_CA) == ID_NONE &&
				itB->CheckSharedEdge(itA->GetEdge(CellSide::SIDE_CA), side))
			{
				itA->SetLinkID(CellSide::SIDE_CA, itB->GetCellID());
				itB->SetLinkID(side, itA->GetCellID());
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
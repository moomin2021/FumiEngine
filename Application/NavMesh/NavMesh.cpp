#include "NavMesh.h"

#include "Collision.h"
#include "PipelineManager.h"

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

void NavMesh::RouteSearch(const Vector3& startVec, const Vector3& goalVec, std::vector<Vector3>& outputRoute)
{
	// -----変数説明一覧----- //
	// 1. ノード
	// 2. オープンリスト(探索候補ノードのリスト)
	// 3. クローズリスト(探索済みノードのリスト)
	// 4. ゴールノード
	std::vector<std::unique_ptr<NavNode>> nodes;
	std::vector<NavNode*> open;
	std::vector<NavNode*> close;
	NavNode* goalNode = nullptr;

	// スタートとゴールのセルIDを求める
	int32_t startID = CheckRay2Cell(Ray(startVec, { 0.0f, -1.0f, 0.0f }));
	int32_t goalID = CheckRay2Cell(Ray(goalVec, { 0.0f, -1.0f, 0.0f }));

	// -----最初の処理----- //
	// 1. ノードを生成
	// 2. スタートIDをもとにセルのポインタを保存
	// 3. 推定コストを計算(スタート位置からエンド位置までの直線距離)
	// 4. スコアを計算(実スコア + 推定スコア)
	// 5. オープンリストに追加
	nodes.emplace_back(std::make_unique<NavNode>());
	nodes.back()->cell = GetNavCell(startID);
	nodes.back()->hCost = CalcCellDist(startID, goalID);
	nodes.back()->CalcScore();
	open.emplace_back(nodes.back().get());

	// ルート探索をゴールまで続ける
	bool isGoal = false;
	while (isGoal == false)
	{
		// 1. 最もスコアの低いノードのポインタ
		// 2. 最も低いスコアの保存する変数
		// 3. 削除するインデックス
		NavNode* minScoreNode = nullptr;
		float minScore = FLT_MAX;
		uint32_t deleteIndex = 0;

		// -----オープンリストから最もスコアの低いノードを取り出す----- //
		for (uint32_t i = 0; i < open.size(); i++)
		{
			// ノードのセルIDがゴールIDと同じなら探索を終わる
			if (open[i]->cell->GetCellID() == goalID)
			{
				// 1. ゴールノードを保存
				// 2. ゴールしたのでフラグをTRUEにする
				goalNode = open[i];
				isGoal = true;
				break;
			}

			// スコアが今までで最も低かったら保存する
			if (open[i]->score < minScore)
			{
				// 1. 今までで最も低いノードとして保存
				// 2. 今までで最も低いスコアとして保存
				minScoreNode = open[i];
				minScore = open[i]->score;
				deleteIndex = i;
			}
		}

		// ゴールしたのでループを抜ける
		if (isGoal) break;

		// 1. 最もスコアの低いノードをクローズリストに追加
		// 2. 最もスコアの低いノードをオープンリストから削除
		close.emplace_back(minScoreNode);
		open.erase(open.begin() + deleteIndex);

		// -----オープンリストから取り出したノードからリンクしているセルを基にノードを生成----- //
		// ノードのセルにリンクしているセルIDを取得
		std::vector<int32_t> linkID = minScoreNode->cell->GetLinkID();

		// ノードを生成
		for (uint8_t i = 0; i < linkID.size(); i++)
		{
			// リンクしていなかったら飛ばす
			if (linkID[i] == ID_NONE) continue;

			// リンクIDからクローズリストにあるか検索
			bool isClose = false;
			for (uint32_t j = 0; j < close.size(); j++)
			{
				if (linkID[i] == close[j]->cell->GetCellID())
				{
					isClose = true;
					break;
				}
			}

			// クローズリストにあったらノードを生成せず飛ばす
			if (isClose) continue;

			// 1. ノードの生成
			// 2. 最もスコアの低いノードを親ノードとして登録
			// 3. IDから対応するセルを登録
			// 4. 移動コストを計算
			// 5. 推定コストを計算
			// 6. スコアを計算
			// 7. オープンリストに追加
			nodes.emplace_back(std::make_unique<NavNode>());
			nodes.back()->parent = minScoreNode;
			nodes.back()->cell = GetNavCell(linkID[i]);
			nodes.back()->cCost = minScoreNode->cCost + CalcCellDist(GetNavCell(linkID[i])->GetCellID(), minScoreNode->cell->GetCellID());
			nodes.back()->hCost = CalcCellDist(linkID[i], goalID);
			nodes.back()->CalcScore();
			open.emplace_back(nodes.back().get());
		}
	}

	// 1. Funnelアルゴリズムで使う三角形
	// 2. 現在のノード(ゴールノードを設定)
	std::vector<Triangle> triangles = {};
	NavNode* nowNode = goalNode;

	// ゴールノードからノードを辿る
	while (nowNode != nullptr)
	{
		triangles.emplace_back(nowNode->cell->GetTriangle());
		nowNode = nowNode->parent;
	}

	// 先頭がゴールになっているので順番を反転
	std::reverse(triangles.begin(), triangles.end());

	float num = 0;

	outputRoute[0] = Vector3(num, num, num);
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
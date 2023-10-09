#include "NavMesh.h"

#include <fstream>
#include <sstream>
#include <cassert>
#include <list>

using namespace std;

void NavMesh::LoadMesh(std::string name)
{
	// ファイルストリーム
	std::ifstream file;

	// .objファイルを開く
	const string modelName = name;
	const string fileName = modelName + ".obj";
	const string directoryPath = "Resources/" + modelName + "/";
	file.open(directoryPath + fileName);

	// ファイルオープン失敗をチェック
	assert(!file.fail());

	// 1行ずつ読み込む
	string line;
	while (getline(file, line))
	{

		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		string key;
		getline(line_stream, key, ' ');

		// 先頭文字列が[v]なら頂点座標
		if (key == "v")
		{
			// X, Y, Z座標読み込み
			Vector3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;

			// 座標データに追加
			vertices_.emplace_back(position);
		}

		// 先頭文字列が[f]ならポリゴン(三角形)
		else if (key == "f")
		{
			std::vector<uint32_t> index(3);
			uint8_t count = 0;

			// 半角スペース区切りで行の続きを読み込む
			string index_string;
			while (getline(line_stream, index_string, ' '))
			{
				// 頂点インデックス1個分の文字列をストリームに変換して解析しやすくする
				std::istringstream index_stream(index_string);
				index_stream >> index[count];
				count++;
			}

			indexes_.emplace_back(Vector3{ (float)index[0] - 1, (float)index[1] - 1, (float)index[2] - 1});
		}
	}

	// ファイルを閉じる
	file.close();

	// セルを作成
	CreateCell();

	// セルをリンク
	LinkCell();
}

void NavMesh::CreateCell()
{
	for (uint32_t i = 0; i < indexes_.size(); i++)
	{
		cells_.emplace_back(std::make_shared<NavCell>(NavCell(
			i, vertices_[(uint8_t)indexes_[i].x], vertices_[(uint8_t)indexes_[i].y], vertices_[(uint8_t)indexes_[i].z])));
	}
}

void NavMesh::LinkCell()
{
	for (auto& current : cells_)
	{
		for (auto& other : cells_)
		{
			// 同じセルまたはすでにリンク済みの場合飛ばす
			if (current == other || current->AlreadyLinkCell(other->GetCellID())) continue;

			if (current->ValidLinkCount() == 3) break;

			CellSide side;

			// currentセルは辺AB側にリンクしているセルがない かつ
			// otherセルは辺AB側でcurrentセルとのリンクが可能である
			if (current->GetLinkID(CellSide::SIDE_AB) == -1 &&
				other->CheckSharedEdge(current->GetEdge(CellSide::SIDE_AB), side))
			{
				current->SetLinkID(CellSide::SIDE_AB, other->GetCellID());
				other->SetLinkID(side, current->GetCellID());
			}

			// currentセルは辺BC側にリンクしているセルがない かつ
			// otherセルは辺BC側でcurrentセルとのリンクが可能である
			if (current->GetLinkID(CellSide::SIDE_BC) == -1 &&
				other->CheckSharedEdge(current->GetEdge(CellSide::SIDE_BC), side))
			{
				current->SetLinkID(CellSide::SIDE_BC, other->GetCellID());
				other->SetLinkID(side, current->GetCellID());
			}

			// currentセルは辺CA側にリンクしているセルがない かつ
			// otherセルは辺CA側でcurrentセルとのリンクが可能である
			if (current->GetLinkID(CellSide::SIDE_CA) == -1 &&
				other->CheckSharedEdge(current->GetEdge(CellSide::SIDE_CA), side))
			{
				current->SetLinkID(CellSide::SIDE_CA, other->GetCellID());
				other->SetLinkID(side, current->GetCellID());
			}
		}
	}
}

//bool NavMeshPathfinder::FindPath(NavMesh* navMesh, int32_t startCellIndex, const Vector3& startPos, int32_t goalCellIndex, const Vector3& endPos)
//{
//	//std::shared_ptr<NavNode> current = nullptr;
//	//std::list<shared_ptr<NavNode>> open, close;
//	//std::shared_ptr<NavNode> startNode = std::make_shared<NavNode>(goalCellIndex);
//	//startNode->gCost = 0.0f;
//
//	//open.emplace_front(startNode);
//	//open.sort();
//
//	//// 過剰なループ防止
//	//int32_t loopCount = 0;
//	//const int32_t loopLimit = 1000000;
//
//	// スタート地点のノードを作成
//	std::shared_ptr<NavNode> startNode = std::make_shared<NavNode>(startCellIndex);
//	startNode->gCost = 0.0f;
//}

//void NavMeshPathfinder::SearchRoute(std::vector<std::shared_ptr<NavCell>> navCells, int32_t startCellID, int32_t endCellID)
//{
//	// スタート地点のノードを作成
//	nodes_.emplace_back(startCellID);
//
//	while (true)
//	{
//
//	}
//}
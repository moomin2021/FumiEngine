#include "NavMesh.h"

#include <fstream>
#include <sstream>
#include <cassert>

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

	std::vector<std::unique_ptr<Cell>> cells;

	for (uint32_t i = 0; i < indexes_.size(); i++)
	{
		cells.emplace_back(std::make_unique<Cell>(Cell(i, vertices_[(uint8_t)indexes_[i].x], vertices_[(uint8_t)indexes_[i].y], vertices_[(uint8_t)indexes_[i].z])));
	}

	for (auto& current : cells)
	{
		for (auto& other : cells)
		{
			// 同じセルまたはすでにリンク済みの場合飛ばす
			if (current == other || current->AlreadyLinkCell(other->cellID)) continue;

			if (current->ValidLinkCount() == 3) break;

			CellSide side;

			// currentセルは辺AB側にリンクしているセルがない かつ
			// otherセルは辺AB側でcurrentセルとのリンクが可能である
			if (current->GetLink(CellSide::SIDE_AB) == -1 &&
				other->CheckSharedEdge(current->GetEdge(CellSide::SIDE_AB), side))
			{
				current->SetLink(CellSide::SIDE_AB, other->cellID);
				other->SetLink(side, current->cellID);
			}

			// currentセルは辺BC側にリンクしているセルがない かつ
			// otherセルは辺BC側でcurrentセルとのリンクが可能である
			if (current->GetLink(CellSide::SIDE_BC) == -1 &&
				other->CheckSharedEdge(current->GetEdge(CellSide::SIDE_BC), side))
			{
				current->SetLink(CellSide::SIDE_BC, other->cellID);
				other->SetLink(side, current->cellID);
			}

			// currentセルは辺CA側にリンクしているセルがない かつ
			// otherセルは辺CA側でcurrentセルとのリンクが可能である
			if (current->GetLink(CellSide::SIDE_CA) == -1 &&
				other->CheckSharedEdge(current->GetEdge(CellSide::SIDE_CA), side))
			{
				current->SetLink(CellSide::SIDE_CA, other->cellID);
				other->SetLink(side, current->cellID);
			}
		}
	}
}

Cell::Cell(int32_t aCellID, const Vector3& pointA, const Vector3& pointB, const Vector3& pointC) :
	cellID(aCellID)
{
	vertices.resize(3);
	edges.resize(3);
	link.resize(3);

	vertices[(uint8_t)CellVert::VERT_A] = pointA;
	vertices[(uint8_t)CellVert::VERT_B] = pointB;
	vertices[(uint8_t)CellVert::VERT_C] = pointC;

	edges[(uint8_t)CellSide::SIDE_AB] = CellEdge(pointA, pointB);
	edges[(uint8_t)CellSide::SIDE_BC] = CellEdge(pointB, pointC);
	edges[(uint8_t)CellSide::SIDE_CA] = CellEdge(pointC, pointA);

	link[(uint8_t)CellSide::SIDE_AB] = -1;
	link[(uint8_t)CellSide::SIDE_BC] = -1;
	link[(uint8_t)CellSide::SIDE_CA] = -1;

	center = (pointA + pointB + pointC) / 3.0f;
	normal = Vector3(pointB - pointA).cross(pointC - pointA);
	normal.normalize();
}

bool Cell::AlreadyLinkCell(int32_t inCellID)
{
	bool result = false;

	for (size_t i = 0;i < 3;i++)
	{
		if (link[i] == inCellID) result = true;
	}

	return result;
}

uint8_t Cell::ValidLinkCount()
{
	uint8_t count = 0;
	count += (link[0] != -1);
	count += (link[1] != -1);
	count += (link[2] != -1);
	return count;
}

bool Cell::CheckSharedEdge(const CellEdge& Edge, CellSide& SideType)
{
	// Edgeの始端と終端と自身の頂点位置とを照らし合わせ該当した辺方向にセルを記録する

	if (vertices[(uint8_t)CellVert::VERT_A] == Edge.left)
	{
		if (vertices[(uint8_t)CellVert::VERT_B] == Edge.right)
		{
			SideType = CellSide::SIDE_AB;
			return true;
		}
		else if (vertices[(uint8_t)CellVert::VERT_C] == Edge.right)
		{
			SideType = CellSide::SIDE_CA;
			return true;
		}
	}

	if (vertices[(uint8_t)CellVert::VERT_B] == Edge.left)
	{
		if (vertices[(uint8_t)CellVert::VERT_A] == Edge.right)
		{
			SideType = CellSide::SIDE_AB;
			return true;
		}
		else if (vertices[(uint8_t)CellVert::VERT_C] == Edge.right)
		{
			SideType = CellSide::SIDE_BC;
			return true;
		}
	}

	if (vertices[(uint8_t)CellVert::VERT_C] == Edge.left)
	{
		if (vertices[(uint8_t)CellVert::VERT_A] == Edge.right)
		{
			SideType = CellSide::SIDE_CA;
			return true;
		}
		else if (vertices[(uint8_t)CellVert::VERT_B] == Edge.right)
		{
			SideType = CellSide::SIDE_BC;
			return true;
		}
	}


	return false;
}

#include "NavMesh.h"

void NavMesh::Initialize(const std::string& fileName)
{
	// モデル生成
	model_ = std::make_unique<Model>(fileName);

	// オブジェクト3D生成
	object_ = std::make_unique<Object3D>(model_.get());
	
	// セルを作成
	CreateCell();

	// セルをリンク
	LinkCell();
}

void NavMesh::MatUpdate()
{
	object_->MatUpdate();
}

void NavMesh::Draw()
{
	object_->Draw();
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
			if (itA->LinkCount()) continue;

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
			if (itA->GetLinkID(CellSide::SIDE_BC) == ID_NONE &&
				itB->CheckSharedEdge(itA->GetEdge(CellSide::SIDE_BC), side))
			{
				itA->SetLinkID(CellSide::SIDE_BC, itB->GetCellID());
				itB->SetLinkID(side, itA->GetCellID());
			}

			// itAセルは辺CA側にリンクしているセルがない かつ
			// itBセルは辺CA側でitAセルとのリンクが可能である
			if (itA->GetLinkID(CellSide::SIDE_CA) == ID_NONE &&
				itB->CheckSharedEdge(itA->GetEdge(CellSide::SIDE_CA), side))
			{
				itA->SetLinkID(CellSide::SIDE_CA, itB->GetCellID());
				itB->SetLinkID(side, itA->GetCellID());
			}
		}
	}
}
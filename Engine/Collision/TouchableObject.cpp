#include "TouchableObject.h"
#include "MeshCollider.h"

TouchableObject::TouchableObject(Model* model) : Object3D(model)
{
	
}

void TouchableObject::Initialize()
{
	// コライダー追加
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	collider->ConstructTriangles(model_);
}

void TouchableObject::Update()
{
	SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
}
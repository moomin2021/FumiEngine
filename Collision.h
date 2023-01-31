#pragma once
#include "CollisionPrimitive.h"

class Collision {
public:
	/// <summary>
	/// ‹…‚Æ•½–Ê‚Ì“–‚½‚è”»’è
	/// </summary>
	/// <param name="sphere"> ‹… </param>
	/// <param name="plane"> •½–Ê </param>
	/// <param name="inter"> Œğ“_(•½–Êã‚ÌÅ‹ßÚ“_) </param>
	/// <returns> Œğ·‚µ‚Ä‚¢‚é‚©”Û‚© </returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);
};
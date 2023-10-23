#pragma once
class BaseLayer {
public:
	BaseLayer() {};
	virtual ~BaseLayer() {};
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void MatUpdate() = 0;
};
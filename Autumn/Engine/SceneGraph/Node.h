#ifndef _NODE_
#define _NODE_

#include <vector>


#ifndef _VECTOR1_
#include "Core/Math/Vector1.h"
#endif

#ifndef _VECTOR2_
#include "Core/Math/Vector2.h"
#endif

#ifndef _VECTOR3_
#include "Core/Math/Vector3.h"
#endif

#ifndef _VECTOR4_
#include "Core/Math/Vector4.h"
#endif

#ifndef _MATRIX4X4_
#include "Core/Math/Matrix4x4.h"
#endif


class Node;

class Node
{
protected:
	Node * m_pParent;
	std::vector<Node*> m_vChild;

	Matrix4x4 m_mLocalMatrix;
	Matrix4x4 m_mGlobalMatrix;

	// TODO bounding sphere

public:
	Node();
	virtual ~Node();

	virtual HRESULT Create();
	virtual HRESULT Destroy();

	virtual void AddChild(Node * _pNode);
	virtual void RemoveChild(Node * _pNode);

	void SetLocalMatrix(const Matrix4x4 & _Matrix);
	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void Scale(float x, float y, float z);

	virtual void Render();
	virtual bool Culling();

};

#endif // _NODE_
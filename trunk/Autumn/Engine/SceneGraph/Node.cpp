#include "stdafx.h"

#ifndef _NODE_
#include "Engine/SceneGraph/Node.h"
#endif

Node::Node()
: m_pParent(NULL)
{
	m_vChild.clear();
}

Node::~Node()
{
}

HRESULT Node::Create()
{
	m_mLocalMatrix.SetIdentity();
	m_mGlobalMatrix.SetIdentity();
	return S_OK;
}

HRESULT Node::Destroy()
{
	for(std::vector<Node *>::iterator it = m_vChild.begin(); it != m_vChild.end(); it++)
	{
		Node * pChild = (*it);
		pChild->Destroy();
		SAFE_DELETE( pChild )
	}

	m_vChild.clear();

	return S_OK;
}

void Node::AddChild(Node * _pNode)
{
	m_vChild.push_back(_pNode);
	_pNode->m_pParent = this;
}


void Node::RemoveChild(Node * _pNode)
{
	for(std::vector<Node *>::iterator it = m_vChild.begin(); it != m_vChild.end(); it++)
	{
		Node * pNode = (*it);
		if( pNode == _pNode )
		{
			m_vChild.erase(it);
			return;
		}
	}
}

void Node::Translate(float x, float y, float z)
{
	m_mLocalMatrix.Translate(x, y, z);
}

void Node::Rotate(float x, float y, float z)
{
	m_mLocalMatrix.Rotate(x, y, z);
}

void Node::Scale(float x, float y, float z)
{
	m_mLocalMatrix.Scale(x, y, z);
}

void Node::Render( EngineCamera * _pCamera )
{

}

bool Node::Culling()
{
	if( m_pParent )
	{
		m_mGlobalMatrix = m_pParent->m_mGlobalMatrix * m_mLocalMatrix;
	}
	else
	{
		m_mGlobalMatrix = m_mLocalMatrix;
	}

	return true;
}
#ifndef _ATLAS_
#define _ATLAS_

class Texture2D;

class Atlas
{
private:
	Texture2D		* m_pTexture;
	int				m_iIconWidth;
	int				m_iIconHeight;
	int				m_nCols;
	int				m_nRows;

public:
	Atlas();
	~Atlas();

	HRESULT Create(const char * _name);
	HRESULT Destroy();

	bool GetRectByIndex(unsigned int _Index, float & _t0x, float & _t0y, float & _t1x, float&  _t1y) const;

	inline Texture2D *	GetTexture() const { return m_pTexture; }
	inline unsigned int	GetAtlasWidth()  const { return ( m_pTexture ? m_pTexture->GetWidth() : 0 ); }
	inline unsigned int	GetAtlasHeight() const { return ( m_pTexture ? m_pTexture->GetHeight() : 0 ); }
	inline int	GetIconWidth()  const { return m_iIconWidth; }
	inline int	GetIconHeight() const { return m_iIconHeight; }
	inline void	SetIconWidth(unsigned int _val)  { m_iIconWidth = _val; }
	inline void	SetIconHeight(unsigned int _val) { m_iIconHeight = _val; }


};

#endif // _ATLAS_
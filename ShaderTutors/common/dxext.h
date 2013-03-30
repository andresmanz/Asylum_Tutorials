
#ifndef _DXEXT_H_
#define _DXEXT_H_

#include <d3dx9.h>
#include <string>

class DXObject
{
private:
	IDirect3DDevice9*		device;
	ID3DXMesh*				mesh;
	D3DXMATERIAL*			materials;
	IDirect3DTexture9**		textures;
	DWORD					nummaterials;

	void Clean();

public:
	enum drawflags
	{
		Opaque = 1,
		Transparent = 2,
		Material = 4,
		All = 7
	};

	DXObject(IDirect3DDevice9* d3ddevice);
	~DXObject();

	bool Load(const std::string& file);
	bool Save(const std::string& file);

	void Draw(unsigned int flags = All, LPD3DXEFFECT effect = NULL);
	void DrawSubset(DWORD subset, unsigned int flags = All, LPD3DXEFFECT effect = NULL);
	void DrawExcept(DWORD subset, unsigned int flags = All, LPD3DXEFFECT effect = NULL);
};

class DXLight
{
protected:
	D3DXCOLOR	color;
	DWORD		shadowsize;
	DWORD		shadowtype;
	bool		needsredraw;
	bool		needsblur;

public:
	enum lighttype
	{
		Static = 0,
		Dynamic = 1
	};

	virtual ~DXLight();

	virtual void CreateShadowMap(LPDIRECT3DDEVICE9 device, DWORD type, DWORD size) = 0;

	inline const D3DXCOLOR& GetColor() const {
		return color;
	}
};

class DXDirectionalLight : public DXLight
{
private:
	LPDIRECT3DTEXTURE9		shadowmap;
	LPDIRECT3DTEXTURE9		blur;
	D3DXVECTOR4				direction;

public:
	DXDirectionalLight(const D3DXCOLOR& color, const D3DXVECTOR4& direction);
	~DXDirectionalLight();

	void DrawShadowMap(LPDIRECT3DDEVICE9 device, LPD3DXEFFECT effect, void (*drawcallback)(LPD3DXEFFECT));
	void BlurShadowMap(LPDIRECT3DDEVICE9 device, LPD3DXEFFECT effect);
	void CreateShadowMap(LPDIRECT3DDEVICE9 device, DWORD type, DWORD size);
	void GetViewProjMatrix(D3DXMATRIX& out, const D3DXVECTOR3& origin);

	inline D3DXVECTOR4& GetDirection() {
		return direction;
	}

	inline LPDIRECT3DTEXTURE9 GetShadowMap() {
		return shadowmap;
	}
};

class DXPointLight : public DXLight
{
private:
	LPDIRECT3DCUBETEXTURE9	shadowmap;
	LPDIRECT3DCUBETEXTURE9	blur;
	D3DXVECTOR3				position;
	float					radius;

public:
	DXPointLight(const D3DXCOLOR& color, const D3DXVECTOR3& pos, float radius);
	~DXPointLight();

	void DrawShadowMap(LPDIRECT3DDEVICE9 device, LPD3DXEFFECT effect, void (*drawcallback)(LPD3DXEFFECT));
	void BlurShadowMap(LPDIRECT3DDEVICE9 device, LPD3DXEFFECT effect);
	void CreateShadowMap(LPDIRECT3DDEVICE9 device, DWORD type, DWORD size);
	void GetScissorRect(RECT& out, const D3DXMATRIX& view, const D3DXMATRIX& proj, int w, int h) const;
	void GetViewProjMatrix(D3DXMATRIX& out, DWORD face);

	inline float GetRadius() const {
		return radius;
	}

	inline D3DXVECTOR3& GetPosition() {
		return position;
	}

	inline LPDIRECT3DCUBETEXTURE9 GetShadowMap() {
		return shadowmap;
	}
};

HRESULT DXLoadMeshFromQM(LPCTSTR file, DWORD options, LPDIRECT3DDEVICE9 d3ddevice, D3DXMATERIAL** materials, DWORD* nummaterials, LPD3DXMESH* mesh);
HRESULT DXSaveMeshToQM(LPCTSTR file, LPD3DXMESH mesh, D3DXMATERIAL* materials, DWORD nummaterials);
HRESULT DXCreateEffect(LPCTSTR file, LPDIRECT3DDEVICE9 d3ddevice, LPD3DXEFFECT* out);
HRESULT DXGenTangentFrame(LPDIRECT3DDEVICE9 d3ddevice, LPD3DXMESH* mesh);

void DXRenderText(const std::string& str, LPDIRECT3DTEXTURE9 tex, DWORD width, DWORD height);
void DXGetCubemapViewMatrix(D3DXMATRIX& out, DWORD i, const D3DXVECTOR3& eye);

#endif
#pragma once

class ClientMgr
{
private:
	bool m_bServer;


private:
	void Join();
	void Damage(float _iDamage, bool _bool);
	void GuardDamage(float _fDamage, bool _bool);
	void SetHP(float _iDamage, bool _bool);
	void SetState(UINT _iState, bool _bool);

public:
	bool run();
	void SetServer(bool _On) { m_bServer = _On; }

public:
	static ClientMgr& Manager()
	{
		static ClientMgr inst;
		return inst;
	}

private:
	ClientMgr();
	~ClientMgr();
};


#pragma once

class CServerManager
{
private:
	bool m_b1P;
	bool m_b2P;

public:
	bool init();
	bool run();

private:
	void Join();

public:
	static CServerManager& Manager()
	{
		static CServerManager inst;
		return inst;
	}

private:
	CServerManager();
	~CServerManager();
};


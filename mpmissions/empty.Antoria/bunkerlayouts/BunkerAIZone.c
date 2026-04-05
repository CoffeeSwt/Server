// ========================== BUNKER AI ZONE ==========================
class BunkerAIZone
{
	protected vector m_Center;
	protected float m_Radius;
	protected int m_WaveSize;
	protected int m_CooldownMs;
	protected bool m_Locked;                  // simple cooldown lock
	protected ref set<PlayerBase> m_Inside;   // who is currently inside

	void BunkerAIZone(vector center, float radius = 20.0, int waveSize = 10, int cooldownMs = 300000)
	{
		m_Center     = center;
		m_Radius     = radius;
		m_WaveSize   = FP_ZED_WAVE_SIZE;   // <-- ignore ctor arg; always use the one constant
		m_CooldownMs = cooldownMs;
		m_Inside     = new set<PlayerBase>;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Check, 1000, true);
	}

	void ~BunkerAIZone()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Remove(Check);
	}

	protected void Check()
	{
		if (!GetGame().IsServer()) return;

		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		foreach (Man m : players)
		{
			PlayerBase p = PlayerBase.Cast(m);
			if (!p) continue;

			bool inNow = vector.DistanceSq(p.GetPosition(), m_Center) <= (m_Radius * m_Radius);
			int idx = m_Inside.Find(p);          // index in the set (or -1 if not found)
			bool wasIn = idx != -1;

			if (inNow && !wasIn)
			{
				m_Inside.Insert(p);
				OnEnter(p);
			}
			else if (!inNow && wasIn)
			{
				m_Inside.Remove(idx);            // <-- Remove by index, not by object
				OnLeave(p);
			}
		}
	}

	protected void OnEnter(PlayerBase player)
	{
		if (m_Locked) return;
		if (FP_HaveInfectedWithin(m_Center, 300.0)) return;
		SpawnWaveAtRandomFloors(m_WaveSize);
		m_Locked = true;
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Unlock, m_CooldownMs, false);
	}

	protected void OnLeave(PlayerBase player)
	{
		// optional: do something when leaving
	}

	protected void Unlock()
	{
		m_Locked = false;
	}

	protected void SpawnWave()
	{
		// Choose a few appropriate infected types (swap for your theme)
		ref array<string> pool = { "ZmbM_NBC_White", "ZmbM_NBC_Grey", "ZmbM_NBC_Yellow" };

		for (int i = 0; i < m_WaveSize; i++)
		{
			vector offset = Vector( Math.RandomFloat(-m_Radius * 0.7, m_Radius * 0.7), 0, Math.RandomFloat(-m_Radius * 0.7, m_Radius * 0.7) );

			vector pos = m_Center + offset;

			// put zeds on the ground; if your floors are navmeshed this is fine inside too
			pos[1] = GetGame().SurfaceY(pos[0], pos[2]);

			string type = pool.Get(Math.RandomInt(0, pool.Count()));
			// EntityAI ent = EntityAI.Cast(GetGame().CreateObjectEx(type, pos, ECE_INITAI | ECE_KEEPHEIGHT | ECE_NOLIFETIME | ECE_CREATEPHYSICS));
			EntityAI ent = EntityAI.Cast(GetGame().CreateObjectEx(type, pos, ECE_SETUP | ECE_INITAI | ECE_EQUIP_ATTACHMENTS | ECE_KEEPHEIGHT | ECE_NOSURFACEALIGN | ECE_NOLIFETIME | ECE_CREATEPHYSICS));

			DayZInfected zmb = DayZInfected.Cast(ent);
			if (zmb)
			{
				zmb.PlaceOnSurface();
				// optional: set patrol/target here
			}
		}
	}
}
void main()
{
	/*
	  [Namalsk] CE 初始化（离线）
	*/
	CreateHive();
	GetHive().InitOffline();

	/*
	  [Namalsk] 任务时间初始化
	   在 CE 初始化之后执行，用于判断存档中的任务时间是否超出了要求的时间范围
	   当前推荐的时间范围为：
		12/1 -> 12/31
		请注意，游戏玩法特性与任务日期（存储在存档中）绑定，因此应该始终保持在这个时间段内！
	   在以下设置下：
		白天加速 6 倍（serverTimeAcceleration=6），现实时间中平均白天约为 78 分钟
		夜晚加速 24 倍（serverNightTimeAcceleration=4），现实时间中平均夜晚约为 26 分钟
	*/
	int year, month, day, hour, minute;
	GetGame().GetWorld().GetDate( year, month, day, hour, minute );

    if ( month < 12 )
    {
    	year = 2011;
        month = 12;
        day = 1;
		
		GetGame().GetWorld().SetDate( year, month, day, hour, minute );
	}
};

class CustomMission: MissionServer
{
	override void OnInit()
	{
		super.OnInit();

		// 推荐保留这段代码，否则事件系统会自动启用并按默认参数运行
		// 如果不使用 Namalsk Survival，请将整个代码块注释掉
		if ( m_EventManagerServer )
		{
			// 启用/禁用事件系统、事件之间的最短时间、最长时间、以及同时存在的最大事件数量
			m_EventManagerServer.OnInitServer( true, 550, 1000, 2 );
			// 注册事件及其触发概率
			m_EventManagerServer.RegisterEvent( Aurora, 0.85 );
			m_EventManagerServer.RegisterEvent( Blizzard, 0.6 );
			m_EventManagerServer.RegisterEvent( ExtremeCold, 0.3 );
			m_EventManagerServer.RegisterEvent( SnowfallE, 0.6 );
			m_EventManagerServer.RegisterEvent( EVRStorm, 0.45 );
			m_EventManagerServer.RegisterEvent( HeavyFog, 0.3 );
		}
	}

	void SetRandomHealth(EntityAI itemEnt)
	{
		if (itemEnt)
		{
			float rndHlt = Math.RandomFloat( 0.50, 0.85 );
			itemEnt.SetHealth01( "", "", rndHlt );
		}
	}

	override void StartingEquipSetup( PlayerBase player, bool clothesChosen )
	{
		EntityAI itemClothing;
		EntityAI itemEnt;
		ItemBase itemBs;
		float rand;
		
		// 上衣
		itemClothing = player.FindAttachmentBySlotName( "Body" );
		if ( itemClothing )
		{
			SetRandomHealth( itemClothing );

			itemEnt = itemClothing.GetInventory().CreateInInventory( "Rag" );
			if ( Class.CastTo( itemBs, itemEnt ) )
			{
				SetRandomHealth( itemEnt );
				itemBs.SetQuantity( 4 );
				itemBs.SetCleanness( 1 );
			}
			player.SetQuickBarEntityShortcut( itemEnt, 0 );
			
			itemEnt = itemClothing.GetInventory().CreateInInventory( "RoadFlare" );
			SetRandomHealth( itemEnt );
			itemEnt = itemClothing.GetInventory().CreateInInventory( "RoadFlare" );
			SetRandomHealth( itemEnt );
			player.SetQuickBarEntityShortcut( itemEnt, 1 );
		}

		// 裤子
		itemClothing = player.FindAttachmentBySlotName( "Legs" );
		if ( itemClothing )
		{
			SetRandomHealth( itemClothing );

			itemEnt = itemClothing.GetInventory().CreateInInventory( "Heatpack" );
			SetRandomHealth( itemEnt );

			int throwDice = Math.RandomInt( 0, 2 );
			if ( throwDice == 0 )
				itemEnt = itemClothing.GetInventory().CreateInInventory( "dzn_tool_watch" );
			else
				itemEnt = itemClothing.GetInventory().CreateInInventory( "dzn_tool_watch2" );
			player.SetQuickBarEntityShortcut( itemEnt, 2 );
		}

		// 鞋子
		itemClothing = player.FindAttachmentBySlotName( "Feet" );
		if ( itemClothing )
		{
			SetRandomHealth( itemClothing );
		}

		// 提高新出生角色的水分和能量数值（用于补偿食物冻结以及水井更难获取的问题）
		player.GetStatWater().Set( 900 );
		player.GetStatEnergy().Set( 1100 );

		// 临时增加对感冒的抗性
		player.SetTemporaryResistanceToAgent(eAgents.INFLUENZA, 900);
	}
};
  
Mission CreateCustomMission(string path)
{
	return new CustomMission();
};

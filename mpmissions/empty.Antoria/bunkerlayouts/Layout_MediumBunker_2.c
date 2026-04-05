class Layout_MediumBunker_2 : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 15;
		const int H = 15;

// 1) Build Grid
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

// 2) HALLWAYS ( FP_HALL )
		FP_GridFillRect(	grid,	0,	0,	15,	2,	FP_HALL );
		FP_GridFillRect(	grid,	0,	0,	2,	15,	FP_HALL );
		FP_GridFillRect(	grid,	13,	0,	2,	15,	FP_HALL );
		FP_GridFillRect(	grid,	0,	13,	15,	2,	FP_HALL );

// 3) ROOMS ( FP_ROOM )
		FP_GridFillRect(	grid,	2,	2, 	5,	5,	FP_ROOM );
		FP_GridFillRect(	grid, 	8,	2,	5, 	5, 	FP_ROOM );
		FP_GridFillRect(	grid, 	2,	8,	5, 	5, 	FP_ROOM );
		FP_GridFillRect(	grid, 	8,	8,	5, 	5, 	FP_ROOM );

// 4) Spawn floors for R + H, record floor objects
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid(floorObjs, W, H);
		
		for ( int y = 0; y < H; y++ )
		{
			for ( int x = 0; x < W; x++ )
			{
				int cell = grid[y][x];
				
				if ( cell != FP_ROOM && cell != FP_HALL )
					continue;
				
				Object f = FP_SpawnFloorAt( origin, x, y );
				floorObjs[y][x] = f;
			}
		}
		g_FP_FloorObjs = floorObjs;

// 5) Doors: one coded doorframe per contiguous room block
		array<ref array<bool>> visited = new array<ref array<bool>>;
		
		for ( int vy = 0; vy < H; vy++ )
		{
			auto vrow = new array<bool>;
			vrow.Resize( W );
			visited.Insert( vrow );
		}

		array<ref Param3<int,int,int>> doorEdges = new array<ref Param3<int,int,int>>;
		
		for ( int yy = 0; yy < H; yy++)
		{
			for ( int xx = 0; xx < W; xx++ )
			{
				if ( grid[yy][xx] == FP_ROOM && !visited[yy][xx] )
					FP_PlaceOneDoorForRoomRegion( grid, W, H, origin, xx, yy, visited, doorEdges );
			}
		}

// 6) Entrance warp door on perimeter (prefers halls)
		int egx, egy, edir;
		
		if ( FP_FindPerimeterEntrance( grid, W, H, egx, egy, edir ) )
		{
			Object warpDoor = FP_SpawnWarpDoorOnEdge( origin, egx, egy, edir );
			FP_AddDoorEdge( doorEdges, egx, egy, edir );

			if ( warpDoor )
			{
				vector mpLocal = "0 0 0";
				
				if ( warpDoor.MemoryPointExists( "teleport_point" ) )
					mpLocal = warpDoor.GetMemoryPointPos( "teleport_point" );

				vector tpWorld = warpDoor.ModelToWorld( mpLocal );
				
				if ( !FP_IsWalkableAtWorld( origin, grid, W, H, tpWorld ) )
				{
					vector ori = warpDoor.GetOrientation();
					ori[0] = ori[0] + 180.0;
					warpDoor.SetOrientation(ori);
					tpWorld = warpDoor.ModelToWorld( mpLocal );
				}

				g_FP_WarpDoorTP_Pos = tpWorld;
				vector doorPos = warpDoor.GetPosition();
				vector dir = tpWorld - doorPos; dir[1] = 0.0;
				
				if ( dir.Length() > 0.001 ) dir.Normalize();
				else dir = warpDoor.GetDirection();
				
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

// 7) Walls: base + top layers (+3m), skip base at door edges
		FP_GenerateWalls( origin, grid, W, H, doorEdges );

// 8) Ceilings for every non-empty cell
		for ( int cy = 0; cy < H; cy++ )
		{
			for ( int cx = 0; cx < W; cx++ )
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}

// 9) One container per contiguous room region
		FP_SpawnContainersForAllRooms( grid, W, H, origin, floorObjs, doorEdges );

// 10 ) Spawn Additional Parts
	// Spawn Obsticles
		
		FP_SpawnObjectAtGrid(	origin,	6, 	0,	"fmdpk_modscifiwalls_doorframe_laser_wide",	90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	6,	0,	"fmdpk_modscifiwalls_halfwall_full",		90.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	6,	1,	"fmdpk_modscifiwalls_halfwall_full",		90.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	10, 13,	"fmdpk_modscifiwalls_doorframe_coded_wide",	90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	10,	13,	"fmdpk_modscifiwalls_halfwall_full",		90.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	10,	14,	"fmdpk_modscifiwalls_halfwall_full",		90.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
	}
}
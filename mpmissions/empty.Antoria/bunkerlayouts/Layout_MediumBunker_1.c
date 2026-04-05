class Layout_MediumBunker_1 : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 12;
		const int H = 13;

// 1) Build Grid
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

// 2) HALLWAYS ( FP_HALL )
		FP_GridFillRect(	grid,	5,	0,	2,	13,	FP_HALL );
		FP_GridFillRect(	grid,	0,	6,	12,	1,	FP_HALL );

// 3) ROOMS ( FP_ROOM )
		FP_GridFillRect(	grid,	0,	0, 	5,	5,	FP_ROOM );
		FP_GridFillRect(	grid, 	7,	0,	5, 	5, 	FP_ROOM );
		FP_GridFillRect(	grid, 	0,	8,	5, 	5, 	FP_ROOM );
		FP_GridFillRect(	grid, 	7,	8,	5, 	5, 	FP_ROOM );

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

// 10 ) Cut Holes into Grid
		FP_DeleteFloorTile( 0, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 1, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 2, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 3, 6, W, H, grid, floorObjs );
		
		FP_DeleteFloorTile( 8, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 9, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 10, 6, W, H, grid, floorObjs );
		FP_DeleteFloorTile( 11, 6, W, H, grid, floorObjs );
		
		// FP_DeleteFloorTile( 1, 3, W, H, grid, floorObjs );
		// FP_DeleteFloorTile( 10, 3, W, H, grid, floorObjs );
		// FP_DeleteFloorTile( 1, 9, W, H, grid, floorObjs );
		// FP_DeleteFloorTile( 10, 9, W, H, grid, floorObjs );

// 11 ) Spawn Additional Parts
	// Spawn Obsticles
		FP_SpawnObjectAtGrid(	origin,	6, 	5,	"fmdpk_modscifiwalls_doorframe_coded_wide",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	6,	5,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	6, 	7,	"fmdpk_modscifiwalls_doorframe_coded_wide",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	5,	7,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	6,	7,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		
		// FP_SpawnObjectAtGrid(	origin,	1,	3,	"fmdpk_modscifiwalls_floor_trap",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		// FP_SpawnObjectAtGrid(	origin,	10,	3,	"fmdpk_modscifiwalls_floor_trap",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		// FP_SpawnObjectAtGrid(	origin,	1,	9,	"fmdpk_modscifiwalls_floor_trap",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		// FP_SpawnObjectAtGrid(	origin,	10,	9,	"fmdpk_modscifiwalls_floor_trap",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	0,	5,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	0,	7,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	11,	5,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	11,	7,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	0,	6,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	6,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	0,	5,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	4,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	3,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	2,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	1,	2,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	11,	5,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	4,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	3,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	2,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	10,	2,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	0,	7,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	8,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	9,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	0,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	1,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	11,	7,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	8,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	9,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	11,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	10,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	10,	6,	"fmdpk_modscifiwalls_floor_slope",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	9,	6,	"fmdpk_modscifiwalls_floor_slope",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	8,	6,	"fmdpk_modscifiwalls_floor_slope",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		
		FP_SpawnObjectAtGrid(	origin,	1,	6,	"fmdpk_modscifiwalls_floor_slope",			180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	6,	"fmdpk_modscifiwalls_floor_slope",			180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	3,	6,	"fmdpk_modscifiwalls_floor_slope",			180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		//
		FP_SpawnObjectOnEdge(	origin,	1,	2,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	1,	2,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	1,	2,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	2,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	2,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	3,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	3,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	4,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	4,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	5,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	5,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	10,	2,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	10,	2,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	10,	2,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	2,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	2,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	3,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	3,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	4,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	4,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	5,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	5,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	9,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	9,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	8,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	8,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	7,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	0,	7,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	10,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	10,	10,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	10,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	10,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	9,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	9,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	8,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	8,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	7,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	11,	7,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	0,	6,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	11,	6,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	1,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	1,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	2,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	2,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	3,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	3,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	4,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	4,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	7,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	7,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	8,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	8,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	9,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	9,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
		FP_SpawnObjectOnEdge(	origin,	10,	6,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectOnEdge(	origin,	10,	6,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		//
	}
}
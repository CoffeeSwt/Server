class Layout_CustomGrid41x41 : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 41;
		const int H = 41;

// 1) Build grid (FP_EMPTY by default)
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

// 2) HALLWAYS ( FP_HALL )
		FP_GridFillRect( grid,	0,	0,	41,	2,	FP_HALL );
		FP_GridFillRect( grid,	0,	7,	41,	4,	FP_HALL );
		FP_GridFillRect( grid,	0,	23,	41,	4,	FP_HALL );
		FP_GridFillRect( grid,	0,	39,	41,	2,	FP_HALL );
		
		FP_GridFillRect( grid,	0,	0,	2,	41,	FP_HALL );
		FP_GridFillRect( grid,	19,	0,	2,	41,	FP_HALL );
		FP_GridFillRect( grid,	39,	0,	2,	41,	FP_HALL );

// 3) ROOMS ( FP_ROOM )
		FP_GridFillRect( grid,	2, 	2, 	5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	8, 	2, 	5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	14, 2, 	5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	21, 2, 	5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	27, 2, 	5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	33, 2, 	5, 	5, 	FP_ROOM );
		
		FP_GridFillRect( grid, 	2, 	11, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	8, 	11, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	14, 11, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	21, 11, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	27, 11, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	33, 11, 5, 	5, 	FP_ROOM );
		
		FP_GridFillRect( grid, 	2, 	18, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	8, 	18, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	14, 18, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	21, 18, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	27, 18, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	33, 18, 5, 	5, 	FP_ROOM );
		
		FP_GridFillRect( grid, 	2, 	27, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	8, 	27, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	14, 27, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	21, 27, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	27, 27, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	33, 27, 5, 	5, 	FP_ROOM );
		
		FP_GridFillRect( grid, 	2, 	33, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	8, 	33, 5, 	5,	FP_ROOM );
		FP_GridFillRect( grid, 	14, 33, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	21, 33, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	27, 33, 5, 	5, 	FP_ROOM );
		FP_GridFillRect( grid, 	33, 33, 5, 	5, 	FP_ROOM );

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
		
		FP_SpawnObjectAtGrid( origin, 0, 0, "fmdpk_modscifiwalls_floor", 0.0 );

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
		FP_DeleteFloorTile(		0, 		10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		0, 		19,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		1, 		19,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		0, 		20,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		1, 		20,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		0, 		21,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(		1, 		21,	W,	H,	grid,	floorObjs );
		
// 11 ) Spawn Additional Parts
	// Spawn Obsticles
		FP_SpawnObjectAtGrid(	origin,	0, 	5,	"fmdpk_modscifiwalls_doorframe_laser",		0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	0,	5,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	1,	5,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	1,	5,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	0,	18,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	18,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	18,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	18,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	19,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	19,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	19,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	19,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
		// Entrance Wall Obsticles
		FP_SpawnObjectOnEdge(	origin,	1,	0,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	1,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	0,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	1,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	0,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	1,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	0,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	1,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		
		// First 2-Wide Hallway Wall Obsticles
		FP_SpawnObjectOnEdge(	origin,	31,	0,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	1,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	0,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	1,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	0,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	1,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	0,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	1,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		
		// Second 2-Wide Hallway Wall Obsticles
		FP_SpawnObjectOnEdge(	origin,	31,	39,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	40,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	39,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	31,	40,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	39,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	40,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	39,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	32,	40,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		
		// First 4-Wide Hallway Divider
		FP_SpawnObjectOnEdge(	origin,	20,	7,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	10,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	7,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	10,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	7,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	10,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	7,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	10,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	8,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	9,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	8,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	9,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	20, 8,	"fmdpk_modscifiwalls_doorframe_coded_wide",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	0 );
		
		// Second 4-Wide Hallway Divider
		FP_SpawnObjectOnEdge(	origin,	20,	23,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	26,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	23,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	26,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	26,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	26,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	24,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	25,	"fmdpk_modscifiwalls_halfwall",				1.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	25,	"fmdpk_modscifiwalls_halfwall",				3.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	-0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	20, 24,	"fmdpk_modscifiwalls_doorframe_coded_wide",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	0 );
		
		// 4-Wide Dividers
		FP_SpawnObjectOnEdge(	origin,	39,	17,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	17,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	17,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	17,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	16,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	16,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	16,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	16,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
		// First 4-Wide Section, Closets
		FP_SpawnObjectOnEdge(	origin,	39,	7,	"fmdpk_modscifiwalls_doorframe_coded",		0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	39,	10,	"fmdpk_modscifiwalls_doorframe_coded",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	39,	23,	"fmdpk_modscifiwalls_doorframe_coded",		0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	39,	26,	"fmdpk_modscifiwalls_doorframe_coded",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	40,	7,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	7,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	7,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	6,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	6,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	6,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	40,	11,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	11,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	11,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	10,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	10,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	10,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	40,	23,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	23,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	23,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	22,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	22,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	22,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	40,	27,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	27,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	27,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	26,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	39,	26,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	40,	26,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );

	// Spawn First 4-Wide Hall Beds
		FP_SpawnObjectOnEdge(	origin,	4,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	5,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	6,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	8,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	9,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	10,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	12,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	13,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	14,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	16,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	17,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	18,	8,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );

	// Spawn Second 4-Wide Hall Beds
		FP_SpawnObjectOnEdge(	origin,	4,	24,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	5,	24,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectOnEdge(	origin,	6,	24,	"fmdpk_modscifiwalls_static_medicalbed",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
	// Spawn Lootable Containers
		FP_SpawnObjectOnEdge(	origin,	2, 	5,	"fmdpk_modscifiwalls_medicalcab",			1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	1.2 );
		
	// Spawn Trap 1 and Exit Slope 1
		FP_SpawnObjectAtGrid(	origin,	0,	10,	"fmdpk_modscifiwalls_floor_trap",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	0,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	1,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	0,	11,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	1,	11,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	0,	12,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	1,	12,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	11,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	11,	"fmdpk_modscifiwalls_doorframe_laser",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectOnEdge(	origin,	0,	11,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	11,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	11,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	11,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	12,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	12,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	12,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	12,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	12,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	12,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	13,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	13,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	13,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	13,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	13,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	13,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	13,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	1,	13,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	14,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	14,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	14,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	14,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	14,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	14,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	14,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	1,	14,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	15,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1,		0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	15,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	15,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,		0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	15,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	15,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	15,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	15,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 2 );
		FP_SpawnObjectAtGrid(	origin,	1,	15,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 2 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	16,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	16,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	16,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	16,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	16,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	1,	16,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	17,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	17,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	17,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	17,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	17,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		FP_SpawnObjectAtGrid(	origin,	1,	17,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	18,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	18,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	18,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	18,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	18,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		FP_SpawnObjectAtGrid(	origin,	1,	18,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	19,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	19,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	19,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	1,	19,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	20,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	20,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	20,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	1,	20,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	21,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	21,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.3 );
		FP_SpawnObjectAtGrid(	origin,	0,	21,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		FP_SpawnObjectAtGrid(	origin,	1,	21,	"fmdpk_modscifiwalls_floor_slope",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		
		//Exit Doorframe
		FP_SpawnObjectOnEdge(	origin,	1, 	23,	"fmdpk_modscifiwalls_doorframe_laser_wide",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	0,	23,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	23,	"fmdpk_modscifiwalls_halfwall",				0.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	0,	22,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	1,	22,	"fmdpk_modscifiwalls_halfwall",				2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		
	// Spawn Security Room
		// Spawn Security Room Floors
		FP_SpawnObjectAtGrid(	origin,	2,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	11,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	12,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	11,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	12,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_floor",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		// Spawn Security Room Walls
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		//
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	11,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	3,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	3,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	11,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	11,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	3,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	3,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 	0.3 );
		
		// Spawn Security Room Ceilings
		FP_SpawnObjectAtGrid(	origin,	2,	10,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	11,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	12,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	10,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	11,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	12,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_ceiling",				0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
	}
}
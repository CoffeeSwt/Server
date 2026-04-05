class Layout_OpenAreaBunker : BaseLayout
{
	override void Spawn(vector origin)
	{
		const int W = 28;
		const int H = 28;

		// 1) BUILD GRID
		array<ref array<int>> grid;
		FP_GridInit(grid, W, H);

		// 2) DEFINE HALLS
		FP_GridFillRect(grid,  8,	1,	2,	7,	FP_HALL);
		FP_GridFillRect(grid,  8,	8,	10,	11,	FP_HALL);
		FP_GridFillRect(grid,  12,	19, 2,	7,	FP_HALL);
		FP_GridFillRect(grid,  13,	7,	1,	1,	FP_HALL);
		FP_GridFillRect(grid,  17,	7,	1,	1,	FP_HALL);
		FP_GridFillRect(grid,  11,	22,	1,	1,	FP_HALL);
		FP_GridFillRect(grid,  14,	22,	1,	1,	FP_HALL);
		FP_GridFillRect(grid,  5,	26,	16,	2,	FP_HALL);

		// 3) DEFINE ROOMS
		FP_GridFillRect(grid,	11,	2,	5,	5,	FP_ROOM);
		FP_GridFillRect(grid,	17,	2,	5,	5,	FP_ROOM);
		FP_GridFillRect(grid,	6,	20,	5,	5,	FP_ROOM);
		FP_GridFillRect(grid,	15,	20,	5,	5,	FP_ROOM);

		// 4) SPAWN FLOORS
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid(floorObjs, W, H);

		for (int y = 0; y < H; y++)
		{
			for (int x = 0; x < W; x++)
			{
				int cell = grid[y][x];
				if ( cell != FP_ROOM && cell != FP_HALL ) continue;
				Object f = FP_SpawnFloorAt(origin, x, y);
				floorObjs[y][x] = f;
			}
		}
		
		g_FP_FloorObjs = floorObjs;

		// 5) SPAWN DOORS
		array<ref array<bool>> visited = new array<ref array<bool>>;
		for (int vy = 0; vy < H; vy++)
		{
			auto vrow = new array<bool>; vrow.Resize(W);
			visited.Insert(vrow);
		}
		
		array<ref Param3<int,int,int>> doorEdges = new array<ref Param3<int,int,int>>;
		
		for (int yy = 0; yy < H; yy++)
		{
			for (int xx = 0; xx < W; xx++)
			{
				if (grid[yy][xx] == FP_ROOM && !visited[yy][xx])
					FP_PlaceOneDoorForRoomRegion(grid, W, H, origin, xx, yy, visited, doorEdges);
			}
		}

		// 6) SPAWN WARP ENTRANCE
		int egx, egy, edir;
		
		if (FP_FindPerimeterEntrance(grid, W, H, egx, egy, edir))
		{
			Object warpDoor = FP_SpawnWarpDoorOnEdge(origin, egx, egy, edir);
			FP_AddDoorEdge(doorEdges, egx, egy, edir);

			if (warpDoor)
			{
				vector mpLocal = "0 0 0";
				
				if (warpDoor.MemoryPointExists("teleport_point"))
					mpLocal = warpDoor.GetMemoryPointPos("teleport_point");

				vector tpWorld = warpDoor.ModelToWorld(mpLocal);
				
				if (!FP_IsWalkableAtWorld(origin, grid, W, H, tpWorld))
				{
					vector ori = warpDoor.GetOrientation();
					ori[0] = ori[0] + 180.0;
					warpDoor.SetOrientation(ori);
					tpWorld = warpDoor.ModelToWorld(mpLocal);
				}

				g_FP_WarpDoorTP_Pos = tpWorld;
				vector doorPos = warpDoor.GetPosition();
				vector dir = tpWorld - doorPos; dir[1] = 0.0;
				
				if (dir.Length() > 0.001)
					dir.Normalize();
				else
					dir = warpDoor.GetDirection();
				
				g_FP_WarpDoorTP_Dir = dir;
			}
		}

		// 7) SPAWN WALLS
		FP_GenerateWalls(origin, grid, W, H, doorEdges);
		
		SpawnDIBunkerBasementWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		SpawnDIBunkerBasementWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		SpawnDIBunkerBasementHallwayWalls( origin );
		SpawnDIBunkerBasementRoom1( origin );
		SpawnDIBunkerBasementRoom2( origin );
		SpawnDIBunkerBasementRoom3( origin );
		SpawnDIBunkerBasementRoom4( origin );
		
		// 8) SPAWN CEILINGS
		for (int cy = 0; cy < H; cy++)
		{
			for (int cx = 0; cx < W; cx++)
			{
				if ( grid[cy][cx] == FP_ROOM || grid[cy][cx] == FP_HALL )
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}
		
		SpawnDIBunkerBasementCeilings( origin );
		
		// 9) AUTO SPAWN LOOT IN 5x5, OR LARGER, ROOMS
		FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
		
		// 10) CUT HOLES IN FLOORS
		FP_DeleteFloorTile(	8,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	10,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	10,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	12,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	11,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	11,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	12,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	12,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	13,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	13,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	13,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	14,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	14,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	15,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	15,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	12,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	16,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	16,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	12,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	17,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	8,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	9,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	10,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	11,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	14,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	15,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	16,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	17,	18,	W,	H,	grid,	floorObjs );
		
		// 11) SPAWN BASEMENT FLOORS
		SpawnDIBunkerBasementFloors( origin );
		SpawnOpenAreaRamps( origin );
		
		// 12) SPAWN FIRST FLOOR GREEBLES
		SpawnDIBunkerFirstFloorGreebles( origin );
		SpawnDIBunkerBasementGreebles( origin );
		
		// 13) SPAWN FIRST FLOOR TRAPS
		SpawnDIBunkerFloorTraps( origin );
		
		// 14 ) SPAWN TURRET TRAPS
		SpawnDIBunkerTurretTraps( origin );
	}
	
	void SpawnDIBunkerBasementFloors( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	8,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	8,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );

		FP_SpawnObjectAtGrid(	origin,	8,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );

		FP_SpawnObjectAtGrid(	origin,	8,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );

		FP_SpawnObjectAtGrid(	origin,	8,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	16,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	8,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	9,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	10,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin, 11,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	12,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	13,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	14,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	15,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	7,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	7,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	7,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	18,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	18,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	4,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	4,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	4,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	5,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	6,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	19,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	9,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	10,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	11,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	12,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	13,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	14,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	15,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnObjectAtGrid(	origin,	19,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	17,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	18,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	19,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	19,	"fmdpk_modscifiwalls_floor",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
	}
	
	void SpawnDIBunkerBasementCeilings( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	7,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	7,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	7,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	18,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	18,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectAtGrid(	origin,	4,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		
		FP_SpawnObjectAtGrid(	origin,	4,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	4,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	5,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	6,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		
		FP_SpawnObjectAtGrid(	origin,	19,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	9,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	10,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	11,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	12,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	13,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	14,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	15,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		
		FP_SpawnObjectAtGrid(	origin,	19,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	17,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	18,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	19,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	20,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	19,	"fmdpk_modscifiwalls_ceiling",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
	}
	
	void SpawnDIBunkerFirstFloorGreebles( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	8,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	9,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	10,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	11,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	14,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	15,	9,	"fmdpk_modscifiwalls_static_glassrailing",	2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		
		FP_SpawnObjectOnEdge(	origin,	12,	10,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	11,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	12,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	13,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	14,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	15,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	16,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	17,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	12,	18,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		
		FP_SpawnObjectOnEdge(	origin,	13,	10,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	11,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	12,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	13,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	14,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	15,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	16,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	17,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		FP_SpawnObjectOnEdge(	origin,	13,	18,	"fmdpk_modscifiwalls_static_glassrailing",	1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GLASSRAIL_INSET );
		
		FP_SpawnObjectOnEdge(	origin,	16,	16,	"fmdpk_modscifiwalls_static_glassrailing",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	FP_GLASSRAIL_INSET );
		
	}
	
	void SpawnOpenAreaRamps( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	16,	15,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	17,	15,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	16,	14,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	14,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	13,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		FP_SpawnObjectAtGrid(	origin,	17,	13,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		
		FP_SpawnObjectAtGrid(	origin,	16,	12,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	17,	12,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	16,	11,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	11,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	10,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		FP_SpawnObjectAtGrid(	origin,	17,	10,	"fmdpk_modscifiwalls_floor_slope",					90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	10,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2,	FP_GLASSRAIL_INSET_2 );
		FP_SpawnObjectOnEdge(	origin,	16,	11,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1,	FP_GLASSRAIL_INSET_2 );
		FP_SpawnObjectOnEdge(	origin,	16,	12,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,		FP_GLASSRAIL_INSET_2 );
		FP_SpawnObjectOnEdge(	origin,	16,	13,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2,	FP_GLASSRAIL_INSET_2 );
		FP_SpawnObjectOnEdge(	origin,	16,	14,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1,	FP_GLASSRAIL_INSET_2 );
		FP_SpawnObjectOnEdge(	origin,	16,	15,	"fmdpk_modscifiwalls_static_glassrailing_sloped",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,		FP_GLASSRAIL_INSET_2 );
	}
	
	void SpawnDIBunkerBasementWalls( vector origin, float voffset )
	{
		FP_SpawnObjectOnEdge(	origin,	8,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	9,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	10,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	11,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	12,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	13,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	14,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	15,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	16,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	8,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	8,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	8,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	9,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	10,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	11,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	12,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	13,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	14,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	15,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	16,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	8,	8,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	0 );
	}
	
	void SpawnDIBunkerBasementHallwayWalls( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	8,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	8,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	17,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	7,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	10,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	10,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	7,	14,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	14,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	14,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	14,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );		
	}
	
	void SpawnDIBunkerFloorTraps( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	12,	11,	"fmdpk_modscifiwalls_floor_trap_2",		90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	13,	13,	"fmdpk_modscifiwalls_floor_trap_2",		90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	12,	16,	"fmdpk_modscifiwalls_floor_trap_2",		90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	12,	17,	"fmdpk_modscifiwalls_floor_trap_2",		90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
	}
	
	void SpawnDIBunkerTurretTraps( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	8,	2,	"fmdpk_modscifiwalls_turret_1",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	17,	8,	"fmdpk_modscifiwalls_turret_1",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	20,	14,	"fmdpk_modscifiwalls_turret_1",			90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
	}
	
	void SpawnDIBunkerBasementRoom1( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	4,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	6,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	10,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	14,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );

		FP_SpawnObjectOnEdge(	origin,	4,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	4,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	4,	14,	"fmdpk_modscifiwalls_glasswall",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	14,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	14,	"fmdpk_modscifiwalls_glasswall",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	14,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	14,	"fmdpk_modscifiwalls_glasswall",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	14,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
	}
	
	void SpawnDIBunkerBasementRoom2( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	4,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	6,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	18,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );

		FP_SpawnObjectOnEdge(	origin,	4,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	5,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	6,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	4,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	4,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
	}
	
	void SpawnDIBunkerBasementRoom3( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	19,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );

		FP_SpawnObjectOnEdge(	origin,	19,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	19,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	18,	"fmdpk_modscifiwalls_doorframe_coded",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
	}
	
	void SpawnDIBunkerBasementRoom4( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	9,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	21,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );

		FP_SpawnObjectOnEdge(	origin,	19,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	20,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	10,	"fmdpk_modscifiwalls_doorframe_coded",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2,	0 );
		FP_SpawnObjectOnEdge(	origin,	19,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3,	0 );
	}
	
	void SpawnDIBunkerBasementGreebles( vector origin )
	{
		FP_SpawnLabDeskAgainstWall(			origin,	10,	18,	2,	0.65,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	18,	2,	0.65,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	18,	2,	0.65,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	18,	2,	0.65,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	16,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	16,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	16,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	16,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	15,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	15,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	15,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	15,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	14,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	14,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	14,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	14,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	13,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	13,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	13,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	13,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	12,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	12,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	12,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	12,	0,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnLabDeskAgainstWall(			origin,	10,	11,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	11,	11,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	13,	11,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnLabDeskAgainstWall(			origin,	14,	11,	2,	0.66,					FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	11,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	17,	11,	1,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	12,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	17,	12,	1,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	13,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	17,	13,	1,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	15,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	16,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	8,	17,	3,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		
		FP_SpawnStorageShelfAgainstWall(	origin,	19,	9,	0,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	19,	15,	2,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	20,	9,	0,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	20,	15,	2,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	21,	9,	0,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnStorageShelfAgainstWall(	origin,	21,	15,	2,	FP_STORAGESHELF_INSET,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
	}
}
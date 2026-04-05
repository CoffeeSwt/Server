class Layout_2FloorMazeBunker : BaseLayout
{
	override void Spawn( vector origin )
	{
		const int W = 29;
		const int H = 29;

		// 1) BUILD GRID
		array<ref array<int>> grid;
		FP_GridInit( grid, W, H );

		// 2) DEFINE HALLWAYS
		FP_GridFillRect( grid,	6,	3,	3,	24,	FP_HALL );
		FP_GridFillRect( grid,	20,	3,	3,	24,	FP_HALL );
		FP_GridFillRect( grid,	9,	6,	4,	3,	FP_HALL );
		FP_GridFillRect( grid,	16,	6,	4,	3,	FP_HALL );

		// 3) DEFINE ROOMS
		FP_GridFillRect( grid,	1,	0,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	1,	6,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	1,	16,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	9,	1,	5,	5,	FP_ROOM );
		
		FP_GridFillRect( grid,	23,	0,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	23,	6,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	23,	12,	5,	5,	FP_ROOM );
		FP_GridFillRect( grid,	15,	1,	5,	5,	FP_ROOM );

		// 4) SPAWN FLOORS FOR R + H, RECORD FLOOR OBJECTS
		array<ref array<Object>> floorObjs;
		FP_InitFloorObjGrid( floorObjs, W, H );

		for (int y = 0; y < H; y++)
		{
			for (int x = 0; x < W; x++)
			{
				int cell = grid[y][x];
				if (cell != FP_HALL && cell != FP_ROOM) continue;
				Object f = FP_SpawnFloorAt(origin, x, y);
				floorObjs[y][x] = f;
			}
		}
		
		g_FP_FloorObjs = floorObjs;

		// 5) DOORS: ONE CODED DOORFRAME PER CONTIGUOUS ROOM BLOCK
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

		// 6) ENTRANCE WARP DOOR ON PERIMETER (PREFERS HALLS)
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

		// 7) WALLS: BASE LAYER + TOP LAYER (+3M), SKIPPING BASE AT DOORS
		FP_GenerateWalls(origin, grid, W, H, doorEdges);

		// 8) Ceilings: 6m above floor plane for every non-empty cell
		for (int cy = 0; cy < H; cy++)
		{
			for (int cx = 0; cx < W; cx++)
			{
				if (grid[cy][cx] == FP_HALL || grid[cy][cx] == FP_ROOM)
					FP_SpawnCeilingAt(origin, cx, cy);
			}
		}
		
		// 9) ONE CONTAINER PER ROOM (CENTERED ON A FLOOR TILE'S 'ENTITY_SPAWN_POINT')
			FP_SpawnContainersForAllRooms(grid, W, H, origin, floorObjs, doorEdges);
		
		// 10) REMOVE FLOORS FOR RAMPS
			DeleteDIBunkerFloorTiles( W, H, grid, floorObjs );
				
		// 11) ADD FIRST FLOOR ENTRANCES TO BASEMENT
			AddDIBunkerFirstFloorEntrances( origin );

		// 12) ADD BASEMENT FLOORS
			SpawnDIBunkerBasementFloors( origin );

		// 13) ADD BASEMENT WALLS AND DOORS
			SpawnDIBunkerBasementWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
			SpawnDIBunkerBasementWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
			
			SpawnDIBunkerEntranceF1Walls( origin, FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 0.0 );
			SpawnDIBunkerEntranceF1Walls( origin, FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 0.0 );
			
			SpawnDIBunkerBasementEntranceWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
			SpawnDIBunkerBasementEntranceWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.0 );
			SpawnDIBunkerBasementEntranceL3Walls( origin, 0.0 );
			
			SpawnDIBunkerBasementRoomWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
			SpawnDIBunkerBasementRoomWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
			
			SpawnDIBunkerBasementVentWalls( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, -0.3 );
			
			SpawnDIBunkerGreebles( origin );
			
		// 14) ADD BASEMENT WALLS AND DOORS
			SpawnDIBunkerBasementRoomDoors( origin );
			
		// 15) ADD BASEMENT CEILINGS
			SpawnDIBunkerBasementCeilings( origin );
			SpawnDIBunkerBasementVentCeilings( origin, FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 - 0.4 );
			
		// 16) CREATE VERTICAL TRAPS
			SpawnDIBunkerVerticalShafts( origin );
			
		// 17) ADD LOOT TO BASEMENTS
			SpawnDIBunkerBasementLoot( origin );
	}

	void DeleteDIBunkerFloorTiles( int W, int H, array<ref array<int>> grid, array<ref array<Object>> floorObjs )
	{
		FP_DeleteFloorTile( 7,	24,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	23,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	22,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	21,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	20,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	19,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 7,	16,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile( 21,	24,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 21,	23,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 21,	22,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 21,	21,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 21,	20,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile( 21,	19,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	21,	18,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	21,	17,	W,	H,	grid,	floorObjs );
		FP_DeleteFloorTile(	21,	16,	W,	H,	grid,	floorObjs );
		
		FP_DeleteFloorTile(	7,	8,	W,	H,	grid,	floorObjs );
	}
	
	void SpawnDIBunkerBasementLoot( vector origin)
	{
		FP_SpawnLockerAgainstWall( origin,	2,	5,	1.0, FP_LOCKER_INSET,		FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnLockerAgainstWall( origin,	3,	12,	2.0, FP_LOCKER_INSET,		FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnConatinerAgainstWall( origin,	25,	4,	2.0, FP_CONTAINER_INSET, 	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnLockerAgainstWall( origin,	26,	11,	3.0, FP_LOCKER_INSET, 		FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnConatinerAgainstWall( origin,	25,	18,	0.0, FP_CONTAINER_INSET, 	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnMedicalAgainstWall( origin,	3,	16,	0.0, FP_MEDICAL_INSET, 		FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
	}
	
	void SpawnDIBunkerVerticalShafts( vector origin )
	{
		FP_SpawnObjectOnEdge(	origin,	7,	8,	"fmdpk_modscifiwalls_halfwall_full",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.0 );
		FP_SpawnObjectOnEdge(	origin,	7,	8,	"fmdpk_modscifiwalls_halfwall_full",		1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.0 );
		FP_SpawnObjectOnEdge(	origin,	7,	8,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.0 );
		FP_SpawnObjectOnEdge(	origin,	7,	8,	"fmdpk_modscifiwalls_halfwall_full",		3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, 	0.0 );
		
		FP_SpawnObjectAtGrid(	origin,	7,	8,	"fmdpk_modscifiwalls_floor_trap_2",			0.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	7,	8,	"fmdpk_modscifiwalls_floor_trap_2",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
	}
	
	void AddDIBunkerFirstFloorEntrances( vector origin )
	{
		int randomBasementEnt1 = Math.RandomInt(0,2);
		int randomBasementEnt2 = Math.RandomInt(0,2);
		
		if ( randomBasementEnt1 )
			FP_SpawnObjectOnEdge(	origin,	7,	15,	"fmdpk_modscifiwalls_doorframe_coded",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		else
			FP_SpawnObjectOnEdge(	origin,	7,	15,	"fmdpk_modscifiwalls_doorframe_laser",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		
		if ( randomBasementEnt2 )
			FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_doorframe_coded",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		else
			FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_doorframe_laser",		2.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET, 	0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	7,	15,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
		FP_SpawnObjectOnEdge(	origin,	21,	15,	"fmdpk_modscifiwalls_halfwall_full",		2.0,	FIRST_FLOOR_UPPER_HEIGHT_OFFSET, 	0.3 );
	}
	
	void SpawnDIBunkerBasementFloors( vector origin )
	{
			// SPAWN LEFT HALLWAY FLOORS
		FP_SpawnObjectAtGrid(	origin,	6,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	6,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	8,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN RIGHT HALLWAY FLOORS
		FP_SpawnObjectAtGrid(	origin,	20,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	20,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	22,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN SIDE ROOM HALLWAY FLOORS
		FP_SpawnObjectAtGrid(	origin,	5,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	5,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	5,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	23,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	23,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	23,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN LEFT SIDEROOM FLOORS
		FP_SpawnObjectAtGrid(	origin,	2,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectAtGrid(	origin,	2,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectAtGrid(	origin,	2,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	2,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	3,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	4,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN RIGHT SIDEROOM FLOORS
		FP_SpawnObjectAtGrid(	origin,	24,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectAtGrid(	origin,	24,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin, 26,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
		FP_SpawnObjectAtGrid(	origin,	24,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	24,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	25,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	26,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN VENT ENTRANCE SLOPED FLOORS
		FP_SpawnObjectAtGrid(	origin,	9,	3,	"fmdpk_modscifiwalls_floor_slope",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	9,	9,	"fmdpk_modscifiwalls_floor_slope",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	9,	24,	"fmdpk_modscifiwalls_floor_slope",		0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	19,	3,	"fmdpk_modscifiwalls_floor_slope",		180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	19,	9,	"fmdpk_modscifiwalls_floor_slope",		180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	19,	24,	"fmdpk_modscifiwalls_floor_slope",		180.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		
			// SPAWN BASEMENT SLOPED FLOORS
		FP_SpawnObjectAtGrid(	origin,	7,	24,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	21,	24,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 );
		FP_SpawnObjectAtGrid(	origin,	7,	23,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	23,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	7,	22,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 2 );
		FP_SpawnObjectAtGrid(	origin,	21,	22,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 2 );
		FP_SpawnObjectAtGrid(	origin,	7,	21,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	21,	21,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 );
		FP_SpawnObjectAtGrid(	origin,	7,	20,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	20,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 1 );
		FP_SpawnObjectAtGrid(	origin,	7,	19,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		FP_SpawnObjectAtGrid(	origin,	21,	19,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 + 2 );
		FP_SpawnObjectAtGrid(	origin,	7,	18,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	18,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	17,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	21,	17,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 1 );
		FP_SpawnObjectAtGrid(	origin,	7,	16,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		FP_SpawnObjectAtGrid(	origin,	21,	16,	"fmdpk_modscifiwalls_floor_slope",		90.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 + 2 );
		
			// SPAWN VENTING FLOORS
		FP_SpawnObjectAtGrid(	origin,	13,	2,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	11,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	13,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	15,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	3,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	11,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	15,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	4,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	11,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	5,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	11,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	6,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	11,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	7,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	12,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	8,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	9,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	10,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	11,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	13,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	12,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	13,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	11,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	14,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	12,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	15,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	12,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	16,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	11,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	15,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	17,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	18,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	13,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	15,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	19,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	20,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	21,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	11,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	22,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	12,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	23,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	10,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	11,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	18,	24,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	14,	25,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
				
		FP_SpawnObjectAtGrid(	origin,	11,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	12,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	13,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	14,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	15,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	16,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
		FP_SpawnObjectAtGrid(	origin,	17,	26,	"fmdpk_modscifiwalls_floor",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1 + 1 );
	}
	
	void SpawnDIBunkerGreebles( vector origin )
	{
		FP_SpawnObjectAtGrid(	origin,	9,	6,	"fmdpk_modscifiwalls_static_wetfloorsign",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	9,	8,	"fmdpk_modscifiwalls_static_wetfloorsign",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	19,	6,	"fmdpk_modscifiwalls_static_wetfloorsign",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	19,	8,	"fmdpk_modscifiwalls_static_wetfloorsign",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );

		FP_SpawnObjectAtGrid(	origin,	7,	10,	"fmdpk_modscifiwalls_static_medicalbed",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		FP_SpawnObjectAtGrid(	origin,	17,	7,	"fmdpk_modscifiwalls_static_medicalbed",			90.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET );
		
		FP_SpawnObjectOnEdge(	origin,	8,	11,	"fmdpk_modscifiwalls_static_greeble_1",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GREEBLE_1_INSET );
		FP_SpawnObjectOnEdge(	origin,	8,	16,	"fmdpk_modscifiwalls_static_greeble_2",				1.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GREEBLE_1_INSET );
		
		FP_SpawnObjectOnEdge(	origin,	20,	9,	"fmdpk_modscifiwalls_static_greeble_1",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GREEBLE_2_INSET );
		FP_SpawnObjectOnEdge(	origin,	20,	17,	"fmdpk_modscifiwalls_static_greeble_2",				3.0,	FIRST_FLOOR_LOWER_HEIGHT_OFFSET,	FP_GREEBLE_2_INSET );
	}
	
	void SpawnDIBunkerBasementRoomWalls( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, float hoffset = 0.3 )
	{
		FP_SpawnObjectOnEdge(	origin,	2,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	4,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	6,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	4,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	5,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	6,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
				// 2
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
				// 3
		FP_SpawnObjectOnEdge(	origin,	2,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	3,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	4,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	2,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		
			// SPAWN BASEMENT RIGHT ROOM WALLS
				// 1
		FP_SpawnObjectOnEdge(	origin,	24,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	4,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	4,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	5,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	6,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	6,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	4,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	6,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
				// 2
		FP_SpawnObjectOnEdge(	origin,	24,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	10,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	12,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
				// 3
		FP_SpawnObjectOnEdge(	origin,	24,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	25,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	26,	18,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	24,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
	}
	
	void SpawnDIBunkerBasementRoomDoors( vector origin )
	{
		int randomBasementDoor1 = Math.RandomInt(0,2);
		int randomBasementDoor2 = Math.RandomInt(0,2);
		int randomBasementDoor3 = Math.RandomInt(0,2);
		int randomBasementDoor4 = Math.RandomInt(0,2);
		int randomBasementDoor5 = Math.RandomInt(0,2);
		int randomBasementDoor6 = Math.RandomInt(0,2);
		
		if ( randomBasementDoor1 )
			FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_doorframe_laser",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		if ( randomBasementDoor2 )
			FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_doorframe_laser",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		if ( randomBasementDoor3 )
			FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_doorframe_coded",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_doorframe_laser",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		if ( randomBasementDoor4 )
			FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_doorframe_coded",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_doorframe_laser",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		if ( randomBasementDoor5 )
			FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_doorframe_coded",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_doorframe_laser",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		if ( randomBasementDoor6 )
			FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_doorframe_coded",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		else
			FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_doorframe_laser",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
			// SPAWN VENT HATCHES
		FP_SpawnObjectOnEdge(	origin,	10,	3,	"fmdpk_modscifiwalls_doorframe_vent",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		FP_SpawnObjectOnEdge(	origin,	10,	9,	"fmdpk_modscifiwalls_doorframe_vent",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		FP_SpawnObjectOnEdge(	origin,	10,	24,	"fmdpk_modscifiwalls_doorframe_vent",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	3,	"fmdpk_modscifiwalls_doorframe_vent",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		FP_SpawnObjectOnEdge(	origin,	18,	9,	"fmdpk_modscifiwalls_doorframe_vent",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
		FP_SpawnObjectOnEdge(	origin,	18,	24,	"fmdpk_modscifiwalls_doorframe_vent",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.0 );
	}
	
	void SpawnDIBunkerBasementWalls( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, float hoffset = 0.3 )
	{
		FP_SpawnObjectOnEdge(	origin,	6,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );

		FP_SpawnObjectOnEdge(	origin,	6,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	6,	2,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	3,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	4,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	6,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	7,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	8,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	9,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	10,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	6,	25,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	8,	2,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	5,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	6,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	7,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	8,	25,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	voffset,	hoffset );
		
			// SPAWN RIGHT BASEMENT HALLWAY WALLS
		FP_SpawnObjectOnEdge(	origin,	20,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	2,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );

		FP_SpawnObjectOnEdge(	origin,	20,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	25,	"fmdpk_modscifiwalls_halfwall_full",	2.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	20,	2,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	5,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	6,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	7,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	12,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	13,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	14,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	15,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	20,	25,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	22,	2,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	3,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	4,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	6,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	7,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	8,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	9,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	10,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	12,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	13,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	14,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	15,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	22,	25,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );	
	}
	
	void SpawnDIBunkerBasementEntranceWalls( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, float hoffset = 0.3 )
	{
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		//
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset, hoffset );
	}

	void SpawnDIBunkerBasementEntranceL3Walls( vector origin, float hoffset = 0.3 )
	{
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	25,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	25,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3, hoffset );
	}
	
	void SpawnDIBunkerBasementVentWalls( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, float hoffset = 0.3 )
	{
				// Vent Ents
		FP_SpawnObjectOnEdge(	origin,	8,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	8,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	8,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	20,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		
				// R2
		FP_SpawnObjectOnEdge(	origin,	13,	2,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	2,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	2,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R3
		FP_SpawnObjectOnEdge(	origin,	9,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	9,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	10,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	11,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	13,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	15,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	19,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	19,	3,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R4
		FP_SpawnObjectOnEdge(	origin,	11,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	15,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	4,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R5
		FP_SpawnObjectOnEdge(	origin,	11,	5,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	5,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	5,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	5,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R6
		FP_SpawnObjectOnEdge(	origin,	11,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	6,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R7
		FP_SpawnObjectOnEdge(	origin,	11,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	7,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R8
		FP_SpawnObjectOnEdge(	origin,	12,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	8,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R9
		FP_SpawnObjectOnEdge(	origin,	9,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	9,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	10,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	19,	9,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R10
		FP_SpawnObjectOnEdge(	origin,	10,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	10,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R11
		FP_SpawnObjectOnEdge(	origin,	10,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	11,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R12
		FP_SpawnObjectOnEdge(	origin,	10,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	13,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	12,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R13
		FP_SpawnObjectOnEdge(	origin,	10,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	13,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R14
		FP_SpawnObjectOnEdge(	origin,	10,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	11,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	14,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R15
		FP_SpawnObjectOnEdge(	origin,	12,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	15,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R16
		FP_SpawnObjectOnEdge(	origin,	12,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	16,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R17
		FP_SpawnObjectOnEdge(	origin,	10,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	11,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	15,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	17,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R18
		FP_SpawnObjectOnEdge(	origin,	10,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	18,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R19
		FP_SpawnObjectOnEdge(	origin,	10,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	13,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	15,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	19,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R20
		FP_SpawnObjectOnEdge(	origin,	10,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	20,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R21
		FP_SpawnObjectOnEdge(	origin,	10,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	21,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R22
		FP_SpawnObjectOnEdge(	origin,	10,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	11,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	22,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R23
		FP_SpawnObjectOnEdge(	origin,	12,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	23,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R24
		FP_SpawnObjectOnEdge(	origin,	9,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	9,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	10,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	10,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	11,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	18,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	18,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	19,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	19,	24,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R25
		FP_SpawnObjectOnEdge(	origin,	14,	25,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	14,	25,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
				// R26
		FP_SpawnObjectOnEdge(	origin,	11,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	11,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	12,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	12,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	13,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	13,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	14,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	15,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	15,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	16,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	16,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
		FP_SpawnObjectOnEdge(	origin,	17,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	17,	26,	"fmdpk_modscifiwalls_halfwall_full_cornered",	2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		
			// SPAWN BASEMENT ROOM ENTRANCE HALLWAYS
				
				// L1
		FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall",			1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	5,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	6,	5,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
				
				// L2
		FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_halfwall",			1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	11,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	6,	11,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
				
				// L3
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall",			1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	5,	17,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	6,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
				
				// L4
		FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	5,	"fmdpk_modscifiwalls_halfwall",			3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	22,	5,	"fmdpk_modscifiwalls_halfwall",			1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	22,	5,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
				
				// L5
		FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	11,	"fmdpk_modscifiwalls_halfwall",			3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	22,	11,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
				
				// L6
		FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_1, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_halfwall",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_halfwall",			2.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	23,	17,	"fmdpk_modscifiwalls_halfwall",			3.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
		FP_SpawnObjectOnEdge(	origin,	22,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_2, 0.3 );
	}
	
	void SpawnDIBunkerEntranceF1Walls( vector origin, float voffset = FIRST_FLOOR_LOWER_HEIGHT_OFFSET, float hoffset = 0.3 )
	{
		FP_SpawnObjectOnEdge(	origin,	7,	25,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	25,	"fmdpk_modscifiwalls_halfwall_full",	0.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	7,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	16,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	17,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	18,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	19,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	20,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	21,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	22,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	23,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	3.0,	voffset,	hoffset );
		FP_SpawnObjectOnEdge(	origin,	21,	24,	"fmdpk_modscifiwalls_halfwall_full",	1.0,	voffset,	hoffset );
	}
	
	void SpawnDIBunkerBasementCeilings( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 )
	{
		FP_SpawnObjectAtGrid(	origin,	6,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	7,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	6,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	7,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	8,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
			// SPAWN RIGHT HALLWAY CEILINGS
		FP_SpawnObjectAtGrid(	origin,	20,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		// FP_SpawnObjectAtGrid(	origin,	21,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	20,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	21,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	22,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
			// SPAWN SIDE ROOM HALLWAY CEILINGS
		FP_SpawnObjectAtGrid(	origin,	5,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	5,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	5,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	23,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	23,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	23,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
			// SPAWN LEFT SIDEROOM CEILINGS
		FP_SpawnObjectAtGrid(	origin,	2,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	2,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	2,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	2,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	3,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	4,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
			// SPAWN RIGHT SIDEROOM CEILINGS
		FP_SpawnObjectAtGrid(	origin,	24,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	24,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin, 26,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		
		FP_SpawnObjectAtGrid(	origin,	24,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	24,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	25,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
		FP_SpawnObjectAtGrid(	origin,	26,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	FIRST_FLOOR_PIT_HEIGHT_OFFSET_3 );
	}
	
	void SpawnDIBunkerBasementVentCeilings( vector origin, float voffset = FIRST_FLOOR_PIT_HEIGHT_OFFSET_2 )
	{
		FP_SpawnObjectAtGrid(	origin,	13,	2,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	9,	3,	"fmdpk_modscifiwalls_floor_slope",		180.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	10,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	11,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	13,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	15,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	3,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	19,	3,	"fmdpk_modscifiwalls_floor_slope",		0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	11,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	15,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	4,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	11,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	5,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	11,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	6,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	11,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	7,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	12,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	8,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	9,	9,	"fmdpk_modscifiwalls_floor_slope",		180.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	10,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	9,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	19,	9,	"fmdpk_modscifiwalls_floor_slope",		0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	10,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	11,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	13,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	12,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	13,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	11,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	14,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	12,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	15,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	12,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	16,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	11,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	15,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	17,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	18,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	13,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	15,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	19,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	20,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	21,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	10,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	11,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	22,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	12,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	23,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	9,	24,	"fmdpk_modscifiwalls_floor_slope",		180.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	10,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	11,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	18,	24,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	19,	24,	"fmdpk_modscifiwalls_floor_slope",		0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	14,	25,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
				
		FP_SpawnObjectAtGrid(	origin,	11,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	12,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	13,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	14,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	15,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	16,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
		FP_SpawnObjectAtGrid(	origin,	17,	26,	"fmdpk_modscifiwalls_ceiling",			0.0,	voffset );
	}
}